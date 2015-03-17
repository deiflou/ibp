# Copyright (C) 2014 - 2015 Deif Lou
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

TARGET = anitools.imagefilter.lumakeyer

include(3rdparty.pri)

win32 {
    CONFIG(debug, debug|release) {
        DLLDESTDIR = ../../bin/win32/debug/plugins
        LIBS += -L../../lib/win32/imgproc/debug -lanitools.imgproc
        LIBS += -L../../lib/win32/widgets/debug -lanitools.widgets
        LIBS += -L../../lib/win32/misc/debug -lanitools.misc
    } else {
        DLLDESTDIR = ../../bin/win32/release/plugins
        LIBS += -L../../lib/win32/imgproc/release -lanitools.imgproc
        LIBS += -L../../lib/win32/widgets/release -lanitools.widgets
        LIBS += -L../../lib/win32/misc/release -lanitools.misc
    }
}

LIBS += -lopencv_core300 -lopencv_imgproc300

INCLUDEPATH += ../../src/widgets/

SOURCES += ../../src/imagefilter_lumakeyer/filter.cpp \
           ../../src/imagefilter_lumakeyer/main.cpp \
           ../../src/imagefilter_lumakeyer/filterwidget.cpp

HEADERS += ../../src/imagefilter_lumakeyer/filter.h \
           ../../src/imagefilter_lumakeyer/filterwidget.h

FORMS += ../../src/imagefilter_lumakeyer/filterwidget.ui

