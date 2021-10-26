#ifndef IPSET_POPUP_H
#define IPSET_POPUP_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QRegExpValidator>
#include <QPushButton>

class ipset_popup : public QDialog
{
    Q_OBJECT

public:
    explicit ipset_popup(QWidget *parent = 0);
    ~ipset_popup();

    char *bnttype;
    bool setitflag = false;
    void language_reload();

signals:
    void setdip();
    void setdyninfor(QString);
    void setsip();
    void setsipinfor(QString,QString,QString,QString);
    void addip();
    void addipinfor(QString,QString,QString,QString);
    void modip();
    void modipinfor(QString,QString,QString);
    void delip();
    void delipinfor(QString);
    void go_back();

private slots:
    void remain();
    void getaddipinforma();
    void getsetsipinforma();
    void getsetdipinforma();
    void getmodipinforma();
    void getdelipinforma();
    void screen_clear();
    void returnmain();
private:
    QLabel *label_netcard;
    QLineEdit * line_netcard;
    QLabel *label_Name;
    QLineEdit *line_name;
    QLabel *label_addr;
    QLineEdit *line_addr;
    QLabel *label_gateway;
    QLineEdit *line_gateway;
    QLabel *label;
    QPushButton *btn_OK;
    QPushButton *btn_return;
};

#endif // IPSET_POPUP_H
