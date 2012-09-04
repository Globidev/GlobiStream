#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H


#include "CalendarDialog.h"
#include "Event.h"
#include "ui_AddEventDialogUi.h"
#include "ui_DoubleStringDialogUi.h"

#include <QInputDialog>

class AddEventDialog : public QDialog, public Ui::UiAddEventDialog
{
    Q_OBJECT

    public :
        AddEventDialog(const Event & event);
        ~AddEventDialog() { }

        static Event getEvent(bool & accepted, const Event & event = Event());

    private slots :
        void onAccepted();

        void onStartCalendar();
        void onEndCalendar();
        void onAddStream();
        void onRemoveStream();
        void onAddLink();
        void onRemoveLink(); 

    private :
        Event _event;
};

class DoubleStringDialog : public QDialog, public Ui::UiDoubleStringDialog
{
    Q_OBJECT

    public :
        DoubleStringDialog(const QString & firstLabel, const QString & secondLabel);
        ~DoubleStringDialog() { }

        static QPair <QString, QString> getStrings(const QString & firstLabel, const QString & secondLabel, bool & accepted);

    private slots :
        void onAccepted();

    private :
        QString _firstString;
        QString _secondString;
};

#endif // ADDEVENTDIALOG_H