#-------------------------------------------------
#
# Project created by QtCreator 2014-03-07T22:21:05
#
#-------------------------------------------------

QT += widgets

TEMPLATE = lib

TARGET = anitools.misc

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = ../../lib/win32/misc/debug
        DLLDESTDIR = ../../bin/win32/debug
    } else {
        DESTDIR = ../../lib/win32/misc/release
        DLLDESTDIR = ../../bin/win32/release
    }
}

SOURCES += \
    ../../src/misc/nativeeventfilter.cpp \

HEADERS += \
    ../../src/misc/util.h \
    ../../src/misc/nativeeventfilter.h \
