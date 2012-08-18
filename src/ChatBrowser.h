#ifndef CHATBROWSER_H
#define CHATBROWSER_H

#include <QTabWidget>
#include <QWebView>
#include <QNetworkCookieJar>
#include <QTabBar>
#include <QWebFrame>
#include <QMouseEvent>
#include <QDebug>
#include "Global.h"

class CookieJar : public QNetworkCookieJar
{
    public :
        CookieJar(const QList <QNetworkCookie> & cookies) : QNetworkCookieJar() { setAllCookies(cookies); }
        virtual ~CookieJar() { }

        QList <QNetworkCookie> dump() const { return allCookies(); }
};

class ChatBrowser : public QTabWidget
{
    Q_OBJECT

    public :
        ChatBrowser(QWidget * parent = 0);
        ~ChatBrowser();

        void mouseReleaseEvent(QMouseEvent * event);
        void dump();

        QList <QNetworkCookie> cookies() { return _cookies; }
        void setCookies(const QList <QNetworkCookie> & cookies) { _cookies = cookies; }

    private slots :
        void onTabCloseRequested(int index);
        void openChat(const QString & streamUrl, const QString & streamName);

    private :
        QList <QWebView *> webPages;
        QList <CookieJar *> jars;
        QList <QNetworkCookie> _cookies;
};

#endif // CHATBROWSER_H