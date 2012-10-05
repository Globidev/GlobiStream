#ifndef EVENTTABLE_H
#define EVENTTABLE_H

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

    signals :
        void changeEvent(const Event & event, int eventIndex);

    private slots :
        void onEventEditRequested();

    private :
        QList <Event> _events;
};

#endif // EVENTTABLE_H