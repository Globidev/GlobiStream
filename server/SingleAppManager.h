#ifndef SINGLEAPPMANAGER_H
#define SINGLEAPPMANAGER_H

#include <QWidget>

class SingleAppManager : public QObject
{
    Q_OBJECT

    public :
        SingleAppManager(QWidget * w) : QObject(), _w(w) { }
        ~SingleAppManager() { }

    private slots :
        void raise(const QString &) { _w->show(); _w->raise(); _w->activateWindow(); }

    private :
        QWidget * _w;
};

#endif // SINGLEAPPMANAGER_H