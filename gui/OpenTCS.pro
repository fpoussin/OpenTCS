#-------------------------------------------------
#
# Project created by QtCreator 2013-09-24T17:28:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenTCS
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/device.cpp

HEADERS  += inc/mainwindow.h \
    inc/device.h

FORMS    += ui/mainwindow.ui

INCLUDEPATH += inc
LIBS += -L/usr/local/lib -lftd2xx
