/********************************************************************************
** Form generated from reading UI file 'all_interface_test.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALL_INTERFACE_TEST_H
#define UI_ALL_INTERFACE_TEST_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_all_interface_test
{
public:
    QWidget *centralwidget;
    QPushButton *return_2;
    QPushButton *test;
    QTextEdit *textEdit;
    QPushButton *clear;
    QPushButton *pushButton_3;
    QLabel *image;
    QPushButton *pushButton_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *all_interface_test)
    {
        if (all_interface_test->objectName().isEmpty())
            all_interface_test->setObjectName(QString::fromUtf8("all_interface_test"));
        all_interface_test->resize(1024, 600);
        centralwidget = new QWidget(all_interface_test);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        return_2 = new QPushButton(centralwidget);
        return_2->setObjectName(QString::fromUtf8("return_2"));
        return_2->setGeometry(QRect(10, 10, 100, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/t507_button_image/return.png"), QSize(), QIcon::Normal, QIcon::On);
        return_2->setIcon(icon);
        test = new QPushButton(centralwidget);
        test->setObjectName(QString::fromUtf8("test"));
        test->setGeometry(QRect(20, 100, 150, 45));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(290, 90, 711, 421));
        textEdit->setReadOnly(true);
        clear = new QPushButton(centralwidget);
        clear->setObjectName(QString::fromUtf8("clear"));
        clear->setGeometry(QRect(20, 170, 150, 45));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(20, 230, 150, 45));
        image = new QLabel(centralwidget);
        image->setObjectName(QString::fromUtf8("image"));
        image->setGeometry(QRect(40, 370, 240, 180));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(20, 290, 150, 45));
        all_interface_test->setCentralWidget(centralwidget);
        menubar = new QMenuBar(all_interface_test);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1024, 23));
        all_interface_test->setMenuBar(menubar);
        statusbar = new QStatusBar(all_interface_test);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        all_interface_test->setStatusBar(statusbar);

        retranslateUi(all_interface_test);

        QMetaObject::connectSlotsByName(all_interface_test);
    } // setupUi

    void retranslateUi(QMainWindow *all_interface_test)
    {
        all_interface_test->setWindowTitle(QApplication::translate("all_interface_test", "MainWindow", nullptr));
        return_2->setText(QString());
        test->setText(QApplication::translate("all_interface_test", "test", nullptr));
        clear->setText(QApplication::translate("all_interface_test", "clear", nullptr));
        pushButton_3->setText(QApplication::translate("all_interface_test", "photo", nullptr));
        image->setText(QString());
        pushButton_4->setText(QApplication::translate("all_interface_test", "show", nullptr));
    } // retranslateUi

};

namespace Ui {
    class all_interface_test: public Ui_all_interface_test {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALL_INTERFACE_TEST_H
