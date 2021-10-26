/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QSplitter *splitter_9;
    QPushButton *vf;
    QLabel *label_33;
    QSplitter *splitter_12;
    QPushButton *g4;
    QLabel *label_39;
    QSplitter *splitter_13;
    QPushButton *keytest;
    QLabel *label_40;
    QSplitter *splitter;
    QPushButton *auto_2;
    QLabel *label_key_6;
    QSplitter *splitter_4;
    QPushButton *ethernet;
    QLabel *label_37;
    QSplitter *splitter_8;
    QPushButton *serialport;
    QLabel *label_32;
    QSplitter *splitter_5;
    QPushButton *wifi;
    QLabel *label_35;
    QSplitter *splitter_2;
    QPushButton *bluetooth;
    QLabel *label_36;
    QSplitter *splitter_3;
    QPushButton *rtc;
    QLabel *label_29;
    QSplitter *splitter_11;
    QPushButton *udev;
    QLabel *label_38;
    QSplitter *splitter_7;
    QPushButton *gpio;
    QLabel *label_31;
    QSplitter *splitter_6;
    QPushButton *backlight;
    QLabel *label_30;
    QLabel *label_34;
    QPushButton *touchca;
    QLabel *label;
    QSplitter *splitter_10;
    QPushButton *pushButton;
    QLabel *label_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 600);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        splitter_9 = new QSplitter(centralWidget);
        splitter_9->setObjectName(QString::fromUtf8("splitter_9"));
        splitter_9->setGeometry(QRect(765, 200, 250, 75));
        splitter_9->setOrientation(Qt::Vertical);
        vf = new QPushButton(splitter_9);
        vf->setObjectName(QString::fromUtf8("vf"));
        splitter_9->addWidget(vf);
        label_33 = new QLabel(splitter_9);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setMinimumSize(QSize(0, 0));
        label_33->setAlignment(Qt::AlignCenter);
        splitter_9->addWidget(label_33);
        splitter_12 = new QSplitter(centralWidget);
        splitter_12->setObjectName(QString::fromUtf8("splitter_12"));
        splitter_12->setGeometry(QRect(513, 310, 250, 75));
        splitter_12->setOrientation(Qt::Vertical);
        g4 = new QPushButton(splitter_12);
        g4->setObjectName(QString::fromUtf8("g4"));
        splitter_12->addWidget(g4);
        label_39 = new QLabel(splitter_12);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setMinimumSize(QSize(0, 0));
        label_39->setAlignment(Qt::AlignCenter);
        splitter_12->addWidget(label_39);
        splitter_13 = new QSplitter(centralWidget);
        splitter_13->setObjectName(QString::fromUtf8("splitter_13"));
        splitter_13->setGeometry(QRect(765, 310, 250, 75));
        splitter_13->setOrientation(Qt::Vertical);
        keytest = new QPushButton(splitter_13);
        keytest->setObjectName(QString::fromUtf8("keytest"));
        splitter_13->addWidget(keytest);
        label_40 = new QLabel(splitter_13);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setMinimumSize(QSize(0, 0));
        label_40->setAlignment(Qt::AlignCenter);
        splitter_13->addWidget(label_40);
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(641, 410, 250, 75));
        splitter->setOrientation(Qt::Vertical);
        auto_2 = new QPushButton(splitter);
        auto_2->setObjectName(QString::fromUtf8("auto_2"));
        auto_2->setEnabled(true);
        splitter->addWidget(auto_2);
        label_key_6 = new QLabel(splitter);
        label_key_6->setObjectName(QString::fromUtf8("label_key_6"));
        label_key_6->setMinimumSize(QSize(0, 0));
        label_key_6->setAlignment(Qt::AlignCenter);
        splitter->addWidget(label_key_6);
        splitter_4 = new QSplitter(centralWidget);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setGeometry(QRect(512, 79, 250, 75));
        splitter_4->setMinimumSize(QSize(125, 20));
        splitter_4->setOrientation(Qt::Vertical);
        ethernet = new QPushButton(splitter_4);
        ethernet->setObjectName(QString::fromUtf8("ethernet"));
        ethernet->setMinimumSize(QSize(125, 20));
        splitter_4->addWidget(ethernet);
        label_37 = new QLabel(splitter_4);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setMinimumSize(QSize(0, 0));
        label_37->setAlignment(Qt::AlignCenter);
        splitter_4->addWidget(label_37);
        splitter_8 = new QSplitter(centralWidget);
        splitter_8->setObjectName(QString::fromUtf8("splitter_8"));
        splitter_8->setGeometry(QRect(513, 200, 250, 75));
        splitter_8->setOrientation(Qt::Vertical);
        serialport = new QPushButton(splitter_8);
        serialport->setObjectName(QString::fromUtf8("serialport"));
        splitter_8->addWidget(serialport);
        label_32 = new QLabel(splitter_8);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setMinimumSize(QSize(0, 0));
        label_32->setAlignment(Qt::AlignCenter);
        splitter_8->addWidget(label_32);
        splitter_5 = new QSplitter(centralWidget);
        splitter_5->setObjectName(QString::fromUtf8("splitter_5"));
        splitter_5->setGeometry(QRect(9, 79, 251, 75));
        splitter_5->setMinimumSize(QSize(125, 20));
        splitter_5->setOrientation(Qt::Vertical);
        wifi = new QPushButton(splitter_5);
        wifi->setObjectName(QString::fromUtf8("wifi"));
        splitter_5->addWidget(wifi);
        label_35 = new QLabel(splitter_5);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setMinimumSize(QSize(0, 0));
        label_35->setAlignment(Qt::AlignCenter);
        splitter_5->addWidget(label_35);
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setGeometry(QRect(260, 79, 250, 75));
        splitter_2->setMinimumSize(QSize(125, 20));
        splitter_2->setOrientation(Qt::Vertical);
        splitter_2->setHandleWidth(5);
        bluetooth = new QPushButton(splitter_2);
        bluetooth->setObjectName(QString::fromUtf8("bluetooth"));
        splitter_2->addWidget(bluetooth);
        label_36 = new QLabel(splitter_2);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setMinimumSize(QSize(0, 0));
        label_36->setAlignment(Qt::AlignCenter);
        splitter_2->addWidget(label_36);
        splitter_3 = new QSplitter(centralWidget);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setGeometry(QRect(764, 79, 250, 75));
        splitter_3->setMinimumSize(QSize(125, 20));
        splitter_3->setOrientation(Qt::Vertical);
        rtc = new QPushButton(splitter_3);
        rtc->setObjectName(QString::fromUtf8("rtc"));
        splitter_3->addWidget(rtc);
        label_29 = new QLabel(splitter_3);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setMinimumSize(QSize(0, 0));
        label_29->setAlignment(Qt::AlignCenter);
        splitter_3->addWidget(label_29);
        splitter_11 = new QSplitter(centralWidget);
        splitter_11->setObjectName(QString::fromUtf8("splitter_11"));
        splitter_11->setGeometry(QRect(261, 310, 250, 75));
        splitter_11->setOrientation(Qt::Vertical);
        udev = new QPushButton(splitter_11);
        udev->setObjectName(QString::fromUtf8("udev"));
        splitter_11->addWidget(udev);
        label_38 = new QLabel(splitter_11);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setMinimumSize(QSize(0, 0));
        label_38->setAlignment(Qt::AlignCenter);
        splitter_11->addWidget(label_38);
        splitter_7 = new QSplitter(centralWidget);
        splitter_7->setObjectName(QString::fromUtf8("splitter_7"));
        splitter_7->setGeometry(QRect(261, 200, 250, 75));
        splitter_7->setOrientation(Qt::Vertical);
        gpio = new QPushButton(splitter_7);
        gpio->setObjectName(QString::fromUtf8("gpio"));
        splitter_7->addWidget(gpio);
        label_31 = new QLabel(splitter_7);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setMinimumSize(QSize(0, 0));
        label_31->setAlignment(Qt::AlignCenter);
        splitter_7->addWidget(label_31);
        splitter_6 = new QSplitter(centralWidget);
        splitter_6->setObjectName(QString::fromUtf8("splitter_6"));
        splitter_6->setGeometry(QRect(10, 200, 250, 75));
        splitter_6->setOrientation(Qt::Vertical);
        backlight = new QPushButton(splitter_6);
        backlight->setObjectName(QString::fromUtf8("backlight"));
        splitter_6->addWidget(backlight);
        label_30 = new QLabel(splitter_6);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setMinimumSize(QSize(0, 0));
        label_30->setAlignment(Qt::AlignCenter);
        splitter_6->addWidget(label_30);
        label_34 = new QLabel(centralWidget);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(10, 354, 250, 31));
        label_34->setMinimumSize(QSize(125, 20));
        label_34->setAlignment(Qt::AlignCenter);
        touchca = new QPushButton(centralWidget);
        touchca->setObjectName(QString::fromUtf8("touchca"));
        touchca->setGeometry(QRect(10, 310, 250, 38));
        touchca->setMinimumSize(QSize(125, 20));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(340, 10, 341, 41));
        QFont font;
        font.setPointSize(18);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        splitter_10 = new QSplitter(centralWidget);
        splitter_10->setObjectName(QString::fromUtf8("splitter_10"));
        splitter_10->setGeometry(QRect(140, 410, 241, 81));
        splitter_10->setOrientation(Qt::Vertical);
        pushButton = new QPushButton(splitter_10);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        splitter_10->addWidget(pushButton);
        label_2 = new QLabel(splitter_10);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);
        splitter_10->addWidget(label_2);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 28));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        vf->setText(QApplication::translate("MainWindow", "8", nullptr));
        label_33->setText(QApplication::translate("MainWindow", "Voice Frequency", nullptr));
        g4->setText(QApplication::translate("MainWindow", "11", nullptr));
        label_39->setText(QApplication::translate("MainWindow", "All Interface Test", nullptr));
        keytest->setText(QApplication::translate("MainWindow", "12", nullptr));
        label_40->setText(QApplication::translate("MainWindow", "Key Test", nullptr));
        auto_2->setText(QApplication::translate("MainWindow", "14", nullptr));
        label_key_6->setText(QApplication::translate("MainWindow", "<html><head/><body><p>About Board</p></body></html>", nullptr));
        ethernet->setText(QApplication::translate("MainWindow", "3", nullptr));
        label_37->setText(QApplication::translate("MainWindow", "Ethernet", nullptr));
        serialport->setText(QApplication::translate("MainWindow", "7", nullptr));
        label_32->setText(QApplication::translate("MainWindow", "Serial Port", nullptr));
        wifi->setText(QApplication::translate("MainWindow", "1", nullptr));
        label_35->setText(QApplication::translate("MainWindow", "Wifi", nullptr));
        bluetooth->setText(QApplication::translate("MainWindow", "2", nullptr));
        label_36->setText(QApplication::translate("MainWindow", "Bluetooth", nullptr));
        rtc->setText(QApplication::translate("MainWindow", "4", nullptr));
        label_29->setText(QApplication::translate("MainWindow", "Rtc", nullptr));
        udev->setText(QApplication::translate("MainWindow", "10", nullptr));
        label_38->setText(QApplication::translate("MainWindow", "Udev Detection", nullptr));
        gpio->setText(QApplication::translate("MainWindow", "6", nullptr));
        label_31->setText(QApplication::translate("MainWindow", "Gpio", nullptr));
        backlight->setText(QApplication::translate("MainWindow", "5", nullptr));
        label_30->setText(QApplication::translate("MainWindow", "Backlight", nullptr));
        label_34->setText(QApplication::translate("MainWindow", "Touch Display", nullptr));
        touchca->setText(QApplication::translate("MainWindow", "9", nullptr));
        label->setText(QApplication::translate("MainWindow", "Main Interface", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "13", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Chiese/English", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
