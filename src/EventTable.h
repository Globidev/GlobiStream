#ifndef EVENTTABLE_H
#define EVENTTABLE_H

#include <QCalendarWidget>
#include <QApplication>
#include <QTextCharFormat>
#include <QMouseEvent>
#include "StreamTable.h"
#include "Event.h"

static const int EVENT_COLUMN_COUNT(4);
static const QStringList EVENT_HEADER_LABELS(QStringList() << "Event Name" << "Period" << "Streams" << "Links");
static const QString dateString("From \n%1 \nto \n%2");

class EventTable : public QTableWidget
{
    Q_OBJECT

    public :
        EventTable(QWidget * parent = 0);
        ~EventTable() { }

        void buildTable(const QList <Event> & events);
        void mouseMoveEvent(QMouseEvent * event);

    private :
        QCalendarWidget w;

};

#endif // EVENTTABLE_H