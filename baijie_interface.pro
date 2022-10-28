#-------------------------------------------------
#
# Project created by QtCreator 2021-09-22T09:31:40
#
#-------------------------------------------------

QT       += core gui serialport sql
QT       += network
QT          += gui-private widgets
#DEFINES += QT_NO_WARNING_OUTPUT\
#     QT_NO_DEBUG_OUTPUT
TRANSLATIONS += chinese.ts
CONFIG += C++11
#DEFINES += QT_NO_WARNING_OUTPUT\
#     QT_NO_DEBUG_OUTPUT


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelperAPP
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
    gpio.cpp \
    touchscreen.cpp \
    ctouchbutton.cpp \
    timeset.cpp \
    globalapp.cpp \
    boardinfo_interface.cpp \
    about_board.cpp \
    ipset_interface.cpp \
    gpio_interface.cpp \
    bluetooth.cpp \
    bluetooth_thread.cpp \
    user_manual.cpp \
    serial.cpp \
    serial_thread.cpp \
    sys_setting.cpp \
    wifi_thread.cpp \
    wifi_bt_interface.cpp \
    switchcontrol.cpp \
    wifi.cpp \
    wificondialog.cpp \
    sim_module.cpp \
    serialdialog.cpp \
    all_test_thread.cpp \
    all_test_interface.cpp \
    all_test.cpp \
    ipset.cpp \
    database.cpp \
    myslider.cpp \
    voice_savefile_name.cpp \
    voice_rename.cpp \
    file_opration.cpp \
    backlight.cpp

HEADERS += \
        mainwindow.h \
    voice.h \
    udev.h \
    gpio.h \
    touchscreen.h \
    ctouchbutton.h \
    timeset.h \
    gpio_interface.h \
    globalapp.h \
    about_board.h \
    boardinfo_interface.h \
    ipset_interface.h \
    bluetooth.h \
    bluetooth_thread.h \
    user_manual.h \
    serial.h \
    serial_thread.h \
    sys_setting.h \
    wifi_thread.h \
    wifi_bt_interface.h \
    switchcontrol.h \
    wifi.h \
    wificondialog.h \
    sim_module.h \
    serialdialog.h \
    all_test_thread.h \
    all_test_interface.h \
    all_test.h \
    ipset.h \
    database.h \
    myslider.h \
    voice_savefile_name.h \
    voice_rename.h \
    file_opration.h \
    backlight.h

FORMS += \
        mainwindow.ui \
    voice.ui \
    udev.ui \
    gpio.ui \
    touchscreen.ui \
    timeset.ui \
    about_board.ui \
    bluetooth.ui \
    user_manual.ui \
    serial.ui \
    sys_setting.ui \
    wifi.ui \
    wificondialog.ui \
    sim_module.ui \
    serialdialog.ui \
    all_test.ui \
    ipset.ui \
    voice_savefile_name.ui \
    voice_rename.ui \
    file_opration.ui \
    backlight.ui

RESOURCES += \
    translation.qrc

DISTFILES += \
    button_image/all/home.jpg \
    button_image/all/home.jpg \
    button_image/all/back1.jpg \
    button_image/disconnect.svg \
    button_image/connect.svg \
    button_image/light.svg \
    button_image/voice/stop.png \
    button_image/voice/stop.png \
    button_image/蓝牙.svg \
    button_image/d_ip.svg \
    button_image/sync.svg \
    button_image/all/background(2).png \
    button_image/serial.svg \
    button_image/system.svg
