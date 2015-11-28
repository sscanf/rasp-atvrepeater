#ifndef CHANGEPASSWD_H
#define CHANGEPASSWD_H

#include <QDialog>

namespace Ui {
class changePasswd;
}

class changePasswd : public QDialog
{
    Q_OBJECT

public:
    explicit changePasswd   (QWidget *parent = 0);
    ~changePasswd           ();
    void setTitle           (QString title);
    void passwd             (QString passwd, bool bSecret = true);
    QString getPasswd       ();

private:
    QString m_password;
    Ui::changePasswd *ui;
};

#endif // CHANGEPASSWD_H
