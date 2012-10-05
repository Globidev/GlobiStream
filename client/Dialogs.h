#ifndef DIALOGS_H
#define DIALOGS_H

#include <QFileDialog>

#include "ui_HostDialogUi.h"
#include "ui_PathDialogUi.h"
#include "ui_GomTVDialogUi.h"

struct HostInfo
{
    QString hostAddress;
    quint16 port;
};

struct AccountInfo
{
    QString username;
    QString password;
};

class HostChangerDialog : public QDialog, public Ui::UiHostDialog
{
    Q_OBJECT
    
    public :
        HostChangerDialog(const QString & _address, const quint16 _port);
        ~HostChangerDialog();

        static HostInfo getConnectionConfig(const QString & defaultAddress, const quint16 defaultPort, bool & accepted);

    private slots :
        void onAccepted();
        void onRejected();

    private :
        HostInfo values;
};

class PathChangerDialog : public QDialog, public Ui::UiPathDialog
{
    Q_OBJECT
    
    public :
        PathChangerDialog(const QString & _path);
        ~PathChangerDialog();

        static QString getPathInfo(const QString & defaultPath, bool & accepted);

    private slots :
        void onAccepted();
        void onRejected();

        void onBrowse();

    private :
        QString path;
};

class GomTVAccountDialog : public QDialog, public Ui::UiGomTVDialog
{
    Q_OBJECT

    public :
        GomTVAccountDialog(const QString & gomUsername, const QString & gomPassword);
        ~GomTVAccountDialog();

        static AccountInfo getGomAccountInfo(const AccountInfo & account, bool & accepted);

    private slots :
        void onAccepted();
        void onRejected();

    private :
        AccountInfo account;
};

#endif // DIALOGS_H