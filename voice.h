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

    void languageReload();
    void refresh(QString);
    QString timeToString(int length); // time lenght change to "00:00:00" mode
    int calculateTimeLength(QString length);
    void setVolume(int value);
    void voiceFont();
    void fileChooseWidgetshow();
    float getWavTimeLength(QString file);  //Calculate the voice time of a wav file
    void saveFileWidgetShow();
    void renameFileWidgetShow();
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void file_path(QString);
    void file_choose_widget_hide();
    void show_recording_time();
    void get_playing_time_length();
    void update_file(QString);
    void record_save(QString);
    void on_btn_ret_clicked();
    void on_btn_record_clicked();
    void on_btn_play_clicked();
    void on_btn_playVoice_clicked();
    void on_btn_delete_clicked();
    void on_btn_rename_clicked();
    void on_btn_volume_clicked();
    void on_btn_startRecord_clicked();
    void on_Slider_volume_valueChanged(int value);
    void on_btn_chooseFile_clicked();

public slots:
    void save_file_widget_hide();
    void rename_file_widget_hide();
    void volume_slider_change(int);

signals:
    void voice_back_msg();
    void volume_slider_value_change_msg(int);

private:
    Ui::voice *ui;
    QString fileName;
    voice_savefile_name saveFileWg;
    voice_rename renameWg;
    File_opration File_oprationWg;
    database databaseWg;
};

#endif // VOICE_H
