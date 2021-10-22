#ifndef ETH0_POPUP_H
#define ETH0_POPUP_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QRegExpValidator>
#include <QPushButton>

class eth0_popup : public QDialog
{
    Q_OBJECT

public:
    explicit eth0_popup(QWidget *parent = 0);
    ~eth0_popup();

    char *bnttype;
    void language_reload();

signals:
    void addip();
    void addipinfor(QString,QString,QString);
    void modip();
    void modipinfor(QString,QString,QString);
    void delip();
    void delipinfor(QString);
private slots:
    void remain();
    void getaddipinforma();
    void getmodipinforma();
    void getdelipinforma();
private:
    QLabel *label_Name;
    QLineEdit *line_name;
    QLabel *label_addr;
    QLineEdit *line_addr;
    QLabel *label_gateway;
    QLineEdit *line_gateway;
    QLabel *label;
    QPushButton *remainmenu;
};

#endif // ETH0_POPUP_H
