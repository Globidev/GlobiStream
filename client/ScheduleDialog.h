#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H

#include "ui_ScheduleDialogUi.h"

typedef QPair <QDateTime, QString> Schedule;

class ScheduleDialog : public QDialog, public Ui::UiScheduleDialog
{
    Q_OBJECT

    public :
        ScheduleDialog();
        ~ScheduleDialog() { }

        static Schedule getSchedule(bool & accepted);

    private slots :
        void onAccepted();

    private :
        Schedule _schedule;

};

#endif // SCHEDULEDIALOG_H