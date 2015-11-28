#-------------------------------------------------
#
# Project created by QtCreator 2014-03-24T20:33:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(third_party/common/common.pri)
include(third_party/svgbutton/svgbutton.pri)
include(third_party/svgtoggleswitch/svgtoggleswitch.pri)
include (third_party/svgdialgauge/svgdialgauge.pri)
include (third_party/multislider/multislider.pri)

TARGET = ea3um
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    banner.cpp \
    gpio.cpp \
    mythread.cpp \
    dialogstatus.cpp \
    tonein.cpp \
    changepasswd.cpp \
    smeter.cpp

HEADERS  += mainwindow.h \
    banner.h \
    gpio.h \
    mythread.h \
    dialogstatus.h \
    tonein.h \
    changepasswd.h \
    smeter.h

FORMS    += mainwindow.ui \
    dialogstatus.ui \
    tonein.ui \
    changepasswd.ui \
    smeter.ui


linux-* {
    target.path = ./
    INSTALLS += target
}

RESOURCES +=  third_party/skins/beryl_multislider.qrc          \
             third_party/skins/beryl_svgslideswitch.qrc       \
             third_party/skins/beryl_scrolldial.qrc           \
             third_party/skins/beryl_5waybutton.qrc           \
             third_party/skins/beryl_scrollwheel.qrc          \
             third_party/skins/beryl_svgtoggleswitch.qrc      \
             third_party/skins/beryl_svgbutton.qrc            \
             third_party/skins/berylsquare_svgbutton.qrc      \
             third_party/skins/berylsquare_svgtoggleswitch.qrc\
             third_party/skins/amperemeter_svgdialgauge.qrc   \
             third_party/skins/thermometer_svgdialgauge.qrc   \
             third_party/skins/tachometer_svgdialgauge.qrc \
    images.qrc
OTHER_FILES +=


