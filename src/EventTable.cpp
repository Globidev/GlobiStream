#include "EventTable.h"

EventTable::EventTable(QWidget * parent) : QTableWidget(parent)
{
    setColumnCount(EVENT_COLUMN_COUNT);
    setHorizontalHeaderLabels(EVENT_HEADER_LABELS);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    verticalHeader()->setMinimumSectionSize(MINIMUM_TABLE_SECTION_SIZE);
    verticalHeader()->setDefaultSectionSize(MINIMUM_TABLE_SECTION_SIZE);
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    setSelectionMode(QAbstractItemView::NoSelection);

    setMouseTracking(true);
}

void EventTable::buildTable(const QList <Event> & events)
{
    setRowCount(events.size());

    for(int i(0); i < events.size(); ++ i)
    {
        const Event & event = events.at(i);
        QTableWidgetItem * eventName = new QTableWidgetItem(event.name());
        QTableWidgetItem * dateName = new QTableWidgetItem(dateString.arg(event.beginDate().toString(), event.endDate().toString()));

        UrlLabel * streamLabel = new UrlLabel();
        UrlLabel * linkLabel = new UrlLabel();
        

        foreach(const QUrl & url, event.streamsUrls())
        {
            streamLabel->setText(QString("%1 <p>%2").arg(streamLabel->text(), 
                                                       URL_RICH_TEXT_TEMPLATE.arg(url.toString(), url.toString())));
        }
        
        QMapIterator<QUrl, QString> it(event.links());
        while(it.hasNext())
        {
            it.next();
            linkLabel->setText(QString("%1 <p>%2").arg(linkLabel->text(),
                                                           URL_RICH_TEXT_TEMPLATE.arg(it.key().toString(), it.value())));
        }

        setItem(i, 0, eventName);
        setItem(i, 1, dateName);
        setCellWidget(i, 2, streamLabel);
         
        setCellWidget(i, 3, linkLabel);
    }
}

void EventTable::mouseMoveEvent(QMouseEvent * event)
{
    QTableWidgetItem * item(itemAt(event->pos()));
    if(item)
    {
        if(item->column() == 1)
        {
            QDate b, e;
            QString rangeString(item->text());
            rangeString.remove("From \n");
            rangeString.replace(" \nto \n",";");
            QStringList dateString(rangeString.split(";"));
            qDebug() << dateString;

            b = QDate::fromString(dateString.at(0));
            e = QDate::fromString(dateString.at(1));
            qDebug() << b << e;

            w.setDateRange(b, e);
            QDate it(b);
            QTextCharFormat f;
            f.setBackground(QBrush(QColor("aqua")));
            while(it < e)
            {
                w.setDateTextFormat(it, f);
                it = it.addDays(1);
            }
            w.move(QApplication::overrideCursor()->pos());
            w.show();
            return;
        }
    }
     w.hide();
}