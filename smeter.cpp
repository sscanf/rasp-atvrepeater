#include "smeter.h"
#include "ui_smeter.h"

SMeter::SMeter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SMeter)
{
    ui->setupUi(this);
    ui->potencia->setSkin("Amperemeter");
    ui->potencia->setNeedleOrigin(0.5, .83596590);
    ui->potencia->setMinimum(0);
    ui->potencia->setMaximum(360);
    ui->potencia->setStartAngle(-42);
    ui->potencia->setEndAngle(42);
    ui->potencia->setValue(0);

    ui->potencia->setMaximum(2);
    ui->potencia->setMinimum(0);
}

SMeter::~SMeter()
{
    delete ui;
}

void SMeter::setWats (float wats)
{
    ui->potencia->setValue(wats);
}
