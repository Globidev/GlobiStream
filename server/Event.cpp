#include "Event.h"

Event::Event(const QString & name, const QDate & beginDate, const QDate & endDate) :
    _name(name), _beginDate(beginDate), _endDate(endDate), _isNull(false)
{

}

Event::Event(const Event & other)
{
    copy(other);
}

Event & Event::operator=(const Event & other)
{
    if(this != &other)
        copy(other);
    return *this;
}

void Event::copy(const Event & other)
{
    _name = other._name;
    _beginDate = other._beginDate;
    _endDate = other._endDate;
    _links = other._links;
    _streamsUrls = other._streamsUrls;
    _schedules = other._schedules;
    _color = other._color;
    _isNull = other._isNull;
}

QDomElement Event::toXmlElement() const
{
    QDomDocument document;

    QDomElement eventElement(document.createElement(XML_TAG_EVENT));
    eventElement.setAttribute(XML_EVENT_ATTR_NAME, _name);
    eventElement.setAttribute(XML_EVENT_ATTR_BEGIN_DATE, _beginDate.toString());
    eventElement.setAttribute(XML_EVENT_ATTR_END_DATE, _endDate.toString());
    eventElement.setAttribute(XML_EVENT_ATTR_COLOR, _color.name());

    // Links
    QDomElement linksElement(document.createElement(XML_TAG_LINKS));
    QMapIterator<QUrl, QString> it(_links);
    while(it.hasNext())
    {
        it.next();
        QDomElement linkElement(document.createElement(XML_TAG_LINK));
        linkElement.setAttribute(XML_ATTR_DESCRIPTION, it.value());
        linkElement.appendChild(document.createTextNode(it.key().toString()));
        linksElement.appendChild(linkElement);
    }
    eventElement.appendChild(linksElement);

    // Stream Urls
    QDomElement urlsElement(document.createElement(XML_TAG_URLS));
    foreach(const QUrl & url, _streamsUrls)
    {
        QDomElement urlElement(document.createElement(XML_TAG_URL));
        urlElement.appendChild(document.createTextNode(url.toString()));
        urlsElement.appendChild(urlElement);
    }
    eventElement.appendChild(urlsElement);

    // Schedules
    QDomElement schedulesElement(document.createElement(XML_TAG_SCHEDULES));
    QMapIterator <QDateTime, QString> it2(_schedules);
    while(it2.hasNext())
    {
        it2.next();
        QDomElement scheduleElement(document.createElement(XML_TAG_SCHEDULE));
        scheduleElement.setAttribute(XML_ATTR_DESCRIPTION, it2.value());
        scheduleElement.appendChild(document.createTextNode(it2.key().toString()));
        schedulesElement.appendChild(scheduleElement);
    }
    eventElement.appendChild(schedulesElement);

    return eventElement;
}

Event Event::fromXmlElement(const QDomElement & eventElement)
{
    QString name(eventElement.attribute(XML_EVENT_ATTR_NAME));
    QDate beginDate(QDate::fromString(eventElement.attribute(XML_EVENT_ATTR_BEGIN_DATE)));
    QDate endDate(QDate::fromString(eventElement.attribute(XML_EVENT_ATTR_END_DATE)));
    QColor color(QColor(eventElement.attribute(XML_EVENT_ATTR_COLOR)));

    Event event(name, beginDate, endDate);
    event.setColor(color);

    // Links
    QDomElement linksElement(eventElement.firstChildElement(XML_TAG_LINKS));
    for(QDomElement linkElement(linksElement.firstChildElement()); !linkElement.isNull(); linkElement = linkElement.nextSiblingElement())
        event.addLink(QUrl(linkElement.text()), linkElement.attribute(XML_ATTR_DESCRIPTION));

    // Stream Urls
    QDomElement urlsElement(eventElement.firstChildElement(XML_TAG_URLS));
    for(QDomElement urlElement(urlsElement.firstChildElement()); !urlElement.isNull(); urlElement = urlElement.nextSiblingElement())
        event.addStreamUrl(QUrl(urlElement.text()));

    // Schedules
    QDomElement schedulesElement(eventElement.firstChildElement(XML_TAG_SCHEDULES));
    for(QDomElement scheduleElement(schedulesElement.firstChildElement()); !scheduleElement.isNull(); scheduleElement = scheduleElement.nextSiblingElement())
        event.addSchedule(QDateTime::fromString(scheduleElement.text()), scheduleElement.attribute(XML_ATTR_DESCRIPTION));

    return event;
}

QDataStream & operator<<(QDataStream & out, const Event & event)
{
    out << event.name() << event.beginDate() << event.endDate()
        << event.links() << event.streamsUrls() << event.schedules()
        << event.color();
    return out;
}

QDataStream & operator>>(QDataStream & in, Event & event)
{
    QString name;
    QDate beginDate, endDate;
    QMap <QUrl, QString> links;
    QList <QUrl> streamsUrls; 
    QMap <QDateTime, QString> schedules;
    QColor color;

    in >> name >> beginDate >> endDate >> links >> streamsUrls >> schedules >> color;

    Event e(name, beginDate, endDate);
    e.setColor(color);
    QMapIterator <QUrl, QString> it(links);
    while(it.hasNext())
    {
        it.next();
        e.addLink(it.key(), it.value());
    }

    foreach(const QUrl & url, streamsUrls)
        e.addStreamUrl(url);

    QMapIterator <QDateTime, QString> it2(schedules);
    while(it2.hasNext())
    {
        it2.next();
        e.addSchedule(it2.key(), it2.value());
    }
    event = e;

    return in;
}