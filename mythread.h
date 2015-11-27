#ifndef THREAD_H
#define THREAD_H

#include <QByteArray>
#include <QThread>
#include <QTimer>
#include <QMap>
#include "gpio.h"

#define RELE_1      17
#define RELE_2      18
#define RELE_3      27
#define RELE_4      22
#define RELE_5      23
#define RELE_6      24
#define RELE_7      25
#define MOSI        10
#define MISO        9
#define SCLK        11
#define CS          7
#define STROBE      2
#define TONE_B3     14
#define TONE_B2     15
#define TONE_B1     4
#define TONE_B0     3
#define SIGNAL_IN   8

class mythread : public QThread
{
    Q_OBJECT

    enum adc_channel
    {
        channel0,
        channel1
    };

public:

    enum reles
    {
        rele1 = RELE_1,
        rele2 = RELE_2,
        rele3 = RELE_3,
        rele4 = RELE_4,
        rele5 = RELE_5,
        rele6 = RELE_6,
        rele7 = RELE_7,
    };

    explicit mythread     (QObject *parent = 0);
    void    run           ();
    void    setAudio      (bool bAudio);
    void    setVentilador (bool bVentilador);
    void    setRele       (mythread::reles rel);
    void    clearRele     (mythread::reles rel);
    quint8  getRelesStatus();
    void    setAllReles   (quint8 reles);

private:
    int     getSample   (mythread::adc_channel chan);
    quint8  getTone     ();

private:
    QGpio           m_gpio;
    QTimer         *m_pTimer;
    quint16         m_word;
    QList<quint8>   m_bTones;
    QList <int>     m_bReles;
    QMap <int, int> m_mapReles;
    quint8          m_statusReles;

signals:
    void temperature (float);
    void tone        (quint8);
    void wats        (float);
    void rele        (quint8, bool);
    void signalIn    (bool);

private slots:
    void on_timeout();

};

#endif // THREAD_H
