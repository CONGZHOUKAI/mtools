QT       += serialport core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += PROGRAM_NAME="\\\"XXX\\\""

DEFINES += VERSION_MAJOR=1 VERSION_MINOR=0 VERSION_PATCH=0 VERSION_STRING=\\\"1.0.0\\\"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstractreader.cpp \
    datatextview.cpp \
    demoreader.cpp \
    form.cpp \
    ledwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    portcontrol.cpp \
    portlist.cpp \
    sink.cpp \
    source.cpp

HEADERS += \
    abstractreader.h \
    datatextview.h \
    demoreader.h \
    form.h \
    ledwidget.h \
    mainwindow.h \
    portcontrol.h \
    portlist.h \
    setting_defines.h \
    sink.h \
    source.h \
    utils.h

FORMS += \
    about_dialog.ui \
    datatextview.ui \
    form.ui \
    mainwindow.ui \
    portcontrol.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    misc/icons.qrc

DISTFILES +=
