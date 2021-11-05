#-------------------------------------------------
#
# Project created by QtCreator 2021-09-22T09:31:40
#
#-------------------------------------------------

QT       += core gui serialport
QT       += network
#DEFINES += QT_NO_WARNING_OUTPUT\
#     QT_NO_DEBUG_OUTPUT
TRANSLATIONS += chinese.ts

#DEFINES += QT_NO_WARNING_OUTPUT\
#     QT_NO_DEBUG_OUTPUT


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

target.path = /
INSTALLS += target
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#DEFINES += QT_NO_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    voice.cpp \
    udev.cpp \
    backlight.cpp \
    gpio.cpp \
    touchscreen.cpp \
    ctouchbutton.cpp \
    timeset.cpp \
    wifi.cpp \
    wificonnect.cpp \
    hotspot.cpp \
    globalapp.cpp \
    boardinfo_interface.cpp \
    keyBoard.cpp \
    about_board.cpp \
    keytest.cpp \
    keythread.cpp \
    ipset_popup.cpp \
    ipset.cpp \
    ipset_interface.cpp \
    gpio_interface.cpp \
    all_interface_test.cpp \
    udev_interface.cpp\
    bluetooth.cpp \
    bluetooth_thread.cpp \
    user_manual.cpp \
    serial.cpp

HEADERS += \
        mainwindow.h \
    voice.h \
    udev.h \
    backlight.h \
    gpio.h \
    touchscreen.h \
    ctouchbutton.h \
    timeset.h \
    wifi.h \
    wificonnect.h \
    hotspot.h \
    gpio_interface.h \
    globalapp.h \
    keyBoard.h \
    about_board.h \
    boardinfo_interface.h \
    keytest.h \
    keythread.h \
    ipset_popup.h \
    ipset.h \
    ipset_interface.h \
    all_interface_test.h \
    udev_interface.h \
    bluetooth.h \
    bluetooth_thread.h \
    user_manual.h \
    serial.h

FORMS += \
        mainwindow.ui \
    voice.ui \
    udev.ui \
    backlight.ui \
    gpio.ui \
    touchscreen.ui \
    timeset.ui \
    wifi.ui \
    wificonnect.ui \
    hotspot.ui \
    about_board.ui \
    keytest.ui \
    ipset.ui \
    all_interface_test.ui \
    bluetooth.ui \
    user_manual.ui \
    serial.ui

RESOURCES += \
    translation.qrc
