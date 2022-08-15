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
    int filepath_flag;
    void showEvent(QShowEvent *event);
    void language_reload();
    void file_font();

private slots:
    void readBashStandardOutputInfo();

    void on_btn_back_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_btn_cancel_clicked();

    void on_pushButton_clicked();

signals:
    void file_rev1(QString,QString);
    void file_rev2(QString);
    void file_hide();

private:
    Ui::File_opration *ui;
    QProcess pro_path;
};

#endif // FILE_OPRATION_H
