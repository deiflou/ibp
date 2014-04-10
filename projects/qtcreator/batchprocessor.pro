#-------------------------------------------------
#
# Project created by QtCreator 2014-03-13T12:33:53
#
#-------------------------------------------------

QT += widgets

TARGET = anitools.batchprocessor
TEMPLATE = app

win32 {
    QT += winextras
    DEFINES += WIN_AERO
    CONFIG(debug, debug|release) {
        DESTDIR = ../../bin/win32/debug/
        LIBS += -L../../lib/win32/widgets/debug/ -lanitools.widgets
        LIBS += -L../../lib/win32/plugins/debug/ -lanitools.plugins
        LIBS += -L../../lib/win32/imgproc/debug/ -lanitools.imgproc
        LIBS += -L../../lib/win32/misc/debug/ -lanitools.misc
    } else {
        DESTDIR = ../../bin/win32/release/
        LIBS += -L../../lib/win32/widgets/release/ -lanitools.widgets
        LIBS += -L../../lib/win32/plugins/release/ -lanitools.plugins
        LIBS += -L../../lib/win32/imgproc/release/ -lanitools.imgproc
        LIBS += -L../../lib/win32/misc/release/ -lanitools.misc
    }
}

LIBS += -lFreeImage

INCLUDEPATH += ../../src/batchprocessor/ \
               ../../src/widgets/

SOURCES += ../../src/batchprocessor/main.cpp \
           ../../src/batchprocessor/mainwindow.toolbar.edit.cpp \
           ../../src/batchprocessor/mainwindow.main.cpp \
           ../../src/batchprocessor/mainwindow.toolbar.batch.cpp \
           ../../src/batchprocessor/mainwindow.view.batch.cpp \
           ../../src/batchprocessor/mainwindow.view.edit.cpp \
           ../../src/batchprocessor/imagefolderlistpopup.cpp \
           ../../src/batchprocessor/widgetlist.cpp

HEADERS += ../../src/batchprocessor/mainwindow.h \
           ../../src/batchprocessor/imagefolderlistpopup.h \
           ../../src/batchprocessor/widgetlist.h

FORMS += ../../src/batchprocessor/mainwindow.ui \
         ../../src/batchprocessor/imagefolderlistpopup.ui

RESOURCES += ../../src/batchprocessor/batchprocessor.qrc
