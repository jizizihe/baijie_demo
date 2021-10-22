#ifndef KEYTEST_H
#define KEYTEST_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QDebug>
#include <string>
#include <QFont>

#include <keythread.h>

namespace Ui {
class keytest;
}

class keytest : public QMainWindow
{
    Q_OBJECT

public:
    explicit keytest(QWidget *parent = 0);
    ~keytest();

    void language_reload();

signals:
    void Mysignal();

private slots:
    void retBt_clicked();
    void startBt_clicked();
    void stopBt_clicked();
    void recvmsg(QString);

private:
    Ui::keytest *ui;

    QPushButton * retBt;
    QPushButton * StartBt;
    QPushButton * StopBt;
    QLabel * KeyLabel;
    QTextEdit * keyText;

    keythread *task=new keythread(NULL);//声明线程

};

#endif // KEYTEST_H
