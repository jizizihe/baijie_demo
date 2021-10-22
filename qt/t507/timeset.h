#ifndef TIMESET_H
#define TIMESET_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QLabel>
#include <QDebug>

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
    void RTCSet(QString  datetext);
    void RTCSetBt_clicked();

private:
    Ui::timeset *ui;

    QPushButton *retBt;
    QLabel * pLabel;
    QDateTimeEdit *datetime;
    QPushButton * SystimeSetBt;
    QPushButton * RTCSetBt;
};

#endif // TIMESET_H
