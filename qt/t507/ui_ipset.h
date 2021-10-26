/********************************************************************************
** Form generated from reading UI file 'ipset.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IPSET_H
#define UI_IPSET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ipset
{
public:
    QPushButton *pushButton_5;
    QTextEdit *textEdit;
    QPushButton *pushButton_7;
    QPushButton *pushButton_9;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_6;
    QPushButton *pushButton_8;

    void setupUi(QWidget *ipset)
    {
        if (ipset->objectName().isEmpty())
            ipset->setObjectName(QString::fromUtf8("ipset"));
        ipset->resize(1024, 600);
        pushButton_5 = new QPushButton(ipset);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(60, 390, 230, 40));
        textEdit = new QTextEdit(ipset);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(320, 90, 680, 460));
        QFont font;
        font.setPointSize(10);
        textEdit->setFont(font);
        textEdit->setReadOnly(true);
        pushButton_7 = new QPushButton(ipset);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(60, 510, 230, 40));
        pushButton_9 = new QPushButton(ipset);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(60, 90, 230, 40));
        pushButton = new QPushButton(ipset);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 20, 100, 40));
        pushButton_2 = new QPushButton(ipset);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(60, 210, 230, 40));
        pushButton_3 = new QPushButton(ipset);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(60, 270, 230, 40));
        pushButton_4 = new QPushButton(ipset);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(60, 330, 230, 40));
        pushButton_6 = new QPushButton(ipset);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(60, 450, 230, 40));
        pushButton_8 = new QPushButton(ipset);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(60, 150, 230, 40));

        retranslateUi(ipset);

        QMetaObject::connectSlotsByName(ipset);
    } // setupUi

    void retranslateUi(QWidget *ipset)
    {
        ipset->setWindowTitle(QApplication::translate("ipset", "Form", nullptr));
        pushButton_5->setText(QApplication::translate("ipset", "modify static ip", nullptr));
        pushButton_7->setText(QApplication::translate("ipset", "speed test", nullptr));
        pushButton_9->setText(QApplication::translate("ipset", "ifconfig", nullptr));
        pushButton->setText(QApplication::translate("ipset", "return", nullptr));
        pushButton_2->setText(QApplication::translate("ipset", "set static ip", nullptr));
        pushButton_3->setText(QApplication::translate("ipset", "set dynamic ip", nullptr));
        pushButton_4->setText(QApplication::translate("ipset", "add static ip", nullptr));
        pushButton_6->setText(QApplication::translate("ipset", "delete static ip", nullptr));
        pushButton_8->setText(QApplication::translate("ipset", "show all IP ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ipset: public Ui_ipset {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IPSET_H
