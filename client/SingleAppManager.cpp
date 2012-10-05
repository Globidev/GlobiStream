#include "SingleAppManager.h"

SingleAppManager::SingleAppManager(QWidget * window) : QObject(), _window(window)
{

}

void SingleAppManager::raise(const QString& message)
{
    _window->show();
    _window->raise();
    _window->activateWindow();
}