#ifndef FILE_OPRATION_H
#define FILE_OPRATION_H

#include <QMainWindow>
#include <QProcess>
#include <QFileInfoList>
#include <QFile>
#include <QFileDialog>
#include <QTreeWidget>

namespace Ui {
class File_opration;
}

class File_opration : public QMainWindow
{
    Q_OBJECT

public:
    explicit File_opration(QWidget *parent = 0);
    ~File_opration();
    int filePathFlag;                           // 0: Choose file  1: Choose path
    void showEvent(QShowEvent *event);
    void languageReload();
    void setFileOprationFont();

private slots:
    void read_bash_standard_output_info();
    void on_btn_back_clicked();  
    void on_btn_cancel_clicked();
    void on_btn_choose_clicked();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

signals:
    void file_rev_file_msg(QString,QString);    // Returns file and path signal
    void file_rev_path_msg(QString);            // Returns path signal
    void file_hide_msg();

private:
    Ui::File_opration *ui;
    QProcess g_proPath;
};

#endif // FILE_OPRATION_H
