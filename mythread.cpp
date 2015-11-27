#include <QDebug>
#include <QByteArray>
#include "mythread.h"
#include <QSettings>

mythread ::mythread(QObject *parent) :
    QThread(parent)
{

    m_statusReles=0;

    m_bTones << TONE_B3;
    m_bTones << TONE_B2;
    m_bTones << TONE_B1;
    m_bTones << TONE_B0;

    m_bReles << RELE_1;
    m_bReles << RELE_2;
    m_bReles << RELE_3;
    m_bReles << RELE_4;
    m_bReles << RELE_5;
    m_bReles << RELE_6;

    m_mapReles[RELE_1]=0;
    m_mapReles[RELE_2]=1;
    m_mapReles[RELE_3]=2;
    m_mapReles[RELE_4]=3;
    m_mapReles[RELE_5]=4;
    m_mapReles[RELE_6]=5;
}

void mythread::on_timeout()
{
}

void mythread::setAllReles (quint8 reles)
{
    quint8 mask=1;
    for (int n=0;n<m_bReles.size();n++)
    {
        if (mask&reles)
        {
            m_gpio.bitSet(m_bReles[n]);
            m_statusReles|=mask;
        }
        else
        {
            m_gpio.bitClear(m_bReles[n]);
            m_statusReles&=~mask;
        }
        mask <<= 1;
    }
}

void mythread::setVentilador(bool bVentilador)
{
    if (bVentilador)
        setRele (mythread::rele4);
    else
        clearRele (mythread::rele4);
}

void mythread::setAudio(bool bAudio)
{
    if (!bAudio)
        setRele (mythread::rele1);
    else
        clearRele (mythread::rele1);
}

quint8 mythread::getRelesStatus()
{
    return m_statusReles;
}

void mythread::setRele(mythread::reles rel)
{
    quint8 mask=1;
    mask<<=m_mapReles[rel];
    m_gpio.bitSet(rel);
    m_statusReles |= mask;
    emit rele ((quint8)m_mapReles[rel],true);

    QSettings settings("/mnt/config/carta.ini",QSettings::IniFormat);
    settings.setValue("reles/status",m_statusReles);
}

void mythread::clearRele(reles rel)
{
    quint8 mask=1;
    mask<<=m_mapReles[rel];
    m_gpio.bitClear(rel);
    m_statusReles &= ~mask;
    emit rele ((quint8)m_mapReles[rel],false);

    QSettings settings("/mnt/config/carta.ini",QSettings::IniFormat);
    settings.setValue("reles/status",m_statusReles);
}

int mythread::getSample(mythread::adc_channel chan)
{
    quint16 sample=0;

    quint8 control = 0x18 | chan;
    quint8 mask = 0x10;

    m_gpio.bitClear (CS);

    for (int n=0;n<5;n++)
    {
        if (control&mask)
            m_gpio.bitSet(MOSI);
        else
            m_gpio.bitClear(MOSI);
        mask >>=1;

        m_gpio.bitSet   (SCLK);
        usleep(15);
        m_gpio.bitClear (SCLK);
        usleep(15);
    }

    for (int n=0;n<12;n++)
    {
        m_gpio.bitSet   (SCLK);
        usleep(15);
        m_gpio.bitClear (SCLK);
        usleep(15);

        sample= (sample <<1);
        sample|=m_gpio.bitValue(MISO);
    }

    sample >>=1;
    m_gpio.bitSet (CS);


    return sample;
}


quint8 mythread ::getTone ()
{
    quint8 tone=0;

    for (int n=0;n<4;n++)
    {
        tone<<=1;
       if (m_gpio.bitValue((m_bTones[n])))
           tone|=1;
    }
    return tone;
}

void mythread ::run()
{
    bool bSended=false;
    //Ponemos todos los bits de los reles como salidas
    for (int n=0;n<m_bReles.count();n++)
        m_gpio.setOutput(m_bReles[n]);

    QSettings settings("/mnt/config/carta.ini",QSettings::IniFormat);
    setAllReles (settings.value ("reles/status").toInt());

//    for (int n=0;n<m_bReles.count();n++)
//        m_gpio.bitClear(m_bReles[n]);

    //Ponemos MOSI como salidas
    m_gpio.setOutput (MOSI);
    m_gpio.setInput  (MISO);
    m_gpio.setOutput (SCLK);
    m_gpio.setOutput (CS);

    m_gpio.setInput (STROBE);
    m_gpio.setInput(TONE_B3);
    m_gpio.setInput(TONE_B2);
    m_gpio.setInput(TONE_B1);
    m_gpio.setInput(TONE_B0);
    m_gpio.setInput(SIGNAL_IN);

    m_gpio.bitSet (MOSI);
    m_gpio.bitSet (SCLK);
    m_gpio.bitClear (CS);
    msleep(1);
    m_gpio.bitSet (CS);

    setAudio(true);

    float   sample=0;
    float   temp;
    int     msecs=1;

    quint8  antTone=0;

    while (1)
    {
        if (m_gpio.bitValue (STROBE))
        {
            quint8 tTone = getTone();

            if (antTone != tTone)
            {
                emit tone(tTone);
                antTone=tTone;
            }
        }
        else
        {
            antTone=0;
            if (--msecs == 0)
            {
                msecs=1;
                temp=0;
                //Leemos la temperatura
                for (int n=0;n<10;n++)
                {
                    sample = getSample (channel0);
                    temp += (((sample)*165)/819)-55;
                }
                emit temperature(temp/10);

                temp=0;
                for (int n=0;n<10;n++)
                {
                    sample= getSample (channel1);
                    temp += (((sample)*2)/819);
                }
                emit wats(temp/10);
            }
        }

        bool bSignalIn = m_gpio.bitValue(SIGNAL_IN);

        if (bSignalIn && bSended==false)
        {
            emit signalIn (true);
            bSended=true;
        }

        if (!bSignalIn && bSended==true)
        {
            emit signalIn (false);
            bSended=false;
        }
        msleep(100);
    }
}
