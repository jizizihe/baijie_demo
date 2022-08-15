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

    void language_reload();
    void about_font();

signals:
    void Mysignal();

private slots:
    void on_pushButton_clicked();
    void battery_update();
    void CPU_temp_update();
    void boardname_update();
    void kernelname_update();
    void OSname_update();
    void resolution_update();
    void QTversion_update();
private:
    Ui::about_board *ui;
    QLabel *QTversion;
};

#endif // ABOUT_BOARD_H
