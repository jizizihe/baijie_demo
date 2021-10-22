#ifndef VOICE_H
#define VOICE_H

#include <QMainWindow>
#include <QDebug>
#include <QFileInfoList>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <qfileinfo.h>
#include <QProcess>

namespace Ui {
class voice;
}

class voice : public QMainWindow
{
    Q_OBJECT

public:
    explicit voice(QWidget *parent = 0);
    ~voice();

    void language_reload();

private slots:


    void on_return_2_clicked();


    void on_choose_3_clicked();

    void on_begin_clicked();

    void on_play_clicked();

signals:
    void Mysignal();

private:
    Ui::voice *ui;
};

#endif // VOICE_H
