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

TARGET = anitools.imagebatchprocessor
TEMPLATE = app

win32 {
    QT += winextras
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

INCLUDEPATH += ../../src/imagebatchprocessor/ \
               ../../src/widgets/

SOURCES += ../../src/imagebatchprocessor/main.cpp \
           ../../src/imagebatchprocessor/mainwindow.toolbar.edit.cpp \
           ../../src/imagebatchprocessor/mainwindow.main.cpp \
           ../../src/imagebatchprocessor/mainwindow.toolbar.batch.cpp \
           ../../src/imagebatchprocessor/mainwindow.view.batch.cpp \
           ../../src/imagebatchprocessor/mainwindow.view.edit.cpp \
           ../../src/imagebatchprocessor/imagefolderlistpopup.cpp

HEADERS += ../../src/imagebatchprocessor/mainwindow.h \
           ../../src/imagebatchprocessor/imagefolderlistpopup.h

FORMS += ../../src/imagebatchprocessor/mainwindow.ui \
         ../../src/imagebatchprocessor/imagefolderlistpopup.ui

RESOURCES += ../../src/imagebatchprocessor/imagebatchprocessor.qrc
