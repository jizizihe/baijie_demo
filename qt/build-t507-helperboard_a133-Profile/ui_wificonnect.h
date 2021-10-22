/********************************************************************************
** Form generated from reading UI file 'wificonnect.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIFICONNECT_H
#define UI_WIFICONNECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WifiConnect
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WifiConnect)
    {
        if (WifiConnect->objectName().isEmpty())
            WifiConnect->setObjectName(QString::fromUtf8("WifiConnect"));
        WifiConnect->resize(800, 600);
        menubar = new QMenuBar(WifiConnect);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        WifiConnect->setMenuBar(menubar);
        centralwidget = new QWidget(WifiConnect);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        WifiConnect->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(WifiConnect);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        WifiConnect->setStatusBar(statusbar);

        retranslateUi(WifiConnect);

        QMetaObject::connectSlotsByName(WifiConnect);
    } // setupUi

    void retranslateUi(QMainWindow *WifiConnect)
    {
        WifiConnect->setWindowTitle(QApplication::translate("WifiConnect", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WifiConnect: public Ui_WifiConnect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIFICONNECT_H
