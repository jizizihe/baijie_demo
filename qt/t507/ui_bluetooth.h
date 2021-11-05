/********************************************************************************
** Form generated from reading UI file 'bluetooth.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLUETOOTH_H
#define UI_BLUETOOTH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_bluetooth
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *bluetooth)
    {
        if (bluetooth->objectName().isEmpty())
            bluetooth->setObjectName(QString::fromUtf8("bluetooth"));
        bluetooth->resize(800, 600);
        menubar = new QMenuBar(bluetooth);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        bluetooth->setMenuBar(menubar);
        centralwidget = new QWidget(bluetooth);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        bluetooth->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(bluetooth);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        bluetooth->setStatusBar(statusbar);

        retranslateUi(bluetooth);

        QMetaObject::connectSlotsByName(bluetooth);
    } // setupUi

    void retranslateUi(QMainWindow *bluetooth)
    {
        bluetooth->setWindowTitle(QApplication::translate("bluetooth", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class bluetooth: public Ui_bluetooth {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLUETOOTH_H
