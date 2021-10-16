#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>

namespace Ui {
class HotSpot;
}

class HotSpot : public QMainWindow
{
    Q_OBJECT

public:
    explicit HotSpot(QWidget *parent = 0);
    ~HotSpot();

private slots:
    void HotSpotUpBt_clicked();
    void HotSpotDownBt_clicked();
    void HotSpotCloseBt_clicked();

private:
    Ui::HotSpot *ui;

    QLabel * HotSpotTitleLab;
    QLabel * HotSpotNameLab;
    QLabel * HotSpotPasswdLab;
    QLineEdit * HotSpotNameLine;
    QLineEdit * HotSpotPasswdLine;
    QPushButton * HotSpotUpBt;
    QPushButton * HotSpotDownBt;
    QPushButton * HotSpotCloseBt;

};

#endif // HOTSPOT_H
