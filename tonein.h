#ifndef TONEIN_H
#define TONEIN_H

#include <QDialog>

namespace Ui {
class tonein;
}

class tonein : public QDialog
{
    Q_OBJECT

public:
    explicit tonein(QWidget *parent = 0);
    void setNumber (QString number);
    ~tonein();

private:
    Ui::tonein *ui;
};

#endif // TONEIN_H
