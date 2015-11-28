#include "changepasswd.h"
#include "ui_changepasswd.h"

changePasswd::changePasswd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changePasswd)
{

    this->setWindowFlags( Qt::SubWindow | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    //this->setFixedSize(84,56);
    this->adjustSize();
    //this->move (QPoint (600,530));

    ui->setupUi(this);
}

void changePasswd::setTitle(QString title)
{
    ui->title->setText (title);
}

void changePasswd::passwd(QString passwd, bool bSecret)
{
    QString secret;

    m_password=passwd;

    if (bSecret)
    {
        secret.fill('*',passwd.length());
        ui->password->setText (secret);
    }
    else
    {
        ui->password->setText (passwd);
    }
}

QString changePasswd::getPasswd()
{
    return m_password;
}

changePasswd::~changePasswd()
{
    delete ui;
}
