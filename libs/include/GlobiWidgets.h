#ifndef GLOBIWIDGETS_H
#define GLOBIWIDGETS_H

#include "globilibs_global.h"

#include <QPushButton>
#include <QPainter>

#define DEFAULT_R 0
#define DEFAULT_G 127
#define DEFAULT_B 255
#define DEFAULT_A 50
#define DEFAULT_MIN_SIZE 15
#define DEFAULT_MAX_SIZE 50

class GLOBILIBS_EXPORT GlobiButton : public QPushButton
{
	Q_OBJECT

	public :
		GlobiButton(QWidget * parent = 0);
		GlobiButton(const QString & text, QWidget * parent = 0);
		~GlobiButton();

		void timerEvent(QTimerEvent *);
		void paintEvent(QPaintEvent *);
		void drawGradient();
		void drawText();
		void updateColors();

		void setText(const QString & text);
		QString text() const { return m_text; }

	private :
		QPainter * painter;
		QString m_text;

		int m_r, m_g, m_b, m_fSize;
		bool m_upr, m_upg, m_upb, m_upFSize;
};

#endif