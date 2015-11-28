#ifndef DIALOGSTATUS_H
#define DIALOGSTATUS_H

#include <QDialog>

namespace Ui {
class DialogStatus;
}

class DialogStatus : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStatus(quint8 releStatus, QWidget *parent = 0);
    ~DialogStatus();

    void setTemperature (float temp);
    void setWats        (float wats);
    void setRele        (quint8 rele, bool bStatus);
    void setSignalIn    (bool bActive);
    void setLock        (bool bLock);
    void setManual      (bool bManual);
    void setVentilador  (float disparo, float histeresis);

private:
    void setReleStatus(quint8 status);

private:

    QList <QWidget *> m_reles;

    Ui::DialogStatus *ui;
};

#endif // DIALOGSTATUS_H
