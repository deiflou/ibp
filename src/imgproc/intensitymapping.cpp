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

#include "intensitymapping.h"
#include "../misc/util.h"

namespace anitools {
namespace imgproc {

bool generateLevelsLUT(unsigned char *lut,
                       double gammaCorrection, double inputBlackPoint, double inputWhitePoint,
                       double outputBlackPoint, double outputWhitePoint)
{
    if (!lut ||
        gammaCorrection < .1 || gammaCorrection > 10. ||
        inputBlackPoint < 0. || inputBlackPoint > 1. ||
        inputWhitePoint < 0. || inputWhitePoint > 1. ||
        outputBlackPoint < 0. || outputBlackPoint > 1. ||
        outputWhitePoint < 0. || outputWhitePoint > 1.)
        return false;
    if (inputBlackPoint >= inputWhitePoint)
        return false;

    const double gamma = 1.0 / gammaCorrection;
    const double mIn = 1.0 / (inputWhitePoint - inputBlackPoint);
    const double bIn = mIn * inputBlackPoint;
    const double mOut = outputWhitePoint - outputBlackPoint;
    const double bOut = outputBlackPoint;
    double value;

    for (int i = 0; i < 256; i++)
    {
        // Shadows & Highights
        value = AT_clamp(0.0, mIn * (i / 255.0) - bIn, 1.0);
        // Midtones
        value = pow(value, gamma);
        // output levels
        value = mOut * value + bOut;

        lut[i] = AT_clamp(0, (int)round(value * 255.0), 255);
    }

    return true;
}

} // namespace imgproc
} // namespace anitools
