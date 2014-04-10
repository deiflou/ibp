#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T21:41:36
#
#-------------------------------------------------

QT       += widgets
TEMPLATE = lib

TARGET = anitools.imagefilter.gaussianblur

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

SOURCES += ../../src/imagefilter_gaussianblur/filter.cpp \
           ../../src/imagefilter_gaussianblur/main.cpp \
           ../../src/imagefilter_gaussianblur/filterwidget.cpp

HEADERS += ../../src/imagefilter_gaussianblur/filter.h \
           ../../src/imagefilter_gaussianblur/filterwidget.h

FORMS += ../../src/imagefilter_gaussianblur/filterwidget.ui

