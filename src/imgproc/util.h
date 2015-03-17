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

#ifndef ANITOOLS_IMGPROC_UTIL_H
#define ANITOOLS_IMGPROC_UTIL_H

#include <QStringList>
#include <QString>

#include "types.h"

namespace anitools {
namespace imgproc {

#define AT_premultiplyBGRAWithAlpha(src, alpha) \
(src).b = lut01[(src).b][(alpha)]; \
(src).g = lut01[(src).g][(alpha)]; \
(src).r = lut01[(src).r][(alpha)];

#define AT_premultiplyBGRA(src) \
(src).b = lut01[(src).b][(src).a]; \
(src).g = lut01[(src).g][(src).a]; \
(src).r = lut01[(src).r][(src).a];

#define AT_postmultiplyBGRA(src) \
(src).b = lut02[(src).b][(src).a]; \
(src).g = lut02[(src).g][(src).a]; \
(src).r = lut02[(src).r][(src).a];

#define AT_pixelIntensity1(src) \
    (lut01[(src).r][54] + lut01[(src).g][183] + lut01[(src).b][18])

#define AT_pixelIntensity2(red, green, blue) \
    (lut01[red][54] + lut01[green][183] + lut01[blue][18])

#define AT_pixelIntensity3(src) \
    ((int)(((src).r * .2126) + ((src).g * .7152) + ((src).b * .0722) + .5))

#define AT_pixelIntensity4(red, green, blue) \
    ((int)(((red) * .2126) + ((green) * .7152) + ((blue) * .0722) + .5))

extern QStringList colorCompositionModeStrings;
inline ColorCompositionMode colorCompositionModeStringToEnum(const QString & mode)
{
    if (!colorCompositionModeStrings.contains(mode))
        return ColorCompositionMode_Unsupported;
    return (ColorCompositionMode)colorCompositionModeStrings.indexOf(mode.toLower());
}
inline QString colorCompositionModeEnumToString(ColorCompositionMode mode)
{
    if (mode < ColorCompositionMode_Normal || mode > ColorCompositionMode_Unsupported)
        return QString();
    return colorCompositionModeStrings.at(mode);
}

}}

#endif // ANITOOLS_IMGPROC_UTIL_H
