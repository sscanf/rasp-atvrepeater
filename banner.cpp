#include <QPainter>
#include <QDebug>
#include "banner.h"

banner::banner(QWidget *parent) :
    QLabel(parent)
{
    m_px = 0;
    m_py = 15;
    m_speed = 2;
    m_direction = RightToLeft;
    connect(&timer, SIGNAL(timeout()), this, SLOT(refreshLabel()));
    timer.start(10);
}

void banner::pause (bool bPause)
{
    if (bPause)
        timer.stop();
    else
        timer.start(10);
}

void banner::show()
{
    adjustSize();
    if (width()<711)
        this->resize(711, this->height());
    //m_textLength = fontMetrics().width(text());
    m_px = 0;//width();
    QLabel::show();
}

void banner::setAlignment(Qt::Alignment al)
{
    m_align = al;
    updateCoordinates();
    QLabel::setAlignment(al);
}
void banner::refreshLabel()
{
    repaint();
}

void banner::setSpeed(int s)
{
    m_speed = s;
}

int banner::getSpeed()
{
    return m_speed;
}

void banner::setDirection(int d)
{
    m_direction = d;
    if (m_direction==RightToLeft)
        m_px = width() - m_textLength;
    else
        m_px = 0;
    refreshLabel();
}

void banner::updateCoordinates()
{
    switch(m_align)
    {
        case Qt::AlignTop:
            m_py = 10;
            break;
        case Qt::AlignBottom:
            m_py = height()-10;
            break;
        case Qt::AlignVCenter:
            m_py = height()/2;
            break;
    }
    m_fontPointSize = font().pointSize()/2;
    m_textLength = fontMetrics().width(text());
}


void banner::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    if(m_direction==RightToLeft)
    {
        m_px -= m_speed;
        //qDebug() << m_px << m_textLength;
        if(m_px <= (-m_textLength))
            m_px = width();
    }
    else
    {
        m_px += m_speed;
        if(m_px >= width())
            m_px = - m_textLength;
    }

    p.drawText(m_px, m_py+m_fontPointSize, text());
    p.drawText(m_px-width(), m_py+m_fontPointSize, text());
    p.drawText(m_px+width(), m_py+m_fontPointSize, text());
    p.translate(m_px,0);
}
