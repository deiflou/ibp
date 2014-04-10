#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T21:41:36
#
#-------------------------------------------------

QT       += widgets
TEMPLATE = lib

TARGET = anitools.imagefilter.median

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

SOURCES += ../../src/imagefilter_median/filter.cpp \
           ../../src/imagefilter_median/main.cpp \
           ../../src/imagefilter_median/filterwidget.cpp

HEADERS += ../../src/imagefilter_median/filter.h \
           ../../src/imagefilter_median/filterwidget.h

FORMS += ../../src/imagefilter_median/filterwidget.ui

