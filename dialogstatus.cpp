#include "dialogstatus.h"
#include "ui_dialogstatus.h"
#include <QSettings>

DialogStatus::DialogStatus(quint8 releStatus, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStatus)
{
    ui->setupUi(this);

    QSettings settings("/mnt/config/carta.ini",QSettings::IniFormat);

    ui->label_rele1->setText (settings.value ("reles/rele_1").toString());
    ui->label_rele2->setText (settings.value ("reles/rele_2").toString());
    ui->label_rele3->setText (settings.value ("reles/rele_3").toString());
    ui->label_rele4->setText (settings.value ("reles/rele_4").toString());
    ui->label_rele5->setText (settings.value ("reles/rele_5").toString());
    ui->label_rele6->setText (settings.value ("reles/rele_6").toString());


    ui->groupBox->setStyleSheet("background-color: gray; color: white; font-size: 24px");
    ui->groupBox_2->setStyleSheet("background-color: gray; color: white; font-size: 24px");
    ui->groupBox_3->setStyleSheet("background-color: gray; color: white; font-size: 24px");

    ui->rele1->setSkin("BerylSquare");
    ui->rele2->setSkin("BerylSquare");
    ui->rele3->setSkin("BerylSquare");
    ui->rele4->setSkin("BerylSquare");
    ui->rele5->setSkin("BerylSquare");
    ui->rele6->setSkin("BerylSquare");
    ui->potencia->setSkin("Amperemeter");
    ui->temperatura->setSkin ("Thermometer");
    ui->tempsettings->setSkin ("Beryl");


    ui->temperatura->setNeedleOrigin(0.456, 0.459);
    ui->temperatura->setMinimum(0);
    ui->temperatura->setMaximum(360);
    ui->temperatura->setStartAngle(-150);
    ui->temperatura->setEndAngle(150);
    ui->temperatura->setValue(0);
    ui->temperatura->setMinimum(-30);
    ui->temperatura->setMaximum(50);

    ui->potencia->setNeedleOrigin(0.5, .83596590);
    ui->potencia->setMinimum(0);
    ui->potencia->setMaximum(360);
    ui->potencia->setStartAngle(-42);
    ui->potencia->setEndAngle(42);
    ui->potencia->setValue(0);

    ui->potencia->setMaximum(2);
    ui->potencia->setMinimum(0);

    m_reles << ui->rele1;
    m_reles << ui->rele2;
    m_reles << ui->rele3;
    m_reles << ui->rele4;
    m_reles << ui->rele5;
    m_reles << ui->rele6;


    setReleStatus(releStatus);
}



void DialogStatus::setVentilador  (float disparo, float histeresis)
{
    ui->tempsettings->setMaximum(50);
    ui->tempsettings->setMaximumRange(disparo);
    ui->tempsettings->setMinimumRange(disparo-histeresis);

    QString str;
    str.sprintf ("D: %02.1f",disparo);
    ui->disparo->setText (str);

    str.sprintf ("H: %02.1f",histeresis);
    ui->histeresis->setText (str);

}

void DialogStatus::setReleStatus(quint8 status)
{
    quint8 mask=0x01;

    for (int n=0;n<6;n++)
    {
        if (status&mask)
            setRele (n,true);
        else
            setRele (n,false);

        mask <<= 1;
    }
}

void DialogStatus::setLock (bool bLock)
{
    if (bLock)
        ui->lock->setPixmap(QPixmap (QString::fromUtf8(":/images/lock_closed")));
    else
        ui->lock->setPixmap(QPixmap (QString::fromUtf8(":/images/lock_open")));
}

void DialogStatus::setManual (bool bManual)
{
    if (bManual)
    {
        ui->led_manual->setPixmap(QPixmap (QString::fromUtf8(":/images/led_on")));
        ui->led_automatico->setPixmap(QPixmap (QString::fromUtf8(":/images/led_off")));
    }
    else
    {
        ui->led_manual->setPixmap(QPixmap (QString::fromUtf8(":/images/led_off")));
        ui->led_automatico->setPixmap(QPixmap (QString::fromUtf8(":/images/led_on")));
    }
}

void DialogStatus::setSignalIn (bool bActive)
{
    if (bActive)
        ui->led_signal->setPixmap(QPixmap (QString::fromUtf8(":/images/led_on")));
    else
        ui->led_signal->setPixmap(QPixmap (QString::fromUtf8(":/images/led_off")));
}

void DialogStatus::setTemperature (float temp)
{
    ui->temperatura->setValue(temp);

    QString str;
    str.sprintf ("%02.1f",temp);
    ui->label_temperature->setText (str);
    ui->tempsettings->setValue(temp);

}

void DialogStatus::setWats (float wats)
{
    ui->potencia->setValue(wats);
/*
    QString str;
    str.sprintf ("%02.1f",wats);
    ui->label_wats->setText (str);*/
}

void DialogStatus::setRele (quint8 rele, bool bStatus)
{
    QtSvgToggleSwitch *pRele = (QtSvgToggleSwitch *)m_reles[rele];
    pRele->setChecked(bStatus);
}

DialogStatus::~DialogStatus()
{
    delete ui;
}
