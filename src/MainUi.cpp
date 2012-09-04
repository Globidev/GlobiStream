#include "MainUi.h"

MainUi::MainUi() : chatPoppedOut(false), hostAddress(DEFAULT_HOST_ADDRESS), port(DEFAULT_HOST_PORT), mediaPlayerPath(DEFAULT_PLAYER_PATH)
{
    init();
    loadConfig();
    setUpConnections();

    // GUI startup behavior
    tray->show();
}

MainUi::~MainUi()
{
    saveCookies();
    socket->close();
    delete socket;
    delete commandProcess;

    delete tray;
    delete streamTable;
    delete chatBrowser;
    delete outputConsole;
    if(clientSideStreamAction)
        delete clientSideStreamAction;
}

void MainUi::init()
{
    // GUI
    UiGStream::setupUi(this);

    tray = new SystemTray();
    streamTable = new StreamTable();
    eventTable = new EventTable();
    chatBrowser = new ChatBrowser();
    outputConsole = new QTextBrowser();
    clientSideStreamAction = NULL;

    ui_dockChat->setWidget(chatBrowser);
    ui_dockOutput->setWidget(outputConsole);
    ui_mainLayout->addWidget(streamTable);
    ui_eventTableLayout->addWidget(eventTable);

    // Core
    commandProcess = new CommandProcess();

    socket = new GLobiTcpSocket();
    socket->setHost(hostAddress, port);
    socket->setAutoConnect(true);
}

void MainUi::loadConfig()
{
    createProjectFolder(PROJECT_NAME);
    QFile confFile(filePathForProject(PROJECT_NAME));
    if(confFile.exists())
    {
        confs = GlobiUtils::getConfFromFile(&confFile);
        mediaPlayerPath = confs.value(CACHE_KEY_PLAYER_PATH, DEFAULT_PLAYER_PATH);
        gomAccount.username = confs.value(CACHE_KEY_GOM_USERNAME, QString());
        gomAccount.password = confs.value(CACHE_KEY_GOM_PASSWORD, QString());
        QString cookieString(confs.value(CACHE_KEY_COOKIES, QString()));
        QList <QNetworkCookie> cookies;
        foreach(const QString & cookie, cookieString.split(CACHE_COOKIES_SEPARATOR))
            cookies << QNetworkCookie::parseCookies(QByteArray().append(cookie));
        chatBrowser->setCookies(cookies);
    }
}

void MainUi::setUpConnections()
{
    // GUI
    QObject::connect(streamTable, SIGNAL(watchClicked(const QString &, const QString &)),
                     this, SLOT(startStream(const QString &, const QString &)));
    QObject::connect(streamTable, SIGNAL(chatClicked(const QString &, const QString &)),
                     chatBrowser, SLOT(openChat(const QString &, const QString &)));

    QObject::connect(ui_dockChat, SIGNAL(topLevelChanged(bool)),
                     this, SLOT(onChatFloatingRequested(bool)));

    QObject::connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                     this, SLOT(onTrayActivated(QSystemTrayIcon::ActivationReason)));
    QObject::connect(tray, SIGNAL(messageClicked()),
                     this, SLOT(onTrayMessageClicked()));

    // Core
    QObject::connect(socket, SIGNAL(connected()),
                     this, SLOT(onConnected()));
    QObject::connect(socket, SIGNAL(disconnected()),
                     this, SLOT(onDisconnected()));
    QObject::connect(socket, SIGNAL(packetReceived(quint16, const QVariant &)),
                     this, SLOT(onPacketReceived(quint16, const QVariant &)));

    
}

void MainUi::saveCookies()
{
    chatBrowser->dump();
    QString cookieString;
    foreach(const QNetworkCookie & cookie, chatBrowser->cookies())
        cookieString += cookie.toRawForm() + CACHE_COOKIES_SEPARATOR;
    cookieString.remove(cookieString.size() - CACHE_COOKIES_SEPARATOR.size(), CACHE_COOKIES_SEPARATOR.size());
    confs.insert(CACHE_KEY_COOKIES, cookieString);
    writeConfFile(PROJECT_NAME, confs);
}

