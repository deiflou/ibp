#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T21:41:36
#
#-------------------------------------------------

QT       += widgets
TEMPLATE = lib

TARGET = anitools.imagefilter.colorlayer

win32 {
    CONFIG(debug, debug|release) {
        DLLDESTDIR = ../../bin/win32/debug/plugins
        LIBS += -L../../lib/win32/imgproc/debug -lanitools.imgproc
        LIBS += -L../../lib/win32/widgets/debug -lanitools.widgets
    } else {
        DLLDESTDIR = ../../bin/win32/release/plugins
        LIBS += -L../../lib/win32/imgproc/release -lanitools.imgproc
        LIBS += -L../../lib/win32/widgets/release -lanitools.widgets
    }
}

INCLUDEPATH += ../../src/widgets/

SOURCES += ../../src/imagefilter_colorlayer/filter.cpp \
           ../../src/imagefilter_colorlayer/main.cpp \
           ../../src/imagefilter_colorlayer/filterwidget.cpp

HEADERS += ../../src/imagefilter_colorlayer/filter.h \
           ../../src/imagefilter_colorlayer/filterwidget.h

FORMS += ../../src/imagefilter_colorlayer/filterwidget.ui

