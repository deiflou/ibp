#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T21:41:36
#
#-------------------------------------------------

QT += widgets

TEMPLATE = lib

TARGET = anitools.imagefilter.threshold

win32 {
    CONFIG(debug, debug|release) {
        DLLDESTDIR = ../../bin/win32/debug/plugins
        LIBS += -L../../lib/win32/imgproc/debug -lanitools.imgproc
    } else {
        DLLDESTDIR = ../../bin/win32/release/plugins
        LIBS += -L../../lib/win32/imgproc/release -lanitools.imgproc
    }
}

SOURCES += ../../src/imagefilter_threshold/filter.cpp \
           ../../src/imagefilter_threshold/main.cpp \
           ../../src/imagefilter_threshold/filterwidget.cpp

HEADERS += ../../src/imagefilter_threshold/filter.h \
           ../../src/imagefilter_threshold/filterwidget.h

FORMS += ../../src/imagefilter_threshold/filterwidget.ui

