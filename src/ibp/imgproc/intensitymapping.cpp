//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "intensitymapping.h"
#include "../misc/util.h"

namespace ibp {
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
        value = IBP_clamp(0.0, mIn * (i / 255.0) - bIn, 1.0);
        // Midtones
        value = pow(value, gamma);
        // output levels
        value = mOut * value + bOut;

        lut[i] = IBP_clamp(0, (int)round(value * 255.0), 255);
    }

    return true;
}

} // namespace imgproc
} // namespace ibp
