#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T21:41:36
#
#-------------------------------------------------

QT       += widgets
TEMPLATE = lib

TARGET = anitools.imagefilter.illuminationcorrection

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

LIBS += -lopencv_core300 -lopencv_imgproc300

INCLUDEPATH += ../../src/widgets

SOURCES += ../../src/imagefilter_illuminationcorrection/filter.cpp \
           ../../src/imagefilter_illuminationcorrection/main.cpp \
           ../../src/imagefilter_illuminationcorrection/filterwidget.cpp

HEADERS += ../../src/imagefilter_illuminationcorrection/filter.h \
           ../../src/imagefilter_illuminationcorrection/filterwidget.h

FORMS += ../../src/imagefilter_illuminationcorrection/filterwidget.ui

