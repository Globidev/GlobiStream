#include <QtSingleApplication>
#include "ServerUi.h"
#include "SingleAppManager.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);
    qRegisterMetaTypeStreamOperators<Event>("Event");
    if(app.isRunning())
        app.sendMessage(QString());
    else
    {
        app.setQuitOnLastWindowClosed(false);
        ServerUi ui;
        SingleAppManager manager(&ui);
        QObject::connect(&app, SIGNAL(messageReceived(const QString &)),
                         &manager, SLOT(raise(const QString &)));
        ui.show();
        return app.exec();
    }
    return 0;
}