void MainUi::unPackStream(const QVariant & data)
{
    StreamList streams;
    QString newOnlineStreams;
    foreach(const QVariant & streamData, data.toList())
    {
        QVariantList streamAttrs(streamData.toList());
        Stream stream(streamAttrs.at(0).toString(), streamAttrs.at(1).toString());
        stream.online = qvariant_cast<int>(streamAttrs.at(2));
        stream.qualities = streamAttrs.at(3).toStringList();
        streams << stream;
    }

    foreach(const Stream & stream, streams)
        if(stream.online == Online && streamTable->lastStates().value(stream.url, Offline) != Online)
            newOnlineStreams += QString(" - %1\n").arg(stream.name);

    if(!newOnlineStreams.isEmpty())
        tray->showMessage(NEW_ONLINE_STREAMS_TITLE, NEW_ONLINE_STREAMS_NOTIFICATION.arg(newOnlineStreams));

    streamTable->buildTable(streams);
}

void MainUi::unPackEvents(const QVariant & data)
{
    QList <Event> events;
    foreach(const QVariant & event, data.toList())
        events << event.value<Event>();
    eventTable->buildTable(events);
}

void MainUi::notifyStreamMonitoringResponse(const QVariant & data)
{
    QVariantList dataList(data.toList());
    QString streamName(dataList.at(0).toString());
    bool accepted(dataList.at(1).toBool());

    if(accepted)
        QMessageBox::information(this, "Wut", QString("%1 is now monitored").arg(streamName));
    else
        QMessageBox::critical(this, "Not Wut", QString("Server refused to monitor %1").arg(streamName));
}

void MainUi::notifyEventAddingResponse(const QVariant & data)
{
    QVariantList dataList(data.toList());
    QString eventName(dataList.at(0).toString());
    bool accepted(dataList.at(1).toBool());

    if(accepted)
        QMessageBox::information(this, "Wut", QString("%1 was added").arg(eventName));
    else
        QMessageBox::critical(this, "Not Wut", QString("Server refused to add %1").arg(eventName));
}


void MainUi::buildClientActionWidget(const QStringList & qualities)
{
    clientSideStreamAction = new StreamActionWidget(qualities, ui_urlEdit->text());
    ui_singleStreamLayout->addWidget(clientSideStreamAction);

    QObject::connect(clientSideStreamAction, SIGNAL(watchClicked(const QString &, const QString &)),
                        this, SLOT(startStream(const QString &, const QString &)));

    QObject::connect(clientSideStreamAction, SIGNAL(chatClicked(const QString &, const QString &)),
                        chatBrowser, SLOT(openChat(const QString &, const QString &)));
}

void MainUi::closeEvent(QCloseEvent * event)
{
    ui_dockChat->setFloating(false);
    QMainWindow::closeEvent(event);
}

void MainUi::on_ui_poll_clicked()
{
    if(ui_urlEdit->text().isEmpty())
        return;
    // Disabling polling capabilities
    WidgetDisabler disabler(ui_poll);

    // Reseting clientActionWidget
    if(clientSideStreamAction)
    {
        delete clientSideStreamAction;
        clientSideStreamAction = NULL;
    }

    // Polling with waiting bar
    QProgressBar bar;
    bar.setMinimum(0); bar.setMaximum(0);
    ui_singleStreamLayout->addWidget(&bar);

    commandProcess->restart();
    QString command(POLLING_COMMAND.arg(ui_urlEdit->text()));
    if(ui_urlEdit->text().contains(GOM_URL_TOKEN))
        command += GOM_ARGS.arg(gomAccount.username, gomAccount.password);
    command += EXIT_COMMAND_ADD;
    commandProcess->write(command);
    commandProcess->waitForComputingFinished();
    
    // Parsing results
    bool streamOnline;
    QStringList qualities(parseQualitiesFromOutput(commandProcess->readAllStandardOutput(),
                                                   streamOnline));
    if(streamOnline)
        buildClientActionWidget(qualities);
}

void MainUi::on_ui_addToMonitored_clicked()
{
    bool accepted;
    QString streamName(QInputDialog::getText(this, "Stream name", "Enter a name for the stream", QLineEdit::Normal, QString(), &accepted));

    if(accepted)
    {
        socket->sendPacket(StreamMonitoringRequest, QVariantList() << ui_urlEdit->text() << streamName);
        ui_statusBar->showMessage("Waiting for server's monitoring approval");
    }
}

void MainUi::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        show();
        activateWindow();
    }
}

