#include "ServerUi.h"

ServerUi::ServerUi() : _port(port)
{
    Ui::UiServer::setupUi(this);

    loadStreams();
    loadEvents();

    server = new GlobiTcpServer();
    QObject::connect(server, SIGNAL(socketConnected(QTcpSocket *)),
                     this, SLOT(onClientConnected(QTcpSocket *)));
    QObject::connect(server, SIGNAL(packetReceived(quint16, const QVariant &, QTcpSocket *)),
                     this, SLOT(onPacketReceived(quint16, const QVariant &, QTcpSocket *)));
    listen();

    context = new QMenu();
    context->addAction("Remove stream from database", this, SLOT(onRemoveStreamRequested()));

    startTimer(alertInterval);

    tray = new QSystemTrayIcon(QIcon(icon));

    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onTrayActivated(QSystemTrayIcon::ActivationReason)));
    tray->show();
    QMenu * contextTray = new QMenu();
    tray->setContextMenu(contextTray);
    contextTray->addAction("Quit", qApp, SLOT(quit()));

    QObject::connect(ui_list, SIGNAL(itemChanged(QListWidgetItem *)),
                     this, SLOT(onItemChanged(QListWidgetItem *)));
    QObject::connect(ui_list, SIGNAL(itemPressed(QListWidgetItem *)),
                     this, SLOT(onItemPressed(QListWidgetItem *)));
}

ServerUi::~ServerUi()
{
    delete tray;
}

void ServerUi::loadStreams()
{
    QFile file(dataFile);

    if(file.open(QIODevice::ReadOnly))
        while(!file.atEnd())
        {
            QStringList streamInfo(QString(file.readLine()).remove('\r').remove('\n').split(' '));
            QString url(streamInfo.last());
            streamInfo.pop_back();
            QString streamName(streamInfo.join(" "));
            QListWidgetItem * item = new QListWidgetItem(url);
            item->setCheckState(Qt::Checked);
            ui_list->addItem(item);
            Poller * p = new Poller(streamName, url);
            connect(p, SIGNAL(finished()), this, SLOT(onThreadFinished()));
            p->start();
            _pollers << p;
        }
}

void ServerUi::loadEvents()
{
    QDomDocument document;
    QFile file(eventFile);

    if(document.setContent(&file))
    {
        QDomElement eventsElement(document.documentElement());
        for(QDomElement eventElement(eventsElement.firstChildElement()); !eventElement.isNull(); eventElement = eventElement.nextSiblingElement())
            _events << Event::fromXmlElement(eventElement);
    }   
}

void ServerUi::onThreadFinished()
{
    Poller * signalSender = qobject_cast <Poller *> (sender());
    if(signalSender)
        QTimer::singleShot(pollingInterval, signalSender, SLOT(start()));
}

QVariantList ServerUi::serializedStreamData()
{
    QVariantList serializedData;
    foreach(Poller * p, _pollers)
    {
        QVariantList streamInfo;
        streamInfo  << p->name() 
                    << p->url()
                    << p->state()
                    << p->qualities();
        serializedData << QVariant(streamInfo);
    }
    return serializedData;
}

void ServerUi::timerEvent(QTimerEvent *)
{
    server->sendPacketToAll(StreamsUpdate, serializedStreamData());
    ui_peers->clear();
    QStringList peers;
    foreach(QTcpSocket * socket, server->sockets().values())
        peers << socket->peerAddress().toString();
    ui_peers->addItems(peers);
}

void ServerUi::listen()
{
    foreach(QTcpSocket * socket, server->sockets().values())
        socket->close();
    server->close();
    if(server->listen(QHostAddress::Any, _port))
        ui_statusBar->showMessage(listenOk.arg(_port));
    else
        ui_statusBar->showMessage(listenFaild.arg(_port));
}

void ServerUi::on_ui_a_changePort_triggered(bool)
{
    _port = QInputDialog::getInt(0, "Port", "Change dat port", _port, 1, 65535);
    listen();
}

void ServerUi::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        show();
}

