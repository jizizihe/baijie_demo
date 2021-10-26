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
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
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
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *usb_detection;
    QLabel *label;
    QLabel *label_2;
    QPushButton *sim_detection;
    QPushButton *sd_detection;
    QTextEdit *message;
    QLabel *label_3;
    QFrame *frame;
    QLabel *usb_label;
    QFrame *frame_2;
    QLabel *sd_label;
    QFrame *frame_3;
    QLabel *sim_label;
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
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(60, 430, 91, 41));
        QFont font;
        font.setPointSize(11);
        label_4->setFont(font);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(230, 30, 531, 41));
        QFont font1;
        font1.setPointSize(18);
        label_5->setFont(font1);
        label_5->setAlignment(Qt::AlignCenter);
        usb_detection = new QPushButton(centralwidget);
        usb_detection->setObjectName(QString::fromUtf8("usb_detection"));
        usb_detection->setGeometry(QRect(770, 374, 131, 41));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 374, 91, 41));
        label->setFont(font);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 494, 91, 41));
        label_2->setFont(font);
        sim_detection = new QPushButton(centralwidget);
        sim_detection->setObjectName(QString::fromUtf8("sim_detection"));
        sim_detection->setGeometry(QRect(770, 494, 131, 41));
        sd_detection = new QPushButton(centralwidget);
        sd_detection->setObjectName(QString::fromUtf8("sd_detection"));
        sd_detection->setGeometry(QRect(770, 430, 131, 41));
        message = new QTextEdit(centralwidget);
        message->setObjectName(QString::fromUtf8("message"));
        message->setGeometry(QRect(10, 100, 1001, 251));
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
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(170, 360, 581, 51));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        usb_label = new QLabel(frame);
        usb_label->setObjectName(QString::fromUtf8("usb_label"));
        usb_label->setGeometry(QRect(3, 5, 571, 41));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(170, 430, 581, 51));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        sd_label = new QLabel(frame_2);
        sd_label->setObjectName(QString::fromUtf8("sd_label"));
        sd_label->setGeometry(QRect(5, 6, 571, 41));
        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(170, 494, 581, 51));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        sim_label = new QLabel(frame_3);
        sim_label->setObjectName(QString::fromUtf8("sim_label"));
        sim_label->setGeometry(QRect(3, 3, 571, 41));
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
        label->setText(QApplication::translate("udev", "USB:", nullptr));
        label_2->setText(QApplication::translate("udev", "SIM:", nullptr));
        sim_detection->setText(QApplication::translate("udev", "detection", nullptr));
        sd_detection->setText(QApplication::translate("udev", "detection", nullptr));
        label_3->setText(QApplication::translate("udev", "Status Message:", nullptr));
        usb_label->setText(QString());
        sd_label->setText(QString());
        sim_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class udev: public Ui_udev {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UDEV_H
