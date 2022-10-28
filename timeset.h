#ifndef TIMESET_H
#define TIMESET_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QLabel>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QFont>

namespace Ui {
class timeset;
}

class timeset : public QMainWindow
{
    Q_OBJECT

public:
    explicit timeset(QWidget *parent = 0);
    ~timeset();
    void languageReload();
    void timesetFont();
    void showEvent(QShowEvent *event);
    void isSyncNetwork();
    QString SystimeSet(QString  dateText);
    QString RTCSet();

signals:
    void time_set_back_msg();

private slots:
    void btn_ret_clicked();
    void btn_sysTimeSet_clicked();
    void btn_RTCSet_clicked();
    void on_btn_sync_clicked();
    void sys_timer_update(void);
    void RTC_timer_update(void);

private:
    Ui::timeset *ui;
    QProcess proSys;
    QProcess proRTC;
    QTimer *SysTimer;
    QTimer *RTCTimer;
};

#endif // TIMESET_H
