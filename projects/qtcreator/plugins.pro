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

TARGET = anitools.plugins

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = ../../lib/win32/plugins/debug
        DLLDESTDIR = ../../bin/win32/debug
    } else {
        DESTDIR = ../../lib/win32/plugins/release
        DLLDESTDIR = ../../bin/win32/release
    }
}

include(3rdparty.pri)

SOURCES += \
    ../../src/plugins/imagefilterpluginloader.cpp

HEADERS += \
    ../../src/plugins/imagefilterpluginloader.h

