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
