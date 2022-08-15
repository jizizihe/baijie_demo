#ifndef RECORD_H
#define RECORD_H

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
class record;
}

class record : public QMainWindow
{
    Q_OBJECT

public:
    explicit record(QWidget *parent = 0);
    ~record();
    void language_reload();
    void refresh(QString);

private slots:


    void on_return_2_clicked();


    void on_choose_3_clicked();

    //void on_begin_clicked();

    //void on_play_clicked();

    void show_time();

   // void on_horizontalSlider_valueChanged(int value);

   // void get_audiotime();

    int calculatetime(QString startime,QString endtime);
    void chang_filename(QString str);
    //void get_nowlength();

    //void on_horizontalSlider_sliderReleased();

    //void on_horizontalSlider_sliderPressed();

    void on_begin_clicked();

signals:
    void Mysignal_r();

private:
    Ui::record *ui;
    QString file_name;

};

#endif // RECORD_H
