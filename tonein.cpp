#include "tonein.h"
#include "ui_tonein.h"

tonein::tonein(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tonein)
{

    this->setWindowFlags( Qt::SubWindow | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    this->setFixedSize(84,56);
    this->adjustSize();
    this->move (QPoint (600,530));
    ui->setupUi(this);

}

tonein::~tonein()
{
    delete ui;
}

void tonein::setNumber(QString number)
{
    ui->label->setText (number);
}
