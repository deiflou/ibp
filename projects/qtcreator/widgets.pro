# Copyright (C) 2014  Deif Lou
#
# This file is part of Anitools
#
# Anitools is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

QT += widgets

TEMPLATE = lib

TARGET = anitools.widgets

win32 {
    QT += winextras
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
    ../../src/widgets/colorbutton.cpp \
    ../../src/widgets/colorslider.cpp \
    ../../src/widgets/histogramviewer.cpp \
    ../../src/widgets/imageviewer.cpp \
    ../../src/widgets/toolbuttonex.cpp \
    ../../src/widgets/genericdialog.cpp \
    ../../src/widgets/colorpicker.cpp \
    ../../src/widgets/colorbox.cpp \
    ../../src/widgets/imagebutton.cpp \
    ../../src/widgets/colorcompositionmodecombobox.cpp \
    ../../src/widgets/widgetlist.cpp \
    ../../src/widgets/affinetransformationlist.cpp

HEADERS += \
    ../../src/widgets/colorbutton.h \
    ../../src/widgets/colorslider.h \
    ../../src/widgets/histogramviewer.h \
    ../../src/widgets/imageviewer.h \
    ../../src/widgets/toolbuttonex.h \
    ../../src/widgets/genericdialog.h \
    ../../src/widgets/colorpicker.h \
    ../../src/widgets/colorbox.h \
    ../../src/widgets/imagebutton.h \
    ../../src/widgets/colorcompositionmodecombobox.h \
    ../../src/widgets/widgetlist.h \
    ../../src/widgets/affinetransformationlist.h

RESOURCES += \
    ../../src/widgets/widgets.qrc

FORMS += \
    ../../src/widgets/genericdialog.ui \
    ../../src/widgets/colorpicker.ui \
    ../../src/widgets/affinetransformationlist.ui
