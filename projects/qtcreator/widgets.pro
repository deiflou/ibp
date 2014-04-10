#-------------------------------------------------
#
# Project created by QtCreator 2014-03-07T22:21:05
#
#-------------------------------------------------

QT += widgets

TEMPLATE = lib

TARGET = anitools.widgets

win32 {
    DEFINES += WIN_AERO
    CONFIG(debug, debug|release) {
        DESTDIR = ../../lib/win32/widgets/debug
        DLLDESTDIR = ../../bin/win32/debug
        LIBS += -L../../lib/win32/imgproc/debug -lanitools.imgproc
    } else {
        DESTDIR = ../../lib/win32/widgets/release
        DLLDESTDIR = ../../bin/win32/release
        LIBS += -L../../lib/win32/imgproc/release -lanitools.imgproc
    }
}

INCLUDEPATH += ../../src/widgets

SOURCES += \
    ../../src/widgets/resinit.cpp \
    ../../src/widgets/blendmodecombobox.cpp \
    ../../src/widgets/colorbutton.cpp \
    ../../src/widgets/colorslider.cpp \
    ../../src/widgets/histogramviewer.cpp \
    ../../src/widgets/imageviewer.cpp \
    ../../src/widgets/toolbuttonex.cpp \
    ../../src/widgets/genericdialog.cpp \
    ../../src/widgets/colorpicker.cpp

HEADERS += \
    ../../src/widgets/blendmodecombobox.h \
    ../../src/widgets/colorbutton.h \
    ../../src/widgets/colorslider.h \
    ../../src/widgets/histogramviewer.h \
    ../../src/widgets/imageviewer.h \
    ../../src/widgets/toolbuttonex.h \
    ../../src/widgets/genericdialog.h \
    ../../src/widgets/colorpicker.h

RESOURCES += \
    ../../src/widgets/widgets.qrc

FORMS += \
    ../../src/widgets/genericdialog.ui \
    ../../src/widgets/colorpicker.ui
