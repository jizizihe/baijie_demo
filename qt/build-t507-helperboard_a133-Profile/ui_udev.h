/********************************************************************************
** Form generated from reading UI file 'udev.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UDEV_H
#define UI_UDEV_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_udev
{
public:
    QWidget *centralwidget;
    QPushButton *return_2;
    QLineEdit *sd_lineEdit;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *sim_lineEdit;
    QPushButton *usb_detection;
    QLineEdit *usb_lineEdit;
    QLabel *label;
    QLabel *label_2;
    QPushButton *sim_detection;
    QPushButton *sd_detection;
    QTextEdit *message;
    QLabel *label_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *udev)
    {
        if (udev->objectName().isEmpty())
            udev->setObjectName(QString::fromUtf8("udev"));
        udev->resize(1024, 600);
        centralwidget = new QWidget(udev);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        return_2 = new QPushButton(centralwidget);
        return_2->setObjectName(QString::fromUtf8("return_2"));
        return_2->setGeometry(QRect(0, 0, 161, 31));
        sd_lineEdit = new QLineEdit(centralwidget);
        sd_lineEdit->setObjectName(QString::fromUtf8("sd_lineEdit"));
        sd_lineEdit->setGeometry(QRect(170, 370, 571, 41));
        sd_lineEdit->setReadOnly(true);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(60, 370, 91, 41));
        QFont font;
        font.setPointSize(11);
        label_4->setFont(font);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(280, 30, 531, 41));
        QFont font1;
        font1.setPointSize(18);
        label_5->setFont(font1);
        sim_lineEdit = new QLineEdit(centralwidget);
        sim_lineEdit->setObjectName(QString::fromUtf8("sim_lineEdit"));
        sim_lineEdit->setGeometry(QRect(170, 424, 571, 41));
        sim_lineEdit->setReadOnly(true);
        usb_detection = new QPushButton(centralwidget);
        usb_detection->setObjectName(QString::fromUtf8("usb_detection"));
        usb_detection->setGeometry(QRect(770, 314, 131, 41));
        usb_lineEdit = new QLineEdit(centralwidget);
        usb_lineEdit->setObjectName(QString::fromUtf8("usb_lineEdit"));
        usb_lineEdit->setGeometry(QRect(170, 314, 571, 41));
        usb_lineEdit->setReadOnly(true);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 314, 91, 41));
        label->setFont(font);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 424, 91, 41));
        label_2->setFont(font);
        sim_detection = new QPushButton(centralwidget);
        sim_detection->setObjectName(QString::fromUtf8("sim_detection"));
        sim_detection->setGeometry(QRect(770, 424, 131, 41));
        sd_detection = new QPushButton(centralwidget);
        sd_detection->setObjectName(QString::fromUtf8("sd_detection"));
        sd_detection->setGeometry(QRect(770, 370, 131, 41));
        message = new QTextEdit(centralwidget);
        message->setObjectName(QString::fromUtf8("message"));
        message->setGeometry(QRect(10, 107, 1001, 201));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        QBrush brush1(QColor(190, 190, 190, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        message->setPalette(palette);
        message->setReadOnly(true);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 60, 231, 41));
        QFont font2;
        font2.setPointSize(13);
        label_3->setFont(font2);
        udev->setCentralWidget(centralwidget);
        menubar = new QMenuBar(udev);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1024, 23));
        udev->setMenuBar(menubar);
        statusbar = new QStatusBar(udev);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        udev->setStatusBar(statusbar);

        retranslateUi(udev);

        QMetaObject::connectSlotsByName(udev);
    } // setupUi

    void retranslateUi(QMainWindow *udev)
    {
        udev->setWindowTitle(QApplication::translate("udev", "MainWindow", nullptr));
        return_2->setText(QApplication::translate("udev", "return", nullptr));
        label_4->setText(QApplication::translate("udev", "SD:", nullptr));
        label_5->setText(QApplication::translate("udev", "USB SD SIM status check", nullptr));
        usb_detection->setText(QApplication::translate("udev", "detection", nullptr));
        label->setText(QApplication::translate("udev", "Usb", nullptr));
        label_2->setText(QApplication::translate("udev", "Sim", nullptr));
        sim_detection->setText(QApplication::translate("udev", "detection", nullptr));
        sd_detection->setText(QApplication::translate("udev", "detection", nullptr));
        label_3->setText(QApplication::translate("udev", "Status Message", nullptr));
    } // retranslateUi

};

namespace Ui {
    class udev: public Ui_udev {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UDEV_H
