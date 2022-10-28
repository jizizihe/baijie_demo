#ifndef USER_MANUAL_H
#define USER_MANUAL_H

#include <QWidget>
#include <QTreeWidget>
#include <QScrollBar>
namespace Ui {
class user_manual;
}

class user_manual : public QWidget
{
    Q_OBJECT

public:
    explicit user_manual(QWidget *parent = 0);
    ~user_manual();

    void languageReload();
    void userFont();

signals:
    void user_manual_back_msg();

private slots:
    void on_btn_ret_clicked();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_btn_ecpandAll_clicked();

private:
    Ui::user_manual *ui;
    bool btnExpandAllFlag = true;
};

#endif // USER_MANUAL_H
