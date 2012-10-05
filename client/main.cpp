#include "MainUi.h"
#include "SingleAppManager.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    qRegisterMetaTypeStreamOperators<Event>("Event");
    if(a.isRunning())
    {
        a.sendMessage(QString());
    }
    else
    {
        a.setQuitOnLastWindowClosed(false);
        MainUi ui;
        SingleAppManager appManager(&ui);
        QObject::connect(&a, SIGNAL(messageReceived(const QString &)),
                         &appManager, SLOT(raise(const QString &)));
        ui.show();
        return a.exec();
    }
    return 0;
}
