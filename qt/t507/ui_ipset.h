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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ipset
{
public:
    QPushButton *modstaticip;
    QTextEdit *textEdit;
    QPushButton *ipshowbtn;
    QPushButton *retbtn;
    QPushButton *delstaticip;
    QPushButton *setstaticip;
    QPushButton *networkupbtn;

    void setupUi(QWidget *ipset)
    {
        if (ipset->objectName().isEmpty())
            ipset->setObjectName(QString::fromUtf8("ipset"));
        ipset->resize(1024, 600);
        modstaticip = new QPushButton(ipset);
        modstaticip->setObjectName(QString::fromUtf8("modstaticip"));
        modstaticip->setGeometry(QRect(40, 390, 260, 40));
        textEdit = new QTextEdit(ipset);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(320, 120, 680, 400));
        QFont font;
        font.setPointSize(12);
        textEdit->setFont(font);
        textEdit->setReadOnly(true);
        ipshowbtn = new QPushButton(ipset);
        ipshowbtn->setObjectName(QString::fromUtf8("ipshowbtn"));
        ipshowbtn->setGeometry(QRect(40, 120, 260, 40));
        retbtn = new QPushButton(ipset);
        retbtn->setObjectName(QString::fromUtf8("retbtn"));
        retbtn->setGeometry(QRect(10, 10, 100, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/t507_button_image/return.png"), QSize(), QIcon::Normal, QIcon::Off);
        retbtn->setIcon(icon);
        delstaticip = new QPushButton(ipset);
        delstaticip->setObjectName(QString::fromUtf8("delstaticip"));
        delstaticip->setGeometry(QRect(40, 480, 260, 40));
        setstaticip = new QPushButton(ipset);
        setstaticip->setObjectName(QString::fromUtf8("setstaticip"));
        setstaticip->setGeometry(QRect(40, 300, 260, 40));
        networkupbtn = new QPushButton(ipset);
        networkupbtn->setObjectName(QString::fromUtf8("networkupbtn"));
        networkupbtn->setGeometry(QRect(40, 210, 260, 40));

        retranslateUi(ipset);

        QMetaObject::connectSlotsByName(ipset);
    } // setupUi

    void retranslateUi(QWidget *ipset)
    {
        ipset->setWindowTitle(QApplication::translate("ipset", "Form", nullptr));
        modstaticip->setText(QApplication::translate("ipset", "modify static ip", nullptr));
        ipshowbtn->setText(QApplication::translate("ipset", "show all ip address", nullptr));
        retbtn->setText(QString());
        delstaticip->setText(QApplication::translate("ipset", "delete static ip", nullptr));
        setstaticip->setText(QApplication::translate("ipset", "set static ip", nullptr));
        networkupbtn->setText(QApplication::translate("ipset", "set network up", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ipset: public Ui_ipset {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IPSET_H
