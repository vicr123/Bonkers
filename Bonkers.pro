#-------------------------------------------------
#
# Project created by QtCreator 2019-02-20T14:59:13
#
#-------------------------------------------------

QT       += core gui network
SHARE_APP_NAME = bonkers

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bonkers
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        crasheddialog.cpp \
    vicr123filebugwidget/totpcoderequest.cpp \
    vicr123filebugwidget/vicr123filebugwidget.cpp

HEADERS += \
        crasheddialog.h \
    vicr123filebugwidget/totpcoderequest.h \
    vicr123filebugwidget/vicr123filebugwidget.h

FORMS += \
        crasheddialog.ui \
    vicr123filebugwidget/totpcoderequest.ui \
    vicr123filebugwidget/vicr123filebugwidget.ui

unix:!macx {
    # Include the-libs build tools
    include(/usr/share/the-libs/pri/buildmaster.pri)

    QT += thelib

    target.path = /usr/bin

    INSTALLS += target cols
}

win32 {
    # Include the-libs build tools
    include(C:/Program Files/thelibs/pri/buildmaster.pri)

    QT += thelib
    INCLUDEPATH += "C:/Program Files/thelibs/include"
    LIBS += -L"C:/Program Files/thelibs/lib" -lthe-libs
}

macx {
    # Bonkers should not be built on macOS
    error(Bonkers should not be built on macOS because the inbuilt reporting dialog is good enough)
}


RESOURCES += \
    resources.qrc
