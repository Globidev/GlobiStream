#include "ChatBrowser.h"

ChatBrowser::ChatBrowser(QWidget * parent) : QTabWidget(parent)
{
    // Twitch chat popup requires a minimum size
    //setMinimumSize(TWITCH_CHAT_MINIMUM_SIZE);
    setTabsClosable(true);

    QObject::connect(this, SIGNAL(tabCloseRequested(int)),
                     this, SLOT(onTabCloseRequested(int)));
}

ChatBrowser::~ChatBrowser()
{
    // Cleaning open web pages
    foreach(QWebView * page, webPages)
        delete page;
    webPages.clear();
}

void ChatBrowser::openChat(const QString & streamUrl, const QString & streamName)
{
    QString chatUrl(twitchChatFromUrl(streamUrl));
    // Checking if chat is already open
    for(int i(0); i < webPages.size(); ++ i)
    {
        if(webPages.at(i)->url().toString() == chatUrl)
        {
            setCurrentIndex(i);
            return;
        }
    }
    
    // Adding new tab with chat
    QWebView * newPage = new QWebView();
    QObject::connect(newPage, SIGNAL(linkClicked(const QUrl &)),
                     this, SLOT(onLinkClicked(const QUrl &)));
    CookieJar * jar = new CookieJar(_cookies);
    jars << jar;
    newPage->page()->networkAccessManager()->setCookieJar(jar);
    newPage->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    newPage->setUrl(QUrl(chatUrl));
    newPage->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    webPages << newPage;

    addTab(newPage, streamName);
    setCurrentIndex(count() - 1);
}

void ChatBrowser::mouseReleaseEvent(QMouseEvent * event)
{
    // Using Google Chrome's tab closing behavior
    int tabIndexPressed(tabBar()->tabAt(event->pos()));
    if(tabIndexPressed != -1)
        if(event->button() == Qt::MiddleButton)
            onTabCloseRequested(tabIndexPressed);
    QTabWidget::mouseReleaseEvent(event);
}

void ChatBrowser::onTabCloseRequested(int index)
{
    webPages.at(index)->page()->deleteLater();
    webPages.at(index)->deleteLater();
    webPages.removeAt(index);
    _cookies = jars.at(index)->dump();
    jars.removeAt(index);
}

void ChatBrowser::dump()
{
    for(int i(0); i < webPages.size(); ++ i)
        onTabCloseRequested(i);
}