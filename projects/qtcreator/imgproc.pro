#-------------------------------------------------
#
# Project created by QtCreator 2014-03-07T22:21:05
#
#-------------------------------------------------

QT += widgets

TEMPLATE = lib

TARGET = anitools.imgproc

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = ../../lib/win32/imgproc/debug
        DLLDESTDIR = ../../bin/win32/debug
        LIBS += -L../../lib/win32/plugins/debug -lanitools.plugins
    } else {
        DESTDIR = ../../lib/win32/imgproc/release
        DLLDESTDIR = ../../bin/win32/release
        LIBS += -L../../lib/win32/plugins/release -lanitools.plugins
    }
}

LIBS += -lFreeImage -llcms2

SOURCES += \
    ../../src/imgproc/colorconversion.cpp \
    ../../src/imgproc/freeimage.cpp \
    ../../src/imgproc/imagefilterlist.cpp \
    ../../src/imgproc/lut01.cpp \
    ../../src/imgproc/lut02.cpp \
    ../../src/imgproc/lut03.cpp \
    ../../src/imgproc/util.cpp

HEADERS += \
    ../../src/imgproc/colorconversion.h \
    ../../src/imgproc/freeimage.h \
    ../../src/imgproc/imagefilter.h \
    ../../src/imgproc/imagefilterlist.h \
    ../../src/imgproc/lut.h \
    ../../src/imgproc/types.h \
    ../../src/imgproc/util.h
