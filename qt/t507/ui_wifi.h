/********************************************************************************
** Form generated from reading UI file 'wifi.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIFI_H
#define UI_WIFI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wifi
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *wifi)
    {
        if (wifi->objectName().isEmpty())
            wifi->setObjectName(QString::fromUtf8("wifi"));
        wifi->resize(800, 600);
        menubar = new QMenuBar(wifi);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        wifi->setMenuBar(menubar);
        centralwidget = new QWidget(wifi);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        wifi->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(wifi);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        wifi->setStatusBar(statusbar);

        retranslateUi(wifi);

        QMetaObject::connectSlotsByName(wifi);
    } // setupUi

    void retranslateUi(QMainWindow *wifi)
    {
        wifi->setWindowTitle(QApplication::translate("wifi", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class wifi: public Ui_wifi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIFI_H
