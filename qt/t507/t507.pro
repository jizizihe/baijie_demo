#-------------------------------------------------
#
# Project created by QtCreator 2021-09-22T09:31:40
#
#-------------------------------------------------

QT       += core gui
QT       += network
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
    interface_gpio.cpp \
    gpio.cpp \
    touchscreen.cpp \
    ctouchbutton.cpp \
    timeset.cpp \
<<<<<<< HEAD
    wifi.cpp \
    wificonnect.cpp \
    hotspot.cpp
=======
    gpio_interface.c \
    eth0set.cpp \
    speedtest.c \
    eth0_popup.cpp
>>>>>>> ly

HEADERS += \
        mainwindow.h \
    voice.h \
    udev.h \
    backlight.h \
    interface_gpio.h \
    gpio_test.h \
    gpio.h \
    touchscreen.h \
    ctouchbutton.h \
    timeset.h \
<<<<<<< HEAD
    wifi.h \
    wificonnect.h \
    hotspot.h
=======
    gpio_interface.h \
    eth0set.h \
    speedtest.h \
    eth0_popup.h
>>>>>>> ly

FORMS += \
        mainwindow.ui \
    voice.ui \
    udev.ui \
    backlight.ui \
    gpio.ui \
    touchscreen.ui \
    timeset.ui \
<<<<<<< HEAD
    wifi.ui \
    wificonnect.ui \
    hotspot.ui
=======
    eth0set.ui
>>>>>>> ly
