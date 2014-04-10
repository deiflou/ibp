#-------------------------------------------------
#
# Project created by QtCreator 2014-03-07T22:21:05
#
#-------------------------------------------------

QT += widgets

TEMPLATE = lib

TARGET = anitools.plugins

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = ../../lib/win32/plugins/debug
        DLLDESTDIR = ../../bin/win32/debug
    } else {
        DESTDIR = ../../lib/win32/plugins/release
        DLLDESTDIR = ../../bin/win32/release
    }
}

SOURCES += \
    ../../src/plugins/imagefilterpluginloader.cpp

HEADERS += \
    ../../src/plugins/imagefilterpluginloader.h

