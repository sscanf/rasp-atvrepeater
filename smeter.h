#ifndef SMETER_H
#define SMETER_H

#include <QDialog>

namespace Ui {
class SMeter;
}

class SMeter : public QDialog
{
    Q_OBJECT

public:
    explicit SMeter(QWidget *parent = 0);
    void setWats(float wats);

    ~SMeter();

private:
    Ui::SMeter *ui;
};

#endif // SMETER_H
