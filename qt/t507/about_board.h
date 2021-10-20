#ifndef ABOUT_BOARD_H
#define ABOUT_BOARD_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
namespace Ui {
class about_board;
}

class about_board : public QWidget
{
    Q_OBJECT

public:
    explicit about_board(QWidget *parent = 0);
    ~about_board();

signals:
    void Mysignal();

private slots:
    void on_pushButton_clicked();
    void battery_update();
    void CPU_temp_update();


private:
    Ui::about_board *ui;
};

#endif // ABOUT_BOARD_H
