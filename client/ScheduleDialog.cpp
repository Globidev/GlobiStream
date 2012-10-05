#include "ScheduleDialog.h"

ScheduleDialog::ScheduleDialog() : QDialog()
{
    Ui::UiScheduleDialog::setupUi(this);
    connect(ui_promptValidator, SIGNAL(accepted()),
            this, SLOT(onAccepted()));

    ui_time->setTime(QTime::currentTime());
}

Schedule ScheduleDialog::getSchedule(bool & accepted)
{
    ScheduleDialog dialog;
    dialog.exec();
    accepted = (dialog.result() == QDialog::Accepted);
    return dialog._schedule;
}

void ScheduleDialog::onAccepted()
{
    QDateTime scheduleDate;
    scheduleDate.setDate(ui_calendar->selectedDate());
    if(ui_detailedSchedule->isChecked())
        scheduleDate.setTime(ui_time->time());
    _schedule.first = scheduleDate;
    _schedule.second = ui_name->text();
}