#ifndef CHATACTIONWIDGET_H
#define CHATACTIONWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>

#include "Global.h"

class StreamActionWidget : public QWidget
{
     Q_OBJECT

    public :
        StreamActionWidget(const QStringList & qualities, const QString & url, const QString & streamName = QString());
        ~StreamActionWidget();
        void _init();

        QString quality() const { return m_qualityBox->currentText(); }
        QString url() const { return m_url; }

    private slots :
        void onWatchClicked() { emit watchClicked(m_url, quality()); }
        void onChatClicked() { emit chatClicked(m_url, m_streamName); }

    signals :
        void watchClicked(const QString &, const QString &);
        void chatClicked(const QString &, const QString &);

    private :
        QComboBox * m_qualityBox;
        QPushButton * m_watchButton;
        QPushButton * m_chatButton;
        QHBoxLayout * m_layout;
        QString m_url;
        QString m_streamName;
};

#endif // CHATACTIONWIDGET_H