/********************************************************************************
** Form generated from reading UI file 'touchscreen.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOUCHSCREEN_H
#define UI_TOUCHSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_touchscreen
{
public:

    void setupUi(QWidget *touchscreen)
    {
        if (touchscreen->objectName().isEmpty())
            touchscreen->setObjectName(QString::fromUtf8("touchscreen"));
        touchscreen->resize(400, 300);

        retranslateUi(touchscreen);

        QMetaObject::connectSlotsByName(touchscreen);
    } // setupUi

    void retranslateUi(QWidget *touchscreen)
    {
        touchscreen->setWindowTitle(QApplication::translate("touchscreen", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class touchscreen: public Ui_touchscreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOUCHSCREEN_H
