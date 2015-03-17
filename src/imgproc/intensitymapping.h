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


#ifndef ANITOOLS_IMGPROC_INTENSITYMAPPING_H
#define ANITOOLS_IMGPROC_INTENSITYMAPPING_H

namespace anitools {
namespace imgproc {

bool generateLevelsLUT(unsigned char * lut,
                       double gammaCorrection = 1., double inputBlackPoint = 0., double inputWhitePoint = 1.,
                       double outputBlackPoint = 0., double outputWhitePoint = 1.);

} // namespace imgproc
} // namespace anitools

#endif // ANITOOLS_IMGPROC_INTENSITYMAPPING_H
