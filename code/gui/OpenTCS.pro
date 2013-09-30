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
    ../common/src/pb_encode.c \
    ../common/src/pb_decode.c \
    ../common/src/nanopb.pb.c \
    ../common/src/messages.pb.c

HEADERS  += inc/mainwindow.h \
    inc/ftdi.h \
    inc/tcscom.h \
    inc/bootloader.h \
    ../common/inc/pb_encode.h \
    ../common/inc/pb_decode.h \
    ../common/inc/pb.h \
    ../common/inc/nanopb.pb.h \
    ../common/inc/messages.pb.h \
    inc/ftd2xx.h \
    inc/compat.h

FORMS    += ui/mainwindow.ui

INCLUDEPATH += inc ../common/inc

unix {
    LIBS += -L/usr/local/lib -lftd2xx
}
 win32 {
     CONFIG += console
     DEFINES += FTD2XX_EXPORTS
     INCLUDEPATH += inc/win32
     CONFIG += embed_manifest_exe
 }
