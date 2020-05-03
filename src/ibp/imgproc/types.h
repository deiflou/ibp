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

#ifndef IBP_IMGPROC_TYPES_H
#define IBP_IMGPROC_TYPES_H

namespace ibp {
namespace imgproc {

enum ColorModel
{
    ColorModel_BGR, ColorModel_HSV, ColorModel_HSL, ColorModel_Lab, ColorModel_CMYK
};

enum ColorChannel
{
    ColorChannel_0 = 0, ColorChannel_1 = 1, ColorChannel_2 = 2, ColorChannel_3 = 3,
    ColorChannel_Blue = 0, ColorChannel_Green = 1, ColorChannel_Red = 2, ColorChannel_Alpha = 3,
    ColorChannel_Hue = 0, ColorChannel_Saturation = 1, ColorChannel_Value = 2, ColorChannel_Lightness = 2,
    ColorChannel_CIEL = 0, ColorChannel_CIEa = 1, ColorChannel_CIEb = 2,
    ColorChannel_Cyan = 0, ColorChannel_Magenta = 1, ColorChannel_Yellow = 2, ColorChannel_Black = 3
};

enum AlphaCompositionMode
{
    AlphaCompositionMode_Source = 0,
    AlphaCompositionMode_Destination,
    AlphaCompositionMode_SourceOverDestination,
    AlphaCompositionMode_DestinationOverSource,
    AlphaCompositionMode_SourceInDestination,
    AlphaCompositionMode_DestinationInSource,
    AlphaCompositionMode_SourceOutDestination,
    AlphaCompositionMode_DestinationOutSource,
    AlphaCompositionMode_SourceAtopDestination,
    AlphaCompositionMode_DestinationAtopSource,
    AlphaCompositionMode_SourceClearDestination,
    AlphaCompositionMode_SourceXorDestination
};

enum ColorCompositionMode
{
    ColorCompositionMode_Normal = 0,
    ColorCompositionMode_Darken,
    ColorCompositionMode_Multiply,
    ColorCompositionMode_ColorBurn,
    ColorCompositionMode_LinearBurn,
    ColorCompositionMode_DarkerColor,
    ColorCompositionMode_Lighten,
    ColorCompositionMode_Screen,
    ColorCompositionMode_ColorDodge,
    ColorCompositionMode_LinearDodge,
    ColorCompositionMode_LighterColor,
    ColorCompositionMode_Overlay,
    ColorCompositionMode_softLight,
    ColorCompositionMode_HardLight,
    ColorCompositionMode_VividLight,
    ColorCompositionMode_LinearLight,
    ColorCompositionMode_PinLight,
    ColorCompositionMode_HardMix,
    ColorCompositionMode_Difference,
    ColorCompositionMode_Exclusion,
    ColorCompositionMode_Hue,
    ColorCompositionMode_Saturation,
    ColorCompositionMode_Color,
    ColorCompositionMode_Luminosity,
    ColorCompositionMode_Unsupported
};

enum AffineTransformationType
{
    Translation,
    Scaling,
    Rotation,
    Shearing
};

struct RGB
{
    unsigned char r, g, b;
};

struct BGR
{
    unsigned char b, g, r;
};

struct RGBA
{
    unsigned char r, g, b, a;
};

struct BGRA
{
    unsigned char b, g, r, a;
};

struct HSV
{
    unsigned char h, s, v;
};

struct HSL
{
    unsigned char h, s, l;
};

struct Lab
{
    unsigned char L, a, b;
};

struct CMYK
{
    unsigned char c, m, y, k;
};

struct AffineTransformation
{
    AffineTransformationType type;
    double x, y, z;

    AffineTransformation()
    {
        type = Translation;
        x = y = z = 0;
    }
    AffineTransformation(AffineTransformationType t, double x, double y, double z)
    {
        type = t;
        this->x = x;
        this->y = y;
        this->z = z;
    }
    bool operator==(const AffineTransformation & t) const
    {
        return t.type == type && t.x == x && t.y == y && t.z == z;
    }
    bool operator!=(const AffineTransformation & t) const
    {
        return !(t.type == type && t.x == x && t.y == y && t.z == z);
    }
};

}}

#endif // IBP_IMGPROC_TYPES_H
