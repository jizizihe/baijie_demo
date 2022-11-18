#ifndef ABOUT_BOARD_H
#define ABOUT_BOARD_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QProcess>
#include <QDebug>
#include <QLabel>

namespace Ui {
class about_board;
}

class about_board : public QWidget
{
    Q_OBJECT

public:
    explicit about_board(QWidget *parent = 0);
    ~about_board();
    void languageReload();
    void setAboutBoardFont();
    void showEvent(QShowEvent *event);

signals:
    void about_board_back_msg();

private slots:
    void battery_update();
    void cpu_temp_update();
    void board_name_update();
    void kernel_name_update();
    void os_name_update();
    void resolution_update();
    void qt_version_update();
    void on_btn_ret_clicked();

private:
    Ui::about_board *ui;
};

#endif // ABOUT_BOARD_H
