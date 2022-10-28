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
    void saveFileFont();
    void fileChooseWidgetShow();

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
    QProcess proPath;
    File_opration File_oprationWg;
    database databaseWg;
};

#endif // VOICE_SAVEFILE_NAME_H
