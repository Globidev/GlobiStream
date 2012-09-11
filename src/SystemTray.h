#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>

#include "Global.h"
#include "Event.h"

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT

    public :
        SystemTray();
        ~SystemTray();

        void showMessage(const QString & title, const QString & message);
        void extractSchedules(const QList <Event> & events);

    protected :
        void timerEvent(QTimerEvent *);

    private :
        QMenu * contextMenu;
        QAction * disableNotifications;
        QMap <QDateTime, QStringList> _schedules;
};

#endif // SYSTEMTRAY_H