/********************************************************************************
** Form generated from reading UI file 'serial.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIAL_H
#define UI_SERIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_serial
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *serial)
    {
        if (serial->objectName().isEmpty())
            serial->setObjectName(QString::fromUtf8("serial"));
        serial->resize(800, 600);
        menubar = new QMenuBar(serial);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        serial->setMenuBar(menubar);
        centralwidget = new QWidget(serial);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        serial->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(serial);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        serial->setStatusBar(statusbar);

        retranslateUi(serial);

        QMetaObject::connectSlotsByName(serial);
    } // setupUi

    void retranslateUi(QMainWindow *serial)
    {
        serial->setWindowTitle(QApplication::translate("serial", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class serial: public Ui_serial {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIAL_H
