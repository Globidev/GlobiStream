#ifndef CUSTOMCALENDAR_H
#define CUSTOMCALENDAR_H

#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QPainter>

#include "Event.h"

class CustomCalendar : public QCalendarWidget
{
    Q_OBJECT

    public :
        CustomCalendar();
        ~CustomCalendar() { }

        void setEvents(const QList <Event> & events);

    protected :
        void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;

    private :
        QList <Event> _events;
        QMap <QDate, QList <QColor> > _colorMap;
        QMap <QDate, QStringList> _schedules;

};

#endif // CUSTOMCALENDAR_H