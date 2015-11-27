#ifndef BANNER_H
#define BANNER_H

#include <QLabel>
#include <QTimer>

class banner : public QLabel
{
    Q_OBJECT
public:
    explicit banner(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void show();
    void setAlignment(Qt::Alignment al);
    void updateCoordinates();
    void setSpeed(int s);
    int  getSpeed();
    void setDirection(int d);
    void pause(bool bPause);

private:
        enum Direction{LeftToRight,RightToLeft};
        int m_px;
        int m_py;
        QTimer timer;
        Qt::Alignment m_align;
        int m_speed;
        int m_direction;
        int m_fontPointSize;
        int m_textLength;

signals:

private slots:
    void refreshLabel();
};

#endif // BANNER_H
