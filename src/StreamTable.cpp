#include "StreamTable.h"

UrlLabel::UrlLabel(const QString & text): QLabel(URL_RICH_TEXT_TEMPLATE.arg(text))
{
    QObject::connect(this, SIGNAL(linkActivated(const QString &)),
                     this, SLOT(onLinkActivated(const QString &)));
}

void UrlLabel::onLinkActivated(const QString & url)
{
    QDesktopServices::openUrl(QUrl(url));
}

StreamTable::StreamTable(QWidget * parent) : QTableWidget(parent), showOfflinePages(false), showNonMonitoredPaged(false)
{
    setColumnCount(COLUMN_COUNT);
    setHorizontalHeaderLabels(TABLE_HEADER_LABELS);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    verticalHeader()->setMinimumSectionSize(MINIMUM_TABLE_SECTION_SIZE);
    verticalHeader()->setDefaultSectionSize(MINIMUM_TABLE_SECTION_SIZE);
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void StreamTable::buildTable(const StreamList & streams)
{
    streamsCache = streams;
    int rowIterator(0);
    StreamList shown;

    foreach(const Stream & stream, streams)
    {
        switch(stream.online)
        {
            case Online :
                shown << stream;
                break;

            case Offline :
                if(showOfflinePages) shown << stream;
                break;

            case UnWatched :
                if(showNonMonitoredPaged) shown << stream;
                break;

            default :
                break;
        }
        _lastStates.insert(stream.url, stream.online);
    }

    setRowCount(shown.size());

    foreach(const Stream & stream, shown)
    {
        // Creating row items
        QTableWidgetItem * streamNameItem = new QTableWidgetItem(stream.name);

        UrlLabel * urlLabel = new UrlLabel(stream.url);
        QString state; QColor color;
        switch(stream.online)
        {
            case Online :
                state = ONLINE_STATE;
                color = ONLINE_COLOR;
                break;

            case Offline :
                state = OFFLINE_STATE;
                color = OFFLINE_COLOR;
                break;

            case UnWatched :
                state = UNWATCHED_STATE;
                color = UNWATCHED_COLOR;
                break;

            default :
                break;
        }
        QTableWidgetItem * statusItem = new QTableWidgetItem(state);
        statusItem->setBackgroundColor(color);

        StreamActionWidget * actionWidget = new StreamActionWidget(stream.qualities, stream.url, stream.name);
        QObject::connect(actionWidget, SIGNAL(watchClicked(const QString &, const QString &)),
                            this, SLOT(onWatchClicked(const QString &, const QString &)));
        QObject::connect(actionWidget, SIGNAL(chatClicked(const QString &, const QString &)),
                            this, SLOT(onChatClicked(const QString &, const QString &)));

        // Placing items
        setItem         (rowIterator, 0, streamNameItem);
        setCellWidget   (rowIterator, 1, urlLabel);
        setItem         (rowIterator, 2, statusItem);
        setCellWidget   (rowIterator, 3, actionWidget);
        rowIterator ++;
    }
}