#ifndef VOICE_H
#define VOICE_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>

namespace Ui {
class voice;
}

class voice : public QMainWindow
{
    Q_OBJECT

public:
    explicit voice(QWidget *parent = 0);
    ~voice();

private slots:
    void on_pushButton_clicked();

    void on_play_clicked();

    void on_return_2_clicked();

signals:
    void Mysignal();

private:
    Ui::voice *ui;
};

#endif // VOICE_H
