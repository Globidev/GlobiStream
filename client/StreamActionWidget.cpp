#include "StreamActionWidget.h"

StreamActionWidget::StreamActionWidget(const QStringList & qualities, const QString & url, const QString & streamName) :
QWidget(), m_url(url), m_streamName(streamName)
{
    _init();
    m_qualityBox->addItems(qualities);

    m_qualityBox->setDisabled(qualities.isEmpty());
    m_watchButton->setDisabled(qualities.isEmpty());

    QObject::connect(m_watchButton, SIGNAL(clicked()),
                     this, SLOT(onWatchClicked()));
    QObject::connect(m_chatButton, SIGNAL(clicked()),
                     this, SLOT(onChatClicked()));
}

void StreamActionWidget::_init()
{
    m_qualityBox = new QComboBox();
    m_watchButton = new QPushButton("Watch");
    m_chatButton = new QPushButton("Chat");
    
    m_layout = new QHBoxLayout();
    m_layout->addWidget(m_qualityBox);
    m_layout->addWidget(m_watchButton);
    m_layout->addWidget(m_chatButton);

    setLayout(m_layout);

    if(m_streamName.isEmpty())
        m_streamName = m_url;
}

StreamActionWidget::~StreamActionWidget()
{
    delete m_layout;
    delete m_qualityBox;
    delete m_watchButton;
    delete m_chatButton;
}