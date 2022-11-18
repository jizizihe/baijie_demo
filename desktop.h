#ifndef DESKTOP_H
#define DESKTOP_H

#include <QMainWindow>
#include "mainwindow.h"
#include "database.h"
#include <QtQuick/QQuickTransform>

namespace Ui {
class Desktop;
}

class Desktop : public QMainWindow
{
    Q_OBJECT

public:
    explicit Desktop(QWidget *parent = 0);
    ~Desktop();
    void mousePressEvent(QMouseEvent *event);
    void showEvent(QShowEvent *event);
    QString executeLinuxCmd(QString strCmd);
    void select_language();

signals:
    void language_translate();

private:
    Ui::Desktop *ui;
    MainWindow g_mainwindowWg;
    database g_database;
};

#endif // DESKTOP_H
