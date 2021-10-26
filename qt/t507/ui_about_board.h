/********************************************************************************
** Form generated from reading UI file 'about_board.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_BOARD_H
#define UI_ABOUT_BOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_about_board
{
public:
    QWidget *widget;
    QWidget *widget_2;
    QPushButton *pushButton;
    QGroupBox *groupBox;
    QLabel *batterylevel_label;
    QLabel *batterystatus_label;
    QLabel *CPU_temp_label;
    QLabel *resolution_label;
    QGroupBox *groupBox_3;
    QLabel *kernel_label;
    QLabel *OS_label;
    QGroupBox *groupBox_7;
    QLabel *name_label;

    void setupUi(QWidget *about_board)
    {
        if (about_board->objectName().isEmpty())
            about_board->setObjectName(QString::fromUtf8("about_board"));
        about_board->resize(1024, 600);
        widget = new QWidget(about_board);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 1024, 600));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(0, 0, 1024, 600));
        pushButton = new QPushButton(widget_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 100, 40));
        groupBox = new QGroupBox(widget_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(200, 110, 600, 270));
        batterylevel_label = new QLabel(groupBox);
        batterylevel_label->setObjectName(QString::fromUtf8("batterylevel_label"));
        batterylevel_label->setGeometry(QRect(20, 20, 550, 40));
        batterystatus_label = new QLabel(groupBox);
        batterystatus_label->setObjectName(QString::fromUtf8("batterystatus_label"));
        batterystatus_label->setGeometry(QRect(20, 80, 400, 40));
        CPU_temp_label = new QLabel(groupBox);
        CPU_temp_label->setObjectName(QString::fromUtf8("CPU_temp_label"));
        CPU_temp_label->setGeometry(QRect(20, 140, 400, 40));
        resolution_label = new QLabel(groupBox);
        resolution_label->setObjectName(QString::fromUtf8("resolution_label"));
        resolution_label->setGeometry(QRect(20, 200, 400, 40));
        groupBox_3 = new QGroupBox(widget_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(200, 400, 600, 180));
        kernel_label = new QLabel(groupBox_3);
        kernel_label->setObjectName(QString::fromUtf8("kernel_label"));
        kernel_label->setGeometry(QRect(20, 80, 400, 40));
        OS_label = new QLabel(groupBox_3);
        OS_label->setObjectName(QString::fromUtf8("OS_label"));
        OS_label->setGeometry(QRect(20, 20, 400, 40));
        groupBox_7 = new QGroupBox(widget_2);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(200, 20, 600, 80));
        name_label = new QLabel(groupBox_7);
        name_label->setObjectName(QString::fromUtf8("name_label"));
        name_label->setGeometry(QRect(20, 20, 400, 40));

        retranslateUi(about_board);

        QMetaObject::connectSlotsByName(about_board);
    } // setupUi

    void retranslateUi(QWidget *about_board)
    {
        about_board->setWindowTitle(QApplication::translate("about_board", "Form", nullptr));
        pushButton->setText(QApplication::translate("about_board", "return", nullptr));
        groupBox->setTitle(QString());
        batterylevel_label->setText(QApplication::translate("about_board", "TextLabel", nullptr));
        batterystatus_label->setText(QApplication::translate("about_board", "TextLabel", nullptr));
        CPU_temp_label->setText(QApplication::translate("about_board", "TextLabel", nullptr));
        resolution_label->setText(QApplication::translate("about_board", "TextLabel", nullptr));
        groupBox_3->setTitle(QString());
        kernel_label->setText(QApplication::translate("about_board", "TextLabel", nullptr));
        OS_label->setText(QApplication::translate("about_board", "TextLabel", nullptr));
        groupBox_7->setTitle(QString());
        name_label->setText(QApplication::translate("about_board", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class about_board: public Ui_about_board {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_BOARD_H
