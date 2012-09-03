#ifndef EVENTTABLE_H
#define EVENTTABLE_H

#include "StreamTable.h"
#include "Event.h"
#include <QScrollBar>

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

    private :


};

#endif // EVENTTABLE_H