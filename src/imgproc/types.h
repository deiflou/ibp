#ifndef ANITOOLS_IMGPROC_TYPES_H
#define ANITOOLS_IMGPROC_TYPES_H

namespace anitools {
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

enum BlendMode
{
    BlendMode_Normal = 0,
    BlendMode_Darken,
    BlendMode_Multiply,
    BlendMode_ColorBurn,
    BlendMode_LinearBurn,
    BlendMode_DarkerColor,
    BlendMode_Lighten,
    BlendMode_Screen,
    BlendMode_ColorDodge,
    BlendMode_LinearDodge,
    BlendMode_LighterColor,
    BlendMode_Overlay,
    BlendMode_softLight,
    BlendMode_HardLight,
    BlendMode_VividLight,
    BlendMode_LinearLight,
    BlendMode_PinLight,
    BlendMode_HardMix,
    BlendMode_Difference,
    BlendMode_Exclusion,
    BlendMode_Hue,
    BlendMode_Saturation,
    BlendMode_Color,
    BlendMode_Luminosity,
    BlendMode_Unsupported
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

}}

#endif // ANITOOLS_IMGPROC_TYPES_H
