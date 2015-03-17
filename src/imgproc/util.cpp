/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
**
** This file is part of Anitools
**
** Anitools is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "util.h"

namespace anitools {
namespace imgproc {

QStringList colorCompositionModeStrings = QStringList() <<
     "normal" <<
     "darken" << "multiply" << "colorburn" << "linearburn" << "darkercolor" <<
     "lighten" << "screen" << "colordodge" << "lineardodge" << "lightercolor" <<
     "overlay" << "softlight" << "hardlight" << "vividlight" << "linearlight" << "pinlight" << "hardmix" <<
     "difference" << "exclusion" <<
     "hue" << "saturation" << "color" << "luminosity" <<
     "unsupported";

}}
