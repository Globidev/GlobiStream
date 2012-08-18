#include "SystemTray.h"

SystemTray::SystemTray() : QSystemTrayIcon(QIcon(APPLICATION_ICON))
{
    contextMenu = new QMenu();

    disableNotifications = new QAction("Disable Notifications", contextMenu);
    disableNotifications->setCheckable(true);
    disableNotifications->setChecked(false);

    contextMenu->addAction(disableNotifications);
    contextMenu->addAction("Quit", qApp, SLOT(quit()));

    setContextMenu(contextMenu);
}

SystemTray::~SystemTray()
{

}

void SystemTray::showMessage(const QString & title, const QString & message)
{
    if(!disableNotifications->isChecked())
        QSystemTrayIcon::showMessage(title, message);
}