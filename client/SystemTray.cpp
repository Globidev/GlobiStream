#include "SystemTray.h"
#include <QDebug>

SystemTray::SystemTray() : QSystemTrayIcon(QIcon(APPLICATION_ICON))
{
    contextMenu = new QMenu();

    disableNotifications = new QAction("Disable Notifications", contextMenu);
    disableNotifications->setCheckable(true);
    disableNotifications->setChecked(false);

    contextMenu->addAction(disableNotifications);
    contextMenu->addAction("Quit", qApp, SLOT(quit()));

    setContextMenu(contextMenu);
    startTimer(10000);
}

SystemTray::~SystemTray()
{

}

void SystemTray::showMessage(const QString & title, const QString & message)
{
    if(!disableNotifications->isChecked())
        QSystemTrayIcon::showMessage(title, message);
}

void SystemTray::extractSchedules(const QList <Event> & events)
{
    _schedules.clear();
    foreach(const Event & event, events)
    {
        QMapIterator <QDateTime, QString> it(event.schedules());
        while(it.hasNext())
        {
            it.next();
            if(it.key().time().isValid())
                _schedules[it.key()].append(it.value());
        }
    }
}

void SystemTray::timerEvent(QTimerEvent *)
{
    QMapIterator <QDateTime, QStringList> it(_schedules);
    QStringList startingSoonEvents;
    while(it.hasNext())
    {
        it.next();
        if(it.key().date() == QDate::currentDate())
        {
            int secsTo(it.key().time().secsTo(QTime::currentTime()));
            qDebug() << secsTo;
            if(secsTo >= -30 && secsTo <= 0)
            {
                startingSoonEvents << it.value();
                _schedules.remove(it.key());
            }
        }
    }

    if(!startingSoonEvents.isEmpty())
        showMessage("Wut", QString("Events are starting soon :\n%1").arg(startingSoonEvents.join("\n")));
}