/********************************************************************************
** Form generated from reading UI file 'eth0set.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ETH0SET_H
#define UI_ETH0SET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_eth0set
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QTextEdit *textEdit;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;

    void setupUi(QWidget *eth0set)
    {
        if (eth0set->objectName().isEmpty())
            eth0set->setObjectName(QString::fromUtf8("eth0set"));
        eth0set->resize(1024, 650);
        pushButton = new QPushButton(eth0set);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 100, 40));
        pushButton_2 = new QPushButton(eth0set);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(60, 200, 230, 40));
        pushButton_3 = new QPushButton(eth0set);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(60, 260, 230, 40));
        pushButton_4 = new QPushButton(eth0set);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(60, 320, 230, 40));
        pushButton_5 = new QPushButton(eth0set);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(60, 380, 230, 40));
        pushButton_6 = new QPushButton(eth0set);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(60, 440, 230, 40));
        pushButton_7 = new QPushButton(eth0set);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(60, 500, 230, 40));
        textEdit = new QTextEdit(eth0set);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(320, 80, 650, 460));
        QFont font;
        font.setPointSize(9);
        textEdit->setFont(font);
        textEdit->setReadOnly(true);
        pushButton_8 = new QPushButton(eth0set);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(60, 140, 230, 40));
        pushButton_9 = new QPushButton(eth0set);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(60, 80, 230, 40));

        retranslateUi(eth0set);

        QMetaObject::connectSlotsByName(eth0set);
    } // setupUi

    void retranslateUi(QWidget *eth0set)
    {
        eth0set->setWindowTitle(QApplication::translate("eth0set", "Form", nullptr));
        pushButton->setText(QApplication::translate("eth0set", "return", nullptr));
        pushButton_2->setText(QApplication::translate("eth0set", "set static ip", nullptr));
        pushButton_3->setText(QApplication::translate("eth0set", "set dynamic ip", nullptr));
        pushButton_4->setText(QApplication::translate("eth0set", "add static ip", nullptr));
        pushButton_5->setText(QApplication::translate("eth0set", "modify static ip", nullptr));
        pushButton_6->setText(QApplication::translate("eth0set", "delete static ip", nullptr));
        pushButton_7->setText(QApplication::translate("eth0set", "speed test", nullptr));
        pushButton_8->setText(QApplication::translate("eth0set", "display all static ip", nullptr));
        pushButton_9->setText(QApplication::translate("eth0set", "ifconfig", nullptr));
    } // retranslateUi

};

namespace Ui {
    class eth0set: public Ui_eth0set {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ETH0SET_H
