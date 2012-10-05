#ifndef SINGLEAPPMANAGER_H
#define SINGLEAPPMANAGER_H

#include <QtSingleApplication>
#include <QWidget>

class SingleAppManager : public QObject
{
    Q_OBJECT
    public:
        SingleAppManager(QWidget * window);
        ~SingleAppManager() { }

    public slots:
        void raise(const QString& message);

    private :
        QWidget * _window;
};

#endif // SINGLEAPPMANAGER_H