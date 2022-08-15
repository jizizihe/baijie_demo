#ifndef SOUND_RECORDER_H
#define SOUND_RECORDER_H

#include "voice.h"
#include "record.h"
#include <QMainWindow>

namespace Ui {
class sound_recorder;
}

class sound_recorder : public QMainWindow
{
    Q_OBJECT

public:
    explicit sound_recorder(QWidget *parent = 0);
    ~sound_recorder();
    void language_reload();
    voice voice_w;
    record record_w;

signals:
    void Mysignal();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_return_2_clicked();
    void show_sound_record();

private:
    Ui::sound_recorder *ui;
};

#endif // SOUND_RECORDER_H
