#ifndef SERVERUI_H
#define SERVERUI_H

#include "ui_ServerUi.h"
#include "Poller.h"
#include "Event.h"
#include <QtGui>
#include <GlobiNetwork>


static const int pollingInterval(60000);
static const int alertInterval(15000);
static const QString dataFile("Data\\Streams.gdb");
static const QString eventFile("Data\\Event.gdb");
static const quint16 port(1337);

static const QString listenOk("Listening on %1");
static const QString listenFaild("ERROR, can not listen on %1");
static const QString icon(":/GlobiStreamServer/Icon");

using namespace GlobiNetwork;

enum PacketType
{
    StreamsUpdate, // StreamList
    StreamMonitoringRequest, // QString, QString
    StreamMonitoringResponse, // QString, bool
    EventsUpdate, // Event
    EventAddingRequest, // Event
    EventAddingResponse, // QString, bool
    EventChangementRequest // int, Event
};

class ServerUi : public QMainWindow, public Ui::UiServer
{
    Q_OBJECT

    public :
        ServerUi();
        ~ServerUi();

        void loadStreams();
        void loadEvents();
        QVariantList serializedStreamData();
        virtual void timerEvent(QTimerEvent *);

        void listen();
        void notifyNewRequest(const QVariant & data, QTcpSocket * sender);
        void notifyNewEvent(const QVariant & data, QTcpSocket * sender);
        void monitorNewStream(const QString & name, const QString & url);
        void addNewEvent(const Event & event);
        void changeEvent(const QVariant & data);
        void sendEventUpdate();

        void saveStreams();
        void saveEvents();

    private slots :
        void onThreadFinished();
        void onItemPressed(QListWidgetItem *);
        void onRemoveStreamRequested();

        void on_ui_a_quit_triggered(bool) { qApp->quit(); }
        void on_ui_a_changePort_triggered(bool);

        void onTrayActivated(QSystemTrayIcon::ActivationReason);
        void onPacketReceived(quint16 packetType, const QVariant & content, QTcpSocket * sender);
        void onClientConnected(QTcpSocket * client);
        void onItemChanged(QListWidgetItem * item);

    private :
        QList <Poller *> _pollers;
        GlobiTcpServer * server;
        quint16 _port;
        QSystemTrayIcon * tray;
        QMenu * context;
        QList <Event> _events;
};

#endif // SERVERUI_H