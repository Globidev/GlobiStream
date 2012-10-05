#ifndef POLLER_H
#define POLLER_H

#include <QThread>
#include <QProcess>

static const QString POLLING_COMMAND("livestreamer %1");
static const QString GOM_ARGS(" --gomtv-username %1 --gomtv-password %2");
static const QString EXIT_COMMAND(" & exit\n");
static const QString OUTPUT_QUALITIES_LINE_BEGIN("Found streams: ");
static const QString QUALITIES_SEPARATOR(", ");
static const QString GOM_URL_TOKEN("gomtv.net");
static const QString GOM_USERNAME("gomaccount");
static const QString GOM_PASSWORD("gompassword");

enum StreamState
{
    Online,
    Offline,
    UnWatched
};

class Poller : public QThread
{
    Q_OBJECT

    public :
        Poller(const QString & name, const QString & streamUrl);
        ~Poller();

        virtual void run();
        QStringList qualities() const { return _qualities; }
        QString url() const { return _url; }
        QString name() const { return _name; }
        StreamState state() const;

        void setActive(bool active) { _active = active; }
        bool active() const { return _active; }

    private :
        QString _name;
        QString _url;
        QProcess * commandProcess;
        QStringList _qualities;
        bool _online;
        bool _active;
};

#endif // POLLER_H