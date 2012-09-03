#ifndef EVENT_H
#define EVENT_H

#include <QDateTime>
#include <QUrl>
#include <QMap>
#include <QDomDocument>
#include <QDebug>

static const QString XML_TAG_EVENTS("events");
static const QString XML_TAG_EVENT("event");
static const QString XML_TAG_LINKS("links");
static const QString XML_TAG_LINK("link");
static const QString XML_TAG_URLS("urls");
static const QString XML_TAG_URL("url");
static const QString XML_TAG_SCHEDULES("schedules");
static const QString XML_TAG_SCHEDULE("schedule");

static const QString XML_EVENT_ATTR_NAME("name");
static const QString XML_EVENT_ATTR_BEGIN_DATE("begin");
static const QString XML_EVENT_ATTR_END_DATE("end");
static const QString XML_ATTR_DESCRIPTION("description");

class Event
{
    public :
        Event() { }
        Event(const QString & name, 
              const QDate & beginDate,
              const QDate & endDate);
        ~Event() { }
        Event(const Event & other);
        Event & operator=(const Event & other);
        void copy(const Event & other);

        void addStreamUrl(const QUrl & streamUrl) { _streamsUrls << streamUrl; }
        void addSchedule(const QDateTime & schedule, 
                         const QString & description) { _schedules.insert(schedule, description); }
        void addLink(const QUrl & link, 
                     const QString & description) { _links.insert(link, description); }

        QDomElement toXmlElement() const;
        static Event fromXmlElement(const QDomElement & eventElement);

        QString name() const { return _name; }
        QDate beginDate() const { return _beginDate; }
        QDate endDate() const { return _endDate; }
        QMap <QUrl, QString> links() const { return _links; }
        QList <QUrl> streamsUrls() const { return _streamsUrls; }
        QMap <QDateTime, QString> schedules() const { return _schedules; }     

    private :
        QString _name;
        QDate _beginDate, _endDate;
        QMap <QUrl, QString> _links;
        QList <QUrl> _streamsUrls; 
        QMap <QDateTime, QString> _schedules;
};

Q_DECLARE_METATYPE(Event);

QDataStream & operator<<(QDataStream & out, const Event & event);
QDataStream & operator>>(QDataStream & in, Event & event);

#endif // EVENT_H