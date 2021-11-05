/********************************************************************************
** Form generated from reading UI file 'voice.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOICE_H
#define UI_VOICE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_voice
{
public:
    QWidget *centralwidget;
    QPushButton *return_2;
    QWidget *function;
    QPushButton *play;
    QPushButton *begin;
    QPushButton *choose_3;
    QWidget *widget;
    QLabel *pathname_2;
    QLabel *label_2;
    QComboBox *combox;
    QLabel *label_3;
    QLabel *time;
    QLabel *label_6;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_5;
    QSlider *horizontalSlider;
    QLabel *label_7;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *voice)
    {
        if (voice->objectName().isEmpty())
            voice->setObjectName(QString::fromUtf8("voice"));
        voice->resize(1024, 600);
        centralwidget = new QWidget(voice);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        return_2 = new QPushButton(centralwidget);
        return_2->setObjectName(QString::fromUtf8("return_2"));
        return_2->setGeometry(QRect(10, 10, 100, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/t507_button_image/return.png"), QSize(), QIcon::Normal, QIcon::On);
        return_2->setIcon(icon);
        function = new QWidget(centralwidget);
        function->setObjectName(QString::fromUtf8("function"));
        function->setGeometry(QRect(50, 180, 241, 211));
        play = new QPushButton(function);
        play->setObjectName(QString::fromUtf8("play"));
        play->setGeometry(QRect(9, 170, 221, 31));
        begin = new QPushButton(function);
        begin->setObjectName(QString::fromUtf8("begin"));
        begin->setGeometry(QRect(9, 90, 221, 31));
        choose_3 = new QPushButton(function);
        choose_3->setObjectName(QString::fromUtf8("choose_3"));
        choose_3->setGeometry(QRect(9, 0, 221, 31));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(330, 180, 641, 211));
        pathname_2 = new QLabel(widget);
        pathname_2->setObjectName(QString::fromUtf8("pathname_2"));
        pathname_2->setGeometry(QRect(140, 0, 491, 41));
        QFont font;
        font.setPointSize(13);
        pathname_2->setFont(font);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 0, 131, 41));
        QFont font1;
        font1.setPointSize(11);
        label_2->setFont(font1);
        combox = new QComboBox(widget);
        combox->setObjectName(QString::fromUtf8("combox"));
        combox->setGeometry(QRect(140, 83, 501, 41));
        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 83, 131, 41));
        label_3->setFont(font1);
        time = new QLabel(widget);
        time->setObjectName(QString::fromUtf8("time"));
        time->setGeometry(QRect(138, 160, 500, 41));
        time->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(0, 160, 131, 41));
        label_6->setFont(font1);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 110, 211, 31));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(330, 110, 101, 31));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(290, 10, 441, 51));
        QFont font2;
        font2.setPointSize(18);
        label_5->setFont(font2);
        label_5->setAlignment(Qt::AlignCenter);
        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(330, 420, 641, 41));
        horizontalSlider->setOrientation(Qt::Horizontal);
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(140, 430, 81, 30));
        label_7->setFont(font1);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        voice->setCentralWidget(centralwidget);
        menubar = new QMenuBar(voice);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1024, 23));
        voice->setMenuBar(menubar);
        statusbar = new QStatusBar(voice);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        voice->setStatusBar(statusbar);

        retranslateUi(voice);

        QMetaObject::connectSlotsByName(voice);
    } // setupUi

    void retranslateUi(QMainWindow *voice)
    {
        voice->setWindowTitle(QApplication::translate("voice", "MainWindow", nullptr));
        return_2->setText(QString());
        play->setText(QApplication::translate("voice", "play", nullptr));
        begin->setText(QApplication::translate("voice", "begin", nullptr));
        choose_3->setText(QApplication::translate("voice", "choose", nullptr));
        pathname_2->setText(QString());
        label_2->setText(QApplication::translate("voice", "file_path:", nullptr));
        label_3->setText(QApplication::translate("voice", "file_name:", nullptr));
        time->setText(QString());
        label_6->setText(QApplication::translate("voice", "time:", nullptr));
        label->setText(QApplication::translate("voice", "FUNCTION:", nullptr));
        label_4->setText(QApplication::translate("voice", "FILE:", nullptr));
        label_5->setText(QApplication::translate("voice", "The recording setting", nullptr));
        label_7->setText(QApplication::translate("voice", "sound:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class voice: public Ui_voice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOICE_H
