#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T21:41:36
#
#-------------------------------------------------

QT       += widgets
TEMPLATE = lib

TARGET = anitools.imagefilter.autothreshold

win32 {
    CONFIG(debug, debug|release) {
        DLLDESTDIR = ../../bin/win32/debug/plugins
        LIBS += -L../../lib/win32/imgproc/debug -lanitools.imgproc
    } else {
        DLLDESTDIR = ../../bin/win32/release/plugins
        LIBS += -L../../lib/win32/imgproc/release -lanitools.imgproc
    }

}

LIBS += -lopencv_core300 -lopencv_imgproc300

SOURCES += ../../src/imagefilter_autothreshold/filter.cpp \
           ../../src/imagefilter_autothreshold/main.cpp \
           ../../src/imagefilter_autothreshold/filterwidget.cpp

HEADERS += ../../src/imagefilter_autothreshold/filter.h \
           ../../src/imagefilter_autothreshold/filterwidget.h

FORMS += ../../src/imagefilter_autothreshold/filterwidget.ui

