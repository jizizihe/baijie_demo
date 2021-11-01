#ifndef USER_MANUAL_H
#define USER_MANUAL_H

#include <QWidget>
#include <QTreeWidget>
namespace Ui {
class user_manual;
}

class user_manual : public QWidget
{
    Q_OBJECT

public:
    explicit user_manual(QWidget *parent = 0);
    ~user_manual();

    void language_reload();
signals:
    void Mysignal();

private slots:
    void on_pushButton_clicked();


    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::user_manual *ui;
};

#endif // USER_MANUAL_H
