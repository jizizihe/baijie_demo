/********************************************************************************
** Form generated from reading UI file 'gpio.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GPIO_H
#define UI_GPIO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gpio
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineedit1_1;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *gpio)
    {
        if (gpio->objectName().isEmpty())
            gpio->setObjectName(QString::fromUtf8("gpio"));
        gpio->resize(535, 256);
        gridLayout = new QGridLayout(gpio);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineedit1_1 = new QLineEdit(gpio);
        lineedit1_1->setObjectName(QString::fromUtf8("lineedit1_1"));

        gridLayout->addWidget(lineedit1_1, 1, 1, 1, 1);

        label = new QLabel(gpio);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        label_2 = new QLabel(gpio);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 5, 1, 1, 1);

        pushButton = new QPushButton(gpio);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 4, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 2, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 3, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);


        retranslateUi(gpio);

        QMetaObject::connectSlotsByName(gpio);
    } // setupUi

    void retranslateUi(QWidget *gpio)
    {
        gpio->setWindowTitle(QApplication::translate("gpio", "Form", nullptr));
        lineedit1_1->setText(QString());
        label->setText(QApplication::translate("gpio", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">plz iput GPIO</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("gpio", "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt; font-weight:600; color:#ff0000;\">For example: If you want to check GPIOG13 ,please input &quot;g13&quot;</span></p></body></html>", nullptr));
        pushButton->setText(QApplication::translate("gpio", "ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class gpio: public Ui_gpio {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GPIO_H
