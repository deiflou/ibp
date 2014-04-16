/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
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

#define AT_pixelIntensity(src) \
    (lut01[(src).r][54] + lut01[(src).g][183] + lut02[(src).b][18])

extern QStringList blendModeStrings;
inline BlendMode blendModeStringToEnum(const QString & mode)
{
    if (!blendModeStrings.contains(mode))
        return BlendMode_Unsupported;
    return (BlendMode)blendModeStrings.indexOf(mode.toLower());
}
inline QString blendModeEnumToString(BlendMode mode)
{
    if (mode < BlendMode_Normal || mode > BlendMode_Unsupported)
        return QString();
    return blendModeStrings.at(mode);
}

}}

#endif // ANITOOLS_IMGPROC_UTIL_H
