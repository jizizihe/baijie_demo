#ifndef VOICE_SAVEFILE_NAME_H
#define VOICE_SAVEFILE_NAME_H

#include <QMainWindow>
#include <QFileInfoList>
#include <QFile>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include "file_opration.h"
#include "database.h"

namespace Ui {
class voice_savefile_name;
}

class voice_savefile_name : public QMainWindow
{
    Q_OBJECT

public:
    explicit voice_savefile_name(QWidget *parent = 0);
    ~voice_savefile_name();
    void languageReload();
    void getFileName(QString fileName);
    void saveFile();
    void setSaveFileFont();
    void chooseFileWidgetShow();

private slots:
    void on_btn_cancel_clicked();
    void on_btn_ok_clicked();
    void on_btn_chooseFile_clicked();
    void get_file_path(QString);
    void choose_file_widget_hide();

signals:
    void save_back_msg();

private:
    Ui::voice_savefile_name *ui;
    QProcess g_proPath;
    File_opration g_fileOprationWg;
    database g_database;
};

#endif // VOICE_SAVEFILE_NAME_H
