#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H

#include <QCalendarWidget>
#include <QDialog>
#include "ui_CalendarDialogUi.h"

class CalendarDialog : public QDialog, public Ui::UiCalendarDialog
{
    Q_OBJECT

    public :
        CalendarDialog(const QDate & date);
        ~CalendarDialog() { }

        static QDate getDate(bool & accepted, const QDate & date = QDate());

    private slots :
        void onAccepted();

    private :
        QDate _date;
};

#endif // CALENDARDIALOG_H