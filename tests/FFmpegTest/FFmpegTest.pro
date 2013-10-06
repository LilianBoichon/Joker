#-------------------------------------------------
#
# Project created by QtCreator 2013-10-01T16:48:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FFmpegTest
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lavformat -lavcodec -lavutil

LIBS += -lxvidcore -lx264 -lvorbis -lvorbisenc -lvorbisfile -lvpx
LIBS += -ltheora -ltheoradec -ltheoraenc
LIBS += -lspeex -lspeexdsp
LIBS += -lschroedinger-1.0 -lopus -lvo-aacenc -lopenjpeg -lmp3lame -lfaac -lcelt0 -lfdk-aac
LIBS += -lswscale -laacplus
#to test
LIBS += -lass -ltiff -ltiffxx -lpng -ljpeg

LIBS += -lssl -lcrypto -lbz2 -lz
LIBS += -liconv