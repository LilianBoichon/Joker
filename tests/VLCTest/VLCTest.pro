TEMPLATE = app
TARGET = VLCTest
DEPENDPATH += .
QT += widgets core gui
INCLUDEPATH += .

# Input
HEADERS += player.h
SOURCES += main.cpp player.cpp

DESTDIR = ./


# Windows specific
win32{
}

# Ubuntu specific
linux {
	VLC_PATH = /usr/lib/vlc
}

# MacOS specific
mac {
	VLC_PATH = /Applications/VLC.app/Contents/MacOS/
}

INCLUDEPATH += $${VLC_PATH}/include
DEPENDPATH += $${VLC_PATH}/include
LIBS += -L$${VLC_PATH}/lib/ -lvlc

QMAKE_POST_LINK += mkdir -p ./$${TARGET}.app/Contents/MacOS/lib
QMAKE_POST_LINK += && mkdir -p ./$${TARGET}.app/Contents/MacOS/plugins
QMAKE_POST_LINK += && cp -r $${VLC_PATH}/lib/* ./$${TARGET}.app/Contents/MacOS/lib
QMAKE_POST_LINK += && cp -r $${VLC_PATH}/plugins/* ./$${TARGET}.app/Contents/MacOS/plugins


