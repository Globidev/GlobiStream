#include "CalendarDialog.h"

CalendarDialog::CalendarDialog(const QDate & date) : QDialog(), _date(date)
{
    Ui::UiCalendarDialog::setupUi(this);
    if(date >= ui_calendar->minimumDate() && date <= ui_calendar->maximumDate())
        ui_calendar->setSelectedDate(date);
    ui_calendar->setSelectedDate(date);
    connect(ui_promptValidator, SIGNAL(accepted()),
            this, SLOT(onAccepted()));
}

void CalendarDialog::onAccepted()
{
    _date = ui_calendar->selectedDate();
}

QDate CalendarDialog::getDate(bool & accepted, const QDate & date)
{
    CalendarDialog dialog(date);
    dialog.exec();
    accepted = (dialog.result() == QDialog::Accepted);
    return dialog._date;
}