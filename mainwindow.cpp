//JN01XG
//EA3UM

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QFont>
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QByteArray>

#include <banner.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    bool  enabled;
    QString text;
    QString color;

    int speed;
    int direction;

    m_bReles << RELE_1;
    m_bReles << RELE_2;
    m_bReles << RELE_3;
    m_bReles << RELE_4;
    m_bReles << RELE_5;
    m_bReles << RELE_6;

    m_currentStatus = MainWindow::iddle;
    m_bAutomatico=true;

    QSettings settings("/mnt/config/carta.ini",QSettings::IniFormat);

    enabled     = settings.value ("banner/enabled",1).toBool();
    text        = settings.value ("banner/text","Ea3um carta ATV").toString();
    color       = settings.value ("banner/color","white").toString();
    speed       = settings.value ("banner/speed",2).toInt();
    direction   = settings.value ("banner/direction",0).toInt();
    m_passwd    = settings.value ("security/passwd","0000").toString();
    m_disparo   = settings.value ("ventilador/disparo","35").toFloat();
    m_histeresis= settings.value ("ventilador/histeresis","2").toFloat();

    ui->setupUi (this);
    m_pTimer=new QTimer (this);
    connect (m_pTimer,SIGNAL (timeout()),this,SLOT(on_timeout()));
    m_pTimer->start (5000);

    m_pTimerCode = new QTimer(this);
    connect (m_pTimerCode, SIGNAL(timeout()),this,SLOT (on_timercode()));
    if (enabled==true)
    {
        //ui->mybanner->setText (text);
        m_strBannerText=text;
        ui->mybanner->setSpeed(speed);
        ui->mybanner->setDirection(direction);
        ui->mybanner->show();

        QString style = QString ("QLabel { background:none; color : %1; }").arg(color);
        ui->mybanner->setStyleSheet(style);
    }

    m_pToneIn = new tonein(this);
    m_pToneIn->hide();

    m_pTimerSrvce = new QTimer ();
    connect (m_pTimerSrvce,SIGNAL (timeout()),this,SLOT (on_srvce()));

    m_pThread = new mythread (this);
    connect (m_pThread,SIGNAL (tone(quint8)),       this,SLOT   (on_tone        (quint8)));
    connect (m_pThread,SIGNAL (temperature(float)), this,SLOT   (on_temperature (float)));
    connect (m_pThread,SIGNAL (wats(float)),        this,SLOT   (on_wats        (float)));
    connect (m_pThread,SIGNAL (rele(quint8,bool)),  this,SLOT   (on_rele (quint8, bool)));
    connect (m_pThread,SIGNAL (signalIn(bool)),     this,SLOT   (on_signalIn      (bool)));

    m_pThread->start();//(QThread::HighestPriority);
    setBannerText();

}


void MainWindow::setBannerText()
{
    QString strTemp;
    QString strWats;
    QString strReles;
    QString strDisparo;
    QString strHisteresis;
    QString bannerText;

    strTemp.sprintf ("%02.1f",m_temperatura);
    strWats.sprintf ("%02.1f",m_wats);
    strDisparo.sprintf ("%02.1f",m_disparo);
    strHisteresis.sprintf ("%02.1f",m_histeresis);

    quint8 stReles = m_pThread->getRelesStatus();
    quint8 mask=1;

    for (int n=0;n<m_bReles.size()+1;n++)
    {
        if (mask & stReles)
            strReles.append(QString (" %1").arg(n+1));

        mask <<=1;
    }

    bannerText = m_strBannerText;
    bannerText.replace ("%temp%",strTemp);
    bannerText.replace ("%wats%",strWats);
    bannerText.replace ("%disparo%",strDisparo);
    bannerText.replace ("%histeresis%",strHisteresis);
    bannerText.replace ("%reles%",strReles);

    ui->mybanner->setText (bannerText);
    ui->mybanner->adjustSize();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timeout()
{
    m_pTimer->stop();
    ui->label->hide();
}


void MainWindow::on_signalIn(bool bSignal)
{
    if (m_pDialogStatus)
    {
        m_pDialogStatus->setSignalIn(bSignal);
    }
}

void MainWindow::on_rele (quint8 rel, bool bStatus)
{
    if (m_pDialogStatus)
        m_pDialogStatus->setRele (rel,bStatus);
}

void MainWindow::on_tone (quint8 tone)
{
    QString secret;
    m_pTimerCode->start(3000);
    m_pToneIn->show();

    switch (m_currentStatus)
    {
        case MainWindow::iddle:
            m_code.append ((tone%0x0a)+'0');
            qDebug() << m_code;
            secret.fill('*',m_code.length());
            m_pToneIn->setNumber(secret);

            if (m_code.size()==4)
            {
                QString passwd(m_code);

                if (passwd==m_passwd)
                {
                    m_pTimer->stop();
                    ui->label->setText ("SVCE");
                    ui->label->show();
                    m_pTimerSrvce->start (TIMEOUT_SERVICE);
                    m_currentStatus = MainWindow::service;
                    m_pThread->setAudio (false);
                    m_code.clear();
                    m_pToneIn->setNumber("");
                }
                on_timercode();
            }

        break;

        case MainWindow::service:
        {
            if (m_pTimerSrvce->isActive())
                m_pTimerSrvce->start (TIMEOUT_SERVICE);

            m_code.append (tone);

            QString strNumber;
            for (int n=0;n<m_code.size();n++)
            {
                quint8 ch = m_code[n];
                if (ch==0x0b)
                    ch='*';
                else if (ch==0x0c)
                    ch='#';
                else
                    ch= (ch%0x0a)+ '0';

                strNumber.append(ch);
            }
            m_pToneIn->setNumber (strNumber);

            if (m_code.size() == 2)
            {
                if (m_code.at(0)==TECLA_ALMOHADILLA)
                {
                    if (m_code.at(1)==1)    //Bloqueado
                    {
                        m_pDialogStatus->setLock(true);
                        m_pTimerSrvce->stop();
                    }

                    if (m_code.at(1)==0x0a) //Desbloqueado
                    {
                        m_pDialogStatus->setLock(false);
                        m_pTimerSrvce->start (TIMEOUT_SERVICE);
                    }

                    if (m_code.at(1)==0x0c) //Automatico / Manual
                    {

                        if (m_bAutomatico)
                        {
                            m_pDialogStatus->setManual(true);
                            m_bAutomatico=false;
                        }
                        else
                        {
                            m_pDialogStatus->setManual(false);
                            m_bAutomatico=true;
                        }
                    }
                }

                if (m_code.at(0)==TECLA_ASTERISCO)
                {
                    if (m_code.at(1)==1)
                    {
                        if (!m_pDialogStatus)
                        {
                            showStatusDlg(true);
                        }
                    }

                    if (m_code.at(1)==0x0a)
                    {
                        showStatusDlg(false);
                    }
                }
                else
                {
                    if (m_code.at(0)<7)
                    {
                        if (m_code.at(1)==1)
                        {
                            m_pThread->setRele ((mythread::reles)m_bReles[m_code.at(0)-1]);
                        }

                        if (m_code.at(1)==0x0a)
                        {
                            m_pThread->clearRele((mythread::reles)m_bReles[m_code.at(0)-1]);
                        }
                    }
                }


                if (m_code.at(0)==0x9 && m_code.at(1)==0x9)
                {
                    showStatusDlg(false);
                    showChangePasswd(true);

                    qDebug() << "Cambio password";
                    m_currentStatus = MainWindow::change_passwd_1;
                    m_pChangePasswd->setTitle ("Entre nueva clave:");
                    m_pTimerSrvce->stop();
                }
                m_code.clear();
            }
        }
        break;

        case MainWindow::change_passwd_1:
        {
            if (tone==TECLA_ALMOHADILLA)
            {
                m_pChangePasswd->setTitle("Vuelva a entrar clave:");
                m_passwd1 = m_pChangePasswd->getPasswd();
                m_currentStatus = MainWindow::change_passwd_2;
                m_code.clear();
                m_pChangePasswd->passwd("");

            }else if (tone==TECLA_ASTERISCO)
            {
                showChangePasswd(false);
                m_currentStatus=MainWindow::service;
                m_pTimerSrvce->start (TIMEOUT_SERVICE);
                m_code.clear();
            }
            else
            {
                if (m_code.size()==4)
                    m_code.clear();

                m_code.append (tone);

                QString strNumber;
                for (int n=0;n<m_code.size();n++)
                {
                    quint8 ch = m_code[n];
                    if (ch==0x0b)
                        ch='*';
                    else if (ch==0x0c)
                        ch='#';
                    else
                        ch= (ch%0x0a)+ '0';

                    strNumber.append(ch);
                }
                m_pChangePasswd->passwd(strNumber);
            }
        }
        break;

        case MainWindow::change_passwd_2:
        {
            if (tone==TECLA_ALMOHADILLA)
            {
                m_passwd2=m_pChangePasswd->getPasswd();

                if (m_passwd1 == m_passwd2)
                {
                    m_code.clear();
                    m_pChangePasswd->setTitle("Clave cambiada OK");
                    m_pChangePasswd->passwd("Pulse *",false);

                    QSettings settings("/mnt/config/carta.ini",QSettings::IniFormat);
                    settings.setValue("security/passwd",m_passwd2);
                    m_passwd=m_passwd2;
                }
                else
                {
                    m_code.clear();
                    m_pChangePasswd->setTitle("Las claves no coinciden");
                    m_pChangePasswd->passwd("Pulse *",false);
                }
            }else if (tone==TECLA_ASTERISCO)
            {
                showChangePasswd(false);
                m_currentStatus=MainWindow::service;
                m_pTimerSrvce->start (TIMEOUT_SERVICE);
                m_code.clear();
            }
            else
            {
                if (m_code.size()==4)
                    m_code.clear();

                m_code.append (tone);

                QString strNumber;
                for (int n=0;n<m_code.size();n++)
                {
                    quint8 ch = m_code[n];
                    if (ch==0x0b)
                        ch='*';
                    else if (ch==0x0c)
                        ch='#';
                    else
                        ch= (ch%0x0a)+ '0';

                    strNumber.append(ch);
                }
                m_pChangePasswd->passwd(strNumber);
            }
        }
        break;
    }
}


void MainWindow::showChangePasswd(bool bShow)
{
    if (bShow)
    {
        m_pChangePasswd = new changePasswd (this);
        m_pChangePasswd ->setStyleSheet("background-image: none;  "\
                                       "background: rgb(180,180,180);" \
                                       "color: black;");
        m_pChangePasswd->show();
    }
    else
    {
       m_pChangePasswd->deleteLater();
    }
}

void MainWindow::showStatusDlg(bool bShow)
{
    if (bShow)
    {
        m_pDialogStatus = new DialogStatus(m_pThread->getRelesStatus());
        m_pDialogStatus->setStyleSheet("background-image: none;  "\
                                       "background: rgb(180,180,180);" \
                                       "color: white;");
        m_pDialogStatus->showFullScreen();
        m_pToneIn->setParent (m_pDialogStatus);
        m_pDialogStatus->setManual (!m_bAutomatico);
        m_pDialogStatus->setVentilador(m_disparo,m_histeresis);
    }
    else
    {
       m_pDialogStatus->deleteLater();
       m_pToneIn->setParent (this);
    }
}

void MainWindow::on_srvce()
{
    ui->label->hide();
    m_currentStatus= MainWindow::iddle;
    m_pThread->setAudio(true);

    showStatusDlg(false);
}

void MainWindow::on_timercode()
{
    m_pToneIn->hide();

    m_code.clear();
    qDebug () << m_code.toHex();
    m_pTimerCode->stop();
}


void MainWindow::on_wats(float wats)
{
    if (m_pDialogStatus)
        m_pDialogStatus->setWats(wats);
    m_wats = wats;
    setBannerText();
}

void MainWindow::on_temperature (float temp)
{
    if (m_pDialogStatus)
        m_pDialogStatus->setTemperature(temp);

    if (m_bAutomatico)
    {
        if (temp>m_disparo)
            m_pThread->setVentilador(true);

        if (temp<m_disparo-m_histeresis)
            m_pThread->setVentilador(false);
    }
    m_temperatura=temp;
    setBannerText();
}
