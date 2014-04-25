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
