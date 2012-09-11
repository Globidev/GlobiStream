#include "CustomCalendar.h"

CustomCalendar::CustomCalendar() : QCalendarWidget()
{
    setGridVisible(true);
    setFirstDayOfWeek(Qt::Monday);
}

void CustomCalendar::paintCell(QPainter * painter, const QRect & rect, const QDate & date) const
{
    int eventCount(_colorMap[date].size());

    painter->setPen(Qt::NoPen);
    for(int i(0); i < eventCount; ++ i)
    {
        painter->setBrush(QBrush(_colorMap[date].at(i)));
        QRect region(rect.x() + i * (rect.width() / eventCount), rect.y(),
                     rect.width() / eventCount, rect.height());
        painter->drawRect(region);
    }
    painter->setPen(Qt::SolidLine);
    painter->drawText(rect, QString::number(date.day()), QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
}

void CustomCalendar::setEvents(const QList <Event> & events)
{
    _events = events;
    _colorMap.clear();
    _schedules.clear();

    foreach(const Event & event, _events)
    {
        _colorMap[event.beginDate()].append(event.color());
        _colorMap[event.endDate()].append(event.color());
        _schedules[event.beginDate()].append(QString("[%1] START").arg(event.name()));
        _schedules[event.endDate()].append(QString("[%1] END").arg(event.name()));

        QMapIterator <QDateTime, QString> it(event.schedules());
        while(it.hasNext())
        {
            it.next();
            _colorMap[it.key().date()].append(event.color());

            QString scheduleDescription;
            if(it.key().time().isValid())
                scheduleDescription += QString("%1 - ").arg(it.key().time().toString());
            scheduleDescription += QString("[%1] ").arg(event.name());
            scheduleDescription += it.value();
            _schedules[it.key().date()].append(scheduleDescription);
        }
    }

    QMapIterator <QDate, QStringList> it2(_schedules);
    while(it2.hasNext())
    {
        it2.next();
        QTextCharFormat toolTipFormat;
        toolTipFormat.setToolTip(it2.value().join("\n"));
        setDateTextFormat(it2.key(), toolTipFormat);
    }
    repaint();
}