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
    src/ftdi.cpp \
    src/tcscom.cpp \
    src/bootloader.cpp \
    src/pb_encode.c \
    src/pb_decode.c

HEADERS  += inc/mainwindow.h \
    inc/ftdi.h \
    inc/structs.h \
    inc/tcscom.h \
    inc/bootloader.h \
    inc/pb_encode.h \
    inc/pb_decode.h \
    inc/pb.h

FORMS    += ui/mainwindow.ui

INCLUDEPATH += inc
LIBS += -L/usr/local/lib -lftd2xx
