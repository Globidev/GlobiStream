#include "Dialogs.h"

HostChangerDialog::HostChangerDialog(const QString & _address, const quint16 _port) : QDialog()
{
    UiHostDialog::setupUi(this);
    ui_inputAddress->setText(_address);
    ui_inputPort->setValue(_port);

    connect(ui_promptValidator, SIGNAL(accepted()),
            this,               SLOT(onAccepted()));

    connect(ui_promptValidator, SIGNAL(rejected()),
            this,               SLOT(onRejected()));
}

HostChangerDialog::~HostChangerDialog()
{

}

HostInfo HostChangerDialog::getConnectionConfig(const QString & defaultAddress, const quint16 defaultPort, bool & accepted)
{
    HostChangerDialog dialog(defaultAddress, defaultPort);
    dialog.exec();
    accepted = (dialog.result() == QDialog::Accepted);
    return dialog.values;
}

void HostChangerDialog::onAccepted() 
{ 
    setResult(QDialog::Accepted);
    values.hostAddress = ui_inputAddress->text();
    values.port = ui_inputPort->value();
}

void HostChangerDialog::onRejected() 
{ 
    setResult(QDialog::Rejected);
}

PathChangerDialog::PathChangerDialog(const QString & _path) : QDialog()
{
    UiPathDialog::setupUi(this);
    ui_inputPath->setText(_path);

    connect(ui_pathBrowse, SIGNAL(clicked()),
            this,          SLOT(onBrowse()));

    connect(ui_promptValidator, SIGNAL(accepted()),
            this,               SLOT(onAccepted()));

    connect(ui_promptValidator, SIGNAL(rejected()),
            this,               SLOT(onRejected()));
}

PathChangerDialog::~PathChangerDialog()
{
    
}

QString PathChangerDialog::getPathInfo(const QString & defaultPath, bool & accepted)
{
    PathChangerDialog dialog(defaultPath);
    dialog.exec();
    accepted = (dialog.result() == QDialog::Accepted);
    return dialog.path;
}

void PathChangerDialog::onAccepted() 
{ 
    setResult(QDialog::Accepted);
    path = ui_inputPath->text();
}
void PathChangerDialog::onRejected() 
{ 
    setResult(QDialog::Rejected);
}

void PathChangerDialog::onBrowse()
{
    QString path = QFileDialog::getOpenFileName(0, QString(), ui_inputPath->text());
    if(!path.isEmpty())
        ui_inputPath->setText(path);
}

GomTVAccountDialog::GomTVAccountDialog(const QString & gomUsername, const QString & gomPassword) : QDialog()
{
    UiGomTVDialog::setupUi(this);
    ui_gomUsername->setText(gomUsername);
    ui_gomPassword->setText(gomPassword);

    connect(ui_promptValidator, SIGNAL(accepted()),
            this,               SLOT(onAccepted()));

    connect(ui_promptValidator, SIGNAL(rejected()),
            this,               SLOT(onRejected()));
}

GomTVAccountDialog::~GomTVAccountDialog()
{

}

void GomTVAccountDialog::onAccepted() 
{ 
    setResult(QDialog::Accepted);
    account.username = ui_gomUsername->text();
    account.password = ui_gomPassword->text();
}
void GomTVAccountDialog::onRejected() 
{ 
    setResult(QDialog::Rejected);
}

AccountInfo GomTVAccountDialog::getGomAccountInfo(const AccountInfo & account, bool & accepted)
{
    GomTVAccountDialog dialog(account.username, account.password);
    dialog.exec();
    accepted = (dialog.result() == QDialog::Accepted);
    return dialog.account;
}