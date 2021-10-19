/********************************************************************************
** Form generated from reading UI file 'timeset.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMESET_H
#define UI_TIMESET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_timeset
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *timeset)
    {
        if (timeset->objectName().isEmpty())
            timeset->setObjectName(QString::fromUtf8("timeset"));
        timeset->resize(800, 600);
        menubar = new QMenuBar(timeset);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        timeset->setMenuBar(menubar);
        centralwidget = new QWidget(timeset);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        timeset->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(timeset);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        timeset->setStatusBar(statusbar);

        retranslateUi(timeset);

        QMetaObject::connectSlotsByName(timeset);
    } // setupUi

    void retranslateUi(QMainWindow *timeset)
    {
        timeset->setWindowTitle(QApplication::translate("timeset", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class timeset: public Ui_timeset {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMESET_H
