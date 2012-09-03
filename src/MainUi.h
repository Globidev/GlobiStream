#ifndef MAINUI_H
#define MAINUI_H

#include <GlobiNetwork>
#include <GlobiUtils>
#include <QProgressBar>
#include <QTextBrowser>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>

#include "StreamTable.h"
#include "ChatBrowser.h"
#include "CommandProcess.h"
#include "Dialogs.h"
#include "SystemTray.h"
#include "Event.h"
#include "EventTable.h"
#include "ui_GStreamUi.h"

using namespace GlobiNetwork;
using namespace GlobiUtils;

class MainUi : public QMainWindow, public Ui::UiGStream
{
    Q_OBJECT

    public :
        MainUi();
        ~MainUi();

        void init();
        void loadConfig();
        void setUpConnections();

        void unPackStream(const QVariant & data);
        void unPackEvents(const QVariant & data);
        void notifyStreamMonitoringResponse(const QVariant & data);
        void buildClientActionWidget(const QStringList & qualities);
        void saveCookies();

        virtual void closeEvent(QCloseEvent *);

    private slots :
        // Menus Actions
        void on_ui_a_quit_triggered(bool) { qApp->quit(); }
        void on_ui_a_netProperties_triggered(bool);
        void on_ui_a_mediaPlayer_triggered(bool);
        void on_ui_a_showOfflineStreams_triggered(bool);
        void on_ui_a_Non_Monitored_Streams_triggered(bool);
        void on_ui_a_gomTV_triggered(bool);
        void on_ui_a_about_triggered(bool);

        // GUI user interactions
        void on_ui_poll_clicked();
        void on_ui_addToMonitored_clicked();
        void onChatFloatingRequested(bool);
        void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
        void onTrayMessageClicked() { show(); }

        // Core Events
        void startStream(const QString & url, const QString & quality);
        void onConnected();
        void onDisconnected();
        void onPacketReceived(quint16 packetType, const QVariant & content);
        void onReadyReadStandardOutput() { outputConsole->append(commandProcess->readAllStandardOutput()); }
        void onReadyReadStandardError() { outputConsole->append(commandProcess->readAllStandardError()); }

    private :
        // GUI
        SystemTray * tray;
        StreamTable * streamTable;
        EventTable * eventTable;
        ChatBrowser * chatBrowser;
        QTextBrowser * outputConsole;
        StreamActionWidget * clientSideStreamAction;
        bool chatPoppedOut;

        // Core
        CommandProcess * commandProcess;
        QString mediaPlayerPath;

        AccountInfo gomAccount;

        GLobiTcpSocket * socket;
        QString hostAddress;
        quint16 port;

        // GConf
        QMap <QString, QString> confs;
};

class WidgetDisabler
{
    public :
        WidgetDisabler(QWidget * w) : _w(w) { _w->setDisabled(true); }
        ~WidgetDisabler() { _w->setEnabled(true); }

    private :
        QWidget * _w;
};

class EventDisabler
{
    public :
        EventDisabler(QObject * sender, const char * signal,
        QObject * receiver, const char * slot) :
        _sender(sender), _signal(signal),
        _receiver(receiver), _slot(slot)
        {
            _sender->disconnect();
        }
        ~EventDisabler()
        {
            QObject::connect(_sender, _signal, _receiver, _slot);
        }

    private :
        QObject * _sender, * _receiver;
        const char * _signal, * _slot;
};

#endif // MAINUI_H