#include "Poller.h"

Poller::Poller(const QString & name, const QString & streamUrl) 
    : _name(name), _url(streamUrl), _qualities(QStringList()), _online(false), _active(true)
{
    commandProcess = NULL;
}

Poller::~Poller()
{
}

void Poller::run()
{
    if(_active)
    {
        commandProcess = new QProcess();
        commandProcess->start("cmd");
        commandProcess->waitForStarted();
        QString command(POLLING_COMMAND.arg(_url));
        if(_url.contains(GOM_URL_TOKEN))
            command += GOM_ARGS.arg(GOM_USERNAME, GOM_PASSWORD);
        command += EXIT_COMMAND;
        commandProcess->write(command.toStdString().c_str());
        commandProcess->waitForFinished();

        QString output = commandProcess->readAllStandardOutput();
        _qualities = QStringList();
        foreach(QString line, output.split('\n'))
        {
            if(line.startsWith(OUTPUT_QUALITIES_LINE_BEGIN))
            {
                _online = true;
                _qualities = line.remove(OUTPUT_QUALITIES_LINE_BEGIN)
                                 .remove('\r').remove('\n')
                                 .split(QUALITIES_SEPARATOR);
                break;
            }
            else
                _online = false;
        }
        delete commandProcess;
    }
    exit();
}

StreamState Poller::state() const
{
    if(_active)
    {
        if(_online) return Online;
        else return Offline;
    }
    return UnWatched;
}