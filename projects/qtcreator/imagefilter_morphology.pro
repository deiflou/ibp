#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T21:41:36
#
#-------------------------------------------------

QT       += widgets
TEMPLATE = lib

TARGET = anitools.imagefilter.morphology

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

SOURCES += ../../src/imagefilter_morphology/filter.cpp \
           ../../src/imagefilter_morphology/main.cpp \
           ../../src/imagefilter_morphology/filterwidget.cpp

HEADERS += ../../src/imagefilter_morphology/filter.h \
           ../../src/imagefilter_morphology/filterwidget.h

FORMS += ../../src/imagefilter_morphology/filterwidget.ui

