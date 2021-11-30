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
    void refresh(QString);

private slots:


    void on_return_2_clicked();


    void on_choose_3_clicked();

    void on_begin_clicked();

    void on_play_clicked();

    void show_time();

    void on_horizontalSlider_valueChanged(int value);

signals:
    void Mysignal();

private:
    Ui::voice *ui;
    QString file_name;
};

#endif // VOICE_H
