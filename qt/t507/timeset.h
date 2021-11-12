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

    void language_reload();

signals:
    void Mysignal();

private slots:
    void retBt_clicked();
    void SystimeSet(QString  datetext);
    void SystimeSetBt_clicked();
    QString RTCSet(QString  datetext);
    void RTCSetBt_clicked();

    void SystimerUpdate(void);
    void RTCtimerUpdate(void);

private:
    Ui::timeset *ui;

    QLabel *SystimeReadLabel;
    QLabel *RTCReadLabel;

    QPushButton *retBt;
    QLabel * pLabel;
    QDateTimeEdit *datetime;
    QPushButton * SystimeSetBt;
    QPushButton * RTCSetBt;
    QLabel *SystimepLabel;
    QLabel *RTCtimepLabel;

    QTimer *SysTimer;
    QTimer *RTCTimer;
};

#endif // TIMESET_H
