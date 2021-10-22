/********************************************************************************
** Form generated from reading UI file 'hotspot.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOTSPOT_H
#define UI_HOTSPOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HotSpot
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *HotSpot)
    {
        if (HotSpot->objectName().isEmpty())
            HotSpot->setObjectName(QString::fromUtf8("HotSpot"));
        HotSpot->resize(800, 600);
        menubar = new QMenuBar(HotSpot);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        HotSpot->setMenuBar(menubar);
        centralwidget = new QWidget(HotSpot);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        HotSpot->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(HotSpot);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        HotSpot->setStatusBar(statusbar);

        retranslateUi(HotSpot);

        QMetaObject::connectSlotsByName(HotSpot);
    } // setupUi

    void retranslateUi(QMainWindow *HotSpot)
    {
        HotSpot->setWindowTitle(QApplication::translate("HotSpot", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HotSpot: public Ui_HotSpot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOTSPOT_H