void MainUi::on_ui_addEvent_clicked()
{
    bool added;
    Event e(AddEventDialog::getEvent(added));
    if(added)
    {
        socket->sendPacket(EventAddingRequest, QVariant::fromValue(e));
        ui_statusBar->showMessage("Waiting for server's monitoring approval");
    }
}

void MainUi::onChatFloatingRequested(bool floating)
{
    // Disabling recursive call of this slots
    EventDisabler disabler(ui_dockChat, SIGNAL(topLevelChanged(bool)),
                           this, SLOT(onChatFloatingRequested(bool)));

    // No Dragging
    if(!QApplication::mouseButtons().testFlag(Qt::LeftButton))
    {
        ui_dockChat->setFloating(false);
        ui_dockChat->setWidget(chatPoppedOut ? chatBrowser : 0);
        chatBrowser->setParent(chatPoppedOut ? ui_dockChat : 0);
        chatBrowser->show();
        chatBrowser->move(this->mapToGlobal(ui_dockChat->pos()));
    }
    chatPoppedOut = !chatPoppedOut;
}

void MainUi::startStream(const QString & url, const QString & quality)
{
    commandProcess->restart();
    QObject::connect(commandProcess, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(onReadyReadStandardOutput()));
    QObject::connect(commandProcess, SIGNAL(readyReadStandardError()),
                     this, SLOT(onReadyReadStandardError()));
    QString command(START_LIVESTREAMER_COMMAND_DEFAULT.arg(url, quality, mediaPlayerPath));
    if(url.contains(GOM_URL_TOKEN))
        command += GOM_ARGS.arg(gomAccount.username, gomAccount.password);
    commandProcess->write(command);
}

void MainUi::onConnected()
{
    ui_addToMonitored->setEnabled(true);
    tray->showMessage(QString(), CONNECTED_TO_HOST.arg(hostAddress).arg(port));
}

void MainUi::onDisconnected()
{
    ui_addToMonitored->setDisabled(true);
    tray->showMessage(QString(), LOST_CONNECTION);
}

void MainUi::onPacketReceived(quint16 packetType, const QVariant & content)
{
    switch(packetType)
    {
        case StreamsUpdate :
            unPackStream(content);
            break;

        case StreamMonitoringResponse :
            notifyStreamMonitoringResponse(content);
            ui_statusBar->clearMessage();
            break;

        case EventsUpdate :
            unPackEvents(content);
            break;

        case EventAddingResponse :

            ui_statusBar->clearMessage();
            break;

        default :
            break;
    }
}

void MainUi::on_ui_a_netProperties_triggered(bool)
{
    bool changeValue;
    HostInfo info = HostChangerDialog::getConnectionConfig(hostAddress, port, changeValue);
    if(changeValue)
    {
        socket->close();
        socket->disconnectFromHost();
        socket->setHost(info.hostAddress, info.port);
        hostAddress = info.hostAddress;
        port = info.port;
    }
}

void MainUi::on_ui_a_mediaPlayer_triggered(bool)
{
    bool changeValue;
    QString path = PathChangerDialog::getPathInfo(mediaPlayerPath, changeValue);
    if(changeValue)
    {
        mediaPlayerPath = path;
        confs.insert(CACHE_KEY_PLAYER_PATH, mediaPlayerPath);
        writeConfFile(PROJECT_NAME, confs);
    }
}

void MainUi::on_ui_a_showOfflineStreams_triggered(bool enabled)
{
    streamTable->setShowOfflinePages(enabled);
    streamTable->rebuildTable();
}

void MainUi::on_ui_a_Non_Monitored_Streams_triggered(bool enabled)
{
    streamTable->setShowNonMonitoredPages(enabled);
    streamTable->rebuildTable();
}

void MainUi::on_ui_a_gomTV_triggered(bool)
{
    bool changeValue;
    AccountInfo account = GomTVAccountDialog::getGomAccountInfo(gomAccount, changeValue);
    if(changeValue)
    {
        gomAccount = account;
        confs.insert(CACHE_KEY_GOM_USERNAME, gomAccount.username);
        confs.insert(CACHE_KEY_GOM_PASSWORD, gomAccount.password);
        writeConfFile(PROJECT_NAME, confs);
    }
}

void MainUi::on_ui_a_about_triggered(bool)
{
    QDesktopServices::openUrl(QUrl("https://github.com/Globicodeur/GlobiStream"));
}