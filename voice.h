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
#include <QSlider>
#include <QMouseEvent>
#include <QTranslator>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScreen>
#include <QApplication>

#include "voice_savefile_name.h"
#include "voice_rename.h"
#include "file_opration.h"
#include "database.h"

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
    QString record_timelength(int length);
    int time_length(QString length);
    void set_volume(int value);
    void settext_size();
    void voice_font();
    void closeEvent(QCloseEvent *event);

private slots:

    void file_path(QString);
    void on_return_2_clicked();
    void file_choose_show();

    void refile_hide();

    void on_begin_clicked();

    void on_play_clicked();

    void show_time();

   // void on_horizontalSlider_valueChanged(int value);

    void get_nowlength();
    float wav_time(QString s);
    void record_save(QString);
    void update_file();

    void on_btn_paly_clicked();

    void on_btn_paly_2_clicked();

    void on_btn_rename_clicked();

    void on_pushButton_clicked();

    void on_btn_rec_clicked();

    void on_Slider_volume_valueChanged(int value);

    void on_choose_btn_clicked();

public slots:
    void save_fileshow();
    void rename_fileshow();
    void savew_hide();
    void renamew_hide();
    void slider_change(int);

signals:
    void Mysignal();
    void sliderchange(int);

private:
    Ui::voice *ui;
    QString file_name;
    voice_savefile_name changname;
    voice_rename rename_w;
    QProcess pro_path;
    File_opration File_oprationw;
    database database_w;
};

#endif // VOICE_H
