#include "AddEventDialog.h"

AddEventDialog::AddEventDialog(const Event & event) : QDialog(), _event(event)
{
    Ui::UiAddEventDialog::setupUi(this);

    if(event.beginDate().isValid())
        ui_eventStart->setDate(event.beginDate());
    else
        ui_eventStart->setDate(QDate::currentDate());

    if(event.endDate().isValid())
        ui_eventEnd->setDate(event.endDate());
    else
        ui_eventEnd->setDate(QDate::currentDate());

    connect(ui_okButton, SIGNAL(clicked()),
            this,        SLOT(onAccepted()));

    connect(ui_eventStartCalendar, SIGNAL(clicked()),
            this, SLOT(onStartCalendar()));

    connect(ui_eventEndCalendar, SIGNAL(clicked()),
            this, SLOT(onEndCalendar()));

    connect(ui_streamAdd, SIGNAL(clicked()),
            this, SLOT(onAddStream()));

    connect(ui_streamRemove, SIGNAL(clicked()),
            this, SLOT(onRemoveStream()));

    connect(ui_linkAdd, SIGNAL(clicked()),
            this, SLOT(onAddLink()));

    connect(ui_linkRemove, SIGNAL(clicked()),
            this, SLOT(onRemoveLink()));
}

Event AddEventDialog::getEvent(bool & accepted, const Event & event)
{
    AddEventDialog dialog(event);
    dialog.exec();
    accepted = (dialog.result() == Accepted);
    return dialog._event;
}

void AddEventDialog::onAccepted()
{
    Event newEvent(ui_eventName->text(), ui_eventStart->date(), ui_eventEnd->date());
    for(int i(0); i < ui_streamList->count(); ++ i)
        newEvent.addStreamUrl(QUrl(ui_streamList->item(i)->text()));

    for(int i(0); i < ui_linkList->count(); ++ i)
    {
        QListWidgetItem * item(ui_linkList->item(i));
        newEvent.addLink(item->data(Qt::UserRole).toUrl(), item->text());
    }
    _event = newEvent;
}

void AddEventDialog::onStartCalendar()
{
    bool accepted;
    QDate date(CalendarDialog::getDate(accepted, ui_eventStart->date()));
    if(accepted)
        ui_eventStart->setDate(date);
}

void AddEventDialog::onEndCalendar()
{
    bool accepted;
    QDate date(CalendarDialog::getDate(accepted, ui_eventEnd->date()));
    if(accepted)
        ui_eventEnd->setDate(date);
}

void AddEventDialog::onAddStream()
{
    bool accepted;
    QString streamUrl(QInputDialog::getText(this, "Add a stream", "Stream Url : ",
                                            QLineEdit::Normal, QString(), &accepted));
    if(accepted)
        ui_streamList->addItem(new QListWidgetItem(streamUrl));
}

void AddEventDialog::onRemoveStream()
{
    foreach(QListWidgetItem * item, ui_streamList->selectedItems())
        ui_streamList->takeItem(ui_streamList->row(item));
}

void AddEventDialog::onAddLink()
{
    bool accepted;
    QPair <QString, QString> link(DoubleStringDialog::getStrings("Link URL : ", "Description", accepted));

    if(accepted)
    {
        QListWidgetItem * item = new QListWidgetItem(link.second);
        item->setData(Qt::UserRole, QUrl(link.first));
        ui_linkList->addItem(item);
    }
}

void AddEventDialog::onRemoveLink()
{
    foreach(QListWidgetItem * item, ui_linkList->selectedItems())
        ui_linkList->takeItem(ui_linkList->row(item));
}

/* ---------------------------------- */

DoubleStringDialog::DoubleStringDialog(const QString & firstLabel, const QString & secondLabel) :
    QDialog()
{
    Ui::UiDoubleStringDialog::setupUi(this);
    ui_l1->setText(firstLabel);
    ui_l2->setText(secondLabel);

    connect(ui_promptValidator, SIGNAL(accepted()),
            this, SLOT(onAccepted()));
}

void DoubleStringDialog::onAccepted()
{
    _firstString = ui_text1->text();
    _secondString = ui_text2->text();
}

QPair <QString, QString> DoubleStringDialog::getStrings(const QString & firstLabel, const QString & secondLabel, bool & accepted)
{
    DoubleStringDialog dialog(firstLabel, secondLabel);
    dialog.exec();
    accepted = (dialog.result() == QDialog::Accepted);
    return QPair <QString, QString>(dialog._firstString, dialog._secondString);
}