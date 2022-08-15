#ifndef VOICE_SAVEFILE_NAME_H
#define VOICE_SAVEFILE_NAME_H

#include <QMainWindow>
#include <QFileInfoList>
#include <QFile>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include "file_opration.h"

namespace Ui {
class voice_savefile_name;
}

class voice_savefile_name : public QMainWindow
{
    Q_OBJECT

public:
    explicit voice_savefile_name(QWidget *parent = 0);
    ~voice_savefile_name();
    void language_reload();
    void get_filename(QString);
    void save_as();
    void voice_savefile_font();
    void file_choose_show();


private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

    void on_opefile_btn_clicked();
    void file_path(QString);
    void file_hide();

signals:
    void fiel_pn(QString);
    void save_back();

private:
    Ui::voice_savefile_name *ui;
    QProcess pro_pathh;
    File_opration File_oprationw;
};

#endif // VOICE_SAVEFILE_NAME_H
