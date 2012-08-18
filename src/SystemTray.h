#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>

#include "Global.h"
#include <QDebug>

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT

    public :
        SystemTray();
        ~SystemTray();

        void showMessage(const QString & title, const QString & message);

    private :
        QMenu * contextMenu;
        QAction * disableNotifications;
};

#endif // SYSTEMTRAY_H