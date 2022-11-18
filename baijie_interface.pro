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

TARGET = BaijieDemo
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
    timeset.cpp \
    about_board.cpp \
    ipset_interface.cpp \
    gpio_interface.cpp \
    bluetooth.cpp \
    bluetooth_thread.cpp \
    user_manual.cpp \
    serial.cpp \
    serial_thread.cpp \
    sys_setting.cpp \
    wifi.cpp \
    all_test_thread.cpp \
    all_test_interface.cpp \
    all_test.cpp \
    ipset.cpp \
    database.cpp \
    voice_savefile_name.cpp \
    voice_rename.cpp \
    file_opration.cpp \
    backlight.cpp \
    wifi_thread.cpp \
    bluetooth_interface.cpp \
    sim_interface.cpp \
    sim_thread.cpp \
    wifi_interface.cpp \
    sim.cpp \
    about_board_interface.cpp \
    common_slider_press.cpp \
    common_switch.cpp \
    touch_screen_button.cpp \
    touch_screen.cpp \
    wifi_condialog.cpp \
    main_touch.cpp \
    desktop.cpp \
    language_selection.cpp \
    serial_dialog.cpp

HEADERS += \
        mainwindow.h \
    voice.h \
    udev.h \
    gpio.h \
    timeset.h \
    gpio_interface.h \
    about_board.h \
    ipset_interface.h \
    bluetooth.h \
    bluetooth_thread.h \
    user_manual.h \
    serial.h \
    serial_thread.h \
    sys_setting.h \
    wifi.h \
    all_test_thread.h \
    all_test_interface.h \
    all_test.h \
    ipset.h \
    database.h \
    voice_savefile_name.h \
    voice_rename.h \
    file_opration.h \
    backlight.h \
    wifi_thread.h \
    bluetooth_interface.h \
    sim_interface.h \
    sim_thread.h \
    wifi_interface.h \
    sim.h \
    about_board_interface.h \
    common_slider_press.h \
    common_switch.h \
    touch_screen_button.h \
    touch_screen.h \
    wifi_condialog.h \
    serial_dialog.h \
    main_touch.h \
    desktop.h \
    language_selection.h

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
    all_test.ui \
    ipset.ui \
    voice_savefile_name.ui \
    voice_rename.ui \
    file_opration.ui \
    backlight.ui \
    desktop.ui \
    language_selection.ui \
    serial_dialog.ui

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
    button_image/system.svg \
    button_image/tel.svg
