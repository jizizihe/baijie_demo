/********************************************************************************
** Form generated from reading UI file 'backlight.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BACKLIGHT_H
#define UI_BACKLIGHT_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_backlight
{
public:
    QWidget *centralwidget;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox;
    QPushButton *return_2;
    QFrame *frame;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSlider *horizontalSlider;
    QPushButton *normal;
    QLabel *label_3;
    QLabel *time;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *backlight)
    {
        if (backlight->objectName().isEmpty())
            backlight->setObjectName(QString::fromUtf8("backlight"));
        backlight->resize(1024, 600);
        centralwidget = new QWidget(backlight);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(100, 370, 800, 70));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        comboBox = new QComboBox(frame_2);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_2->addWidget(comboBox);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        return_2 = new QPushButton(centralwidget);
        return_2->setObjectName(QString::fromUtf8("return_2"));
        return_2->setGeometry(QRect(10, 10, 100, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/t507_button_image/return.png"), QSize(), QIcon::Normal, QIcon::On);
        return_2->setIcon(icon);
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(100, 210, 800, 70));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        horizontalSlider = new QSlider(frame);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setTickPosition(QSlider::NoTicks);

        horizontalLayout->addWidget(horizontalSlider);

        normal = new QPushButton(frame);
        normal->setObjectName(QString::fromUtf8("normal"));

        horizontalLayout->addWidget(normal);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(130, 20, 771, 51));
        QFont font;
        font.setPointSize(18);
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);
        time = new QLabel(centralwidget);
        time->setObjectName(QString::fromUtf8("time"));
        time->setGeometry(QRect(300, 140, 421, 41));
        time->setAlignment(Qt::AlignCenter);
        backlight->setCentralWidget(centralwidget);
        menubar = new QMenuBar(backlight);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1024, 23));
        backlight->setMenuBar(menubar);
        statusbar = new QStatusBar(backlight);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        backlight->setStatusBar(statusbar);

        retranslateUi(backlight);

        QMetaObject::connectSlotsByName(backlight);
    } // setupUi

    void retranslateUi(QMainWindow *backlight)
    {
        backlight->setWindowTitle(QApplication::translate("backlight", "MainWindow", nullptr));
        label->setText(QApplication::translate("backlight", "Sleep time", nullptr));
        comboBox->setItemText(0, QApplication::translate("backlight", "15s", nullptr));
        comboBox->setItemText(1, QApplication::translate("backlight", "30s", nullptr));
        comboBox->setItemText(2, QApplication::translate("backlight", "1min", nullptr));
        comboBox->setItemText(3, QApplication::translate("backlight", "2min", nullptr));
        comboBox->setItemText(4, QApplication::translate("backlight", "5min", nullptr));
        comboBox->setItemText(5, QApplication::translate("backlight", "10min", nullptr));
        comboBox->setItemText(6, QApplication::translate("backlight", "never", nullptr));

        return_2->setText(QString());
        label_2->setText(QApplication::translate("backlight", "brightness", nullptr));
        normal->setText(QApplication::translate("backlight", "normal", nullptr));
        label_3->setText(QApplication::translate("backlight", "Backlight  Settings", nullptr));
        time->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class backlight: public Ui_backlight {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BACKLIGHT_H
