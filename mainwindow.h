#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QBuffer>
#include <QPointer>

#include "mythread.h"
#include "dialogstatus.h"
#include "tonein.h"
#include "changepasswd.h"
#include "smeter.h"

#define TIMEOUT_SERVICE     50000
#define TIMEOUT_SMETER      30*60*1000

#define TECLA_ALMOHADILLA   0x0c
#define TECLA_ASTERISCO     0x0b

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:

private:
    enum status
    {
        iddle,
        service,
        change_passwd_1,    //Entre clave
        change_passwd_2    //Vuelva a entrar clave
    };

    Ui::MainWindow     *ui;
    QTimer             *m_pTimer;
    QTimer             *m_pRequestTimer;
    QTimer             *m_pTimerCode;
    QTimer             *m_pTimerSrvce;
    QTimer             *m_pTimerSMeter;
    mythread           *m_pThread;
    MainWindow::status  m_currentStatus;
    QByteArray          m_code;    
    QList <int>         m_bReles;
    tonein             *m_pToneIn;
    bool                m_bAutomatico;
    float               m_disparo;
    float               m_histeresis;
    QString             m_passwd1;
    QString             m_passwd2;
    QString             m_strBannerText;
    QString             m_passwd;

    float               m_temperatura;
    float               m_wats;
    quint8              m_releStatus;

    QPointer <changePasswd> m_pChangePasswd;
    QPointer <DialogStatus> m_pDialogStatus;
    QPointer <SMeter>       m_pSMeter;



private:
    void showSMeterDlg(bool bShow);
    void showStatusDlg(bool bShow);
    void showChangePasswd(bool bShow);
    void setBannerText();

public slots:

    void on_timeout();
    void on_srvce();
    void on_tone (quint8 tone);
    void on_timercode();
    void on_temperature(float);
    void on_wats(float);
    void on_rele(quint8, bool);
    void on_signalIn (bool);
    void on_TimerSMeter();
};


#endif // MAINWINDOW_H
