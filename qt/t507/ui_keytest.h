/********************************************************************************
** Form generated from reading UI file 'keytest.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYTEST_H
#define UI_KEYTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_keytest
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *keytest)
    {
        if (keytest->objectName().isEmpty())
            keytest->setObjectName(QString::fromUtf8("keytest"));
        keytest->resize(800, 600);
        centralwidget = new QWidget(keytest);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        keytest->setCentralWidget(centralwidget);
        menubar = new QMenuBar(keytest);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        keytest->setMenuBar(menubar);
        statusbar = new QStatusBar(keytest);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        keytest->setStatusBar(statusbar);

        retranslateUi(keytest);

        QMetaObject::connectSlotsByName(keytest);
    } // setupUi

    void retranslateUi(QMainWindow *keytest)
    {
        keytest->setWindowTitle(QApplication::translate("keytest", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class keytest: public Ui_keytest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYTEST_H
