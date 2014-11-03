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

TARGET = anitools.misc

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = ../../lib/win32/misc/debug
        DLLDESTDIR = ../../bin/win32/debug
    } else {
        DESTDIR = ../../lib/win32/misc/release
        DLLDESTDIR = ../../bin/win32/release
    }
}

include(3rdparty.pri)

SOURCES += \
    ../../src/misc/nativeeventfilter.cpp \
    ../../src/misc/configurationmanager.cpp \
    ../../src/misc/interpolator1D.cpp \
    ../../src/misc/basesplineinterpolator1D.cpp \
    ../../src/misc/cubicsplineinterpolator1D.cpp \
    ../../src/misc/linearsplineinterpolator1D.cpp \
    ../../src/misc/nearestneighborsplineinterpolator1D.cpp

HEADERS += \
    ../../src/misc/util.h \
    ../../src/misc/nativeeventfilter.h \
    ../../src/misc/configurationmanager.h \
    ../../src/misc/interpolator1D.h \
    ../../src/misc/basesplineinterpolator1D.h \
    ../../src/misc/cubicsplineinterpolator1D.h \
    ../../src/misc/linearsplineinterpolator1D.h \
    ../../src/misc/nearestneighborsplineinterpolator1D.h
