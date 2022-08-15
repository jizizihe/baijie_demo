#ifndef IPSET_POPUP_H
#define IPSET_POPUP_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QRegExpValidator>
#include <QPushButton>
#include <QtNetwork>

class ipset_popup : public QDialog
{
    Q_OBJECT

public:
    explicit ipset_popup(QWidget *parent = 0);
    ~ipset_popup();

    char *bnttype; 
    void language_reload();

signals:
    void addip();
    void addipinfor(QString,QString);
    void modip();
    void modipinfor(QString);
    void go_back();

private slots:
    void remain();
    void getaddipinforma();
    void getmodipinforma();
    void screen_clear();
    void returnmain();
private:
    QLabel *label_netcard;
    QLineEdit * line_netcard;
    QLabel *label_addr;
    QLineEdit *line_addr;
    QLabel *label;
    QPushButton *btn_OK;
    QPushButton *btn_return;
    QList<QHostAddress> list;
};

#endif // IPSET_POPUP_H
