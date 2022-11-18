#ifndef VOICE_RENAME_H
#define VOICE_RENAME_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QObject>

namespace Ui {
class voice_rename;
}

class voice_rename : public QMainWindow
{
    Q_OBJECT

public:
    explicit voice_rename(QWidget *parent = 0);
    ~voice_rename();
    void getBeforeFile(QString,QString);
    void rename(QString newName,QString beforeName);
    void languageReload();
    void setVoiceRenameFont();

private slots:
    void on_btn_ok_clicked();
    void on_btn_cel_clicked();

signals:
    void rename_finish_msg(QString);
    void rename_back_msg();

private:
    Ui::voice_rename *ui;
};

#endif // VOICE_RENAME_H
