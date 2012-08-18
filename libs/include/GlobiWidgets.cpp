#include "GlobiWidgets.h"

GlobiButton::GlobiButton(QWidget * parent) : QPushButton(parent),
	m_r(DEFAULT_R), m_g(DEFAULT_G), m_b(DEFAULT_B), m_upr(true), m_upg(true), m_upb(false),
	m_fSize(DEFAULT_MIN_SIZE), m_upFSize(true)
{
	this->setFlat(true);
	painter = new QPainter();

	startTimer(15);
}

GlobiButton::GlobiButton(const QString & text, QWidget * parent) : QPushButton(parent),
	m_r(DEFAULT_R), m_g(DEFAULT_G), m_b(DEFAULT_B), m_upr(true), m_upg(true), m_upb(false),
	m_fSize(DEFAULT_MIN_SIZE), m_upFSize(true), m_text(text)
{
	this->setFlat(true);
	painter = new QPainter();

	startTimer(15);
}

GlobiButton::~GlobiButton()
{
	delete painter;
}

void GlobiButton::timerEvent(QTimerEvent *)
{
	update();
}

void GlobiButton::paintEvent(QPaintEvent * event)
{
	QPushButton::paintEvent(event);
	painter->begin(this);
	drawGradient();
	drawText();
	painter->end();
	updateColors();
}

void GlobiButton::drawGradient()
{
	QLinearGradient lgrad(this->rect().topLeft(), this->rect().bottomRight());
	lgrad.setColorAt(0, QColor(m_r, m_g, m_b, DEFAULT_A));
	lgrad.setColorAt(1, QColor(255 - m_r, 255 - m_g, 255 - m_b, DEFAULT_A));

	painter->fillRect(this->rect(), QBrush(lgrad));
}

void GlobiButton::drawText()
{
	painter->setPen(QColor(0, 0, 0, 100));
	QFont f("Futura", m_fSize);
	f.setBold(true);
	painter->setFont(f);
	painter->drawText(this->rect(), m_text, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
}

void GlobiButton::updateColors()
{
	if(m_upr) m_r ++; else m_r --;
	if(m_upg) m_g ++; else m_g --;
	if(m_upb) m_b ++; else m_b --;
	if(m_r == 0) m_upr = true; if(m_r == 255) m_upr = false;
	if(m_g == 0) m_upg = true; if(m_g == 255) m_upg = false;
	if(m_b == 0) m_upb = true; if(m_b == 255) m_upb = false;

	if(m_upFSize) m_fSize ++; else m_fSize --;
	if(m_fSize == DEFAULT_MIN_SIZE) m_upFSize = true; if(m_fSize == DEFAULT_MAX_SIZE) m_upFSize = false;
}

void GlobiButton::setText(const QString & text)
{
	m_text = text;
}