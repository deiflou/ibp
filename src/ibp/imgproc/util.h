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

#ifndef IBP_IMGPROC_UTIL_H
#define IBP_IMGPROC_UTIL_H

#include <QStringList>
#include <QString>

#include "types.h"

namespace ibp {
namespace imgproc {

#define IBP_premultiplyBGRAWithAlpha(src, alpha) \
(src).b = lut01[(src).b][(alpha)]; \
(src).g = lut01[(src).g][(alpha)]; \
(src).r = lut01[(src).r][(alpha)];

#define IBP_premultiplyBGRA(src) \
(src).b = lut01[(src).b][(src).a]; \
(src).g = lut01[(src).g][(src).a]; \
(src).r = lut01[(src).r][(src).a];

#define IBP_postmultiplyBGRA(src) \
(src).b = lut02[(src).b][(src).a]; \
(src).g = lut02[(src).g][(src).a]; \
(src).r = lut02[(src).r][(src).a];

#define IBP_pixelIntensity1(src) \
    (lut01[(src).r][54] + lut01[(src).g][183] + lut01[(src).b][18])

#define IBP_pixelIntensity2(red, green, blue) \
    (lut01[red][54] + lut01[green][183] + lut01[blue][18])

#define IBP_pixelIntensity3(src) \
    ((int)(((src).r * .2126) + ((src).g * .7152) + ((src).b * .0722) + .5))

#define IBP_pixelIntensity4(red, green, blue) \
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

#endif // IBP_IMGPROC_UTIL_H
