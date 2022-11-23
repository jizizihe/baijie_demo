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
    void refreshFile(QString);
    void setVolume(int value);
    void setVoiceFont();
    void chooseFileWidgetShow();
    void saveFileWidgetShow();
    void renameFileWidgetShow();
    void record();
    void play();
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    int calculateTimeLength(QString length);
    float getWavTimeLength(QString file);    // Calculate the voice time of a wav file
    QString timeToString(int length);        // Time lenght change to "00:00:00" mode

private slots:
    void get_file_widget_path(QString);
    void file_choose_widget_hide();
    void show_recording_time();
    void get_playing_time_length();
    void update_file(QString);
    void record_save(QString fileName);
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
    QString g_fileName;
    voice_savefile_name g_saveFileWg;
    voice_rename g_renameWg;
    File_opration g_fileOprationWg;
    database g_database;
};

#endif // VOICE_H