void ServerUi::onPacketReceived(quint16 packetType, const QVariant & content, QTcpSocket * sender)
{
    switch(packetType)
    {
        case StreamMonitoringRequest :
            notifyNewRequest(content, sender);
            break;

        case EventAddingRequest :
            notifyNewEvent(content, sender);
            break;

        case EventChangementRequest :
            changeEvent(content);
            break;

        default :
            break;
    }
}

void ServerUi::onClientConnected(QTcpSocket * client)
{
    sendEventUpdate();
}

void ServerUi::onItemChanged(QListWidgetItem * item)
{
    _pollers.at(ui_list->row(item))->setActive(item->checkState() == Qt::Checked);
}

void ServerUi::notifyNewRequest(const QVariant & data, QTcpSocket * sender)
{
    QVariantList dataList(data.toList());
    QString streamUrl(dataList.at(0).toString());
    QString streamName(dataList.at(1).toString());

    raise();
    show();
    activateWindow();

    int response(QMessageBox::question(this, "New monitoring request", QString("Monitor %1 : %2 ?").arg(streamName, streamUrl), QMessageBox::Yes | QMessageBox::No));

    if(response == QMessageBox::Yes)
        monitorNewStream(streamName, streamUrl);
    sender->write(generateNetworkPacket(StreamMonitoringResponse, QVariantList() << streamName << (response == QMessageBox::Yes)));
}

void ServerUi::notifyNewEvent(const QVariant & data, QTcpSocket * sender)
{
    Event event(data.value<Event>());

    raise();
    show();
    activateWindow();

    int response(QMessageBox::question(this, "New Event request", QString("Add Event %1 ?").arg(event.name()), QMessageBox::Yes | QMessageBox::No));

    if(response == QMessageBox::Yes)
        addNewEvent(event);
    sender->write(generateNetworkPacket(EventAddingResponse, QVariantList() << event.name() << (response == QMessageBox::Yes)));
}

void ServerUi::monitorNewStream(const QString & name, const QString & url)
{
    QFile file(dataFile);

    if(file.open(QIODevice::Append))
        file.write(QString("%1 %2\n").arg(name, url).toStdString().c_str());
    
    Poller * p = new Poller(name, url);
    connect(p, SIGNAL(finished()), this, SLOT(onThreadFinished()));
    p->start();
    _pollers << p;

    QListWidgetItem * item = new QListWidgetItem(url);
    item->setCheckState(Qt::Checked);
    ui_list->addItem(item);
}

void ServerUi::addNewEvent(const Event & event)
{
    _events << event;
    saveEvents();
    sendEventUpdate();
}

void ServerUi::changeEvent(const QVariant & data)
{
    QVariantList dataList(data.toList());
    int eventIndex(dataList.at(0).toInt());
    Event e(dataList.at(1).value <Event>());

    _events.replace(eventIndex, e);
    saveEvents();
    sendEventUpdate();
}

void ServerUi::sendEventUpdate()
{
    QVariantList v;
    foreach(const Event & e, _events)
        v << QVariant::fromValue(e);
    server->sendPacketToAll(EventsUpdate, v);
}

void ServerUi::onItemPressed(QListWidgetItem * item)
{
    if(QApplication::mouseButtons().testFlag(Qt::RightButton))
        context->exec(qApp->overrideCursor()->pos());
}

void ServerUi::onRemoveStreamRequested()
{
    int streamIndex(ui_list->currentRow());
    Poller * poller(_pollers.at(streamIndex));
    QListWidgetItem * currentItem(ui_list->takeItem(streamIndex));

    poller->quit();

    _pollers.removeAt(streamIndex);
    delete poller;
    delete currentItem;
    saveStreams();
}

void ServerUi::saveStreams()
{
    QFile file(dataFile);
    if(file.open(QIODevice::WriteOnly))
    {
        foreach(Poller * p, _pollers)
            file.write(QString("%1 %2\n").arg(p->name(), p->url()).toStdString().c_str());
    }
}

void ServerUi::saveEvents()
{
    QDomDocument document;
    QDomElement eventsElement(document.createElement(XML_TAG_EVENTS));
    foreach(const Event & event, _events)
        eventsElement.appendChild(event.toXmlElement());
    document.appendChild(eventsElement);

    QFile file(eventFile);
    if(file.open(QIODevice::WriteOnly))
        file.write(document.toByteArray());
}