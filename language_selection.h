#ifndef LANGUAGE_SELECTION_H
#define LANGUAGE_SELECTION_H

#include <QMainWindow>
#include "database.h"
#include "desktop.h"

namespace Ui {
class language_selection;
}

class language_selection : public QMainWindow
{
    Q_OBJECT

public:
    explicit language_selection(QWidget *parent = 0);
    ~language_selection();
    void setLanguageFont();
    void desktopShow();

public slots:
    void on_btn_ok_clicked();

signals:

private:
    Ui::language_selection *ui;
    database g_database;
    Desktop g_desktopWg;
};

#endif // LANGUAGE_SELECTION_H
