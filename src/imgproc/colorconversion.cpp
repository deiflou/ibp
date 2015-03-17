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

#include <QResource>
#include <QDebug>
#include <lcms2.h>

#include "colorconversion.h"
#include "lut.h"
#include "../misc/util.h"

namespace anitools {
namespace imgproc {

using namespace anitools::misc;

cmsHTRANSFORM BGRToLabTransform = 0;
cmsHTRANSFORM BGRToCMYKTransform = 0;
cmsHTRANSFORM LabToBGRTransform = 0;
cmsHTRANSFORM LabToCMYKTransform = 0;
cmsHTRANSFORM CMYKToBGRTransform = 0;
cmsHTRANSFORM CMYKToLabTransform = 0;

void initColorProfiles()
{
    static bool initialized = false;
    if (initialized)
        return;

    QResource res(":/anitools/other/cmykProfile");
    cmsHPROFILE BGRProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE CMYKProfile = cmsOpenProfileFromMem(res.data(), res.size());
    cmsHPROFILE LabProfile = cmsCreateLab4Profile(0);

    BGRToLabTransform = cmsCreateTransform(BGRProfile, TYPE_BGRA_8, LabProfile, TYPE_Lab_8, INTENT_PERCEPTUAL, 0);
    BGRToCMYKTransform = cmsCreateTransform(BGRProfile, TYPE_BGRA_8, CMYKProfile, TYPE_CMYK_8, INTENT_PERCEPTUAL, 0);
    LabToBGRTransform = cmsCreateTransform(LabProfile, TYPE_Lab_8, BGRProfile, TYPE_BGRA_8, INTENT_PERCEPTUAL, 0);
    LabToCMYKTransform = cmsCreateTransform(LabProfile, TYPE_Lab_8, CMYKProfile, TYPE_CMYK_8, INTENT_PERCEPTUAL, 0);
    CMYKToBGRTransform = cmsCreateTransform(CMYKProfile, TYPE_CMYK_8, BGRProfile, TYPE_BGRA_8, INTENT_PERCEPTUAL, 0);
    CMYKToLabTransform = cmsCreateTransform(CMYKProfile, TYPE_CMYK_8, LabProfile, TYPE_Lab_8, INTENT_PERCEPTUAL, 0);

    cmsCloseProfile(BGRProfile);
    cmsCloseProfile(LabProfile);
    cmsCloseProfile(CMYKProfile);

    initialized = true;
}

void convertBGRToHSV(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    register int r, g, b, min, max, dMax, h, s, v;
    while (nPixels--)
    {
        r = inputBuffer[ColorChannel_Red] * 4095 / 255;
        g = inputBuffer[ColorChannel_Green] * 4095 / 255;
        b = inputBuffer[ColorChannel_Blue] * 4095 / 255;

        min = AT_minimum3(r, g, b);
        max = AT_maximum3(r, g, b);
        dMax = max - min;

        if (dMax == 0)
        {
            outputBuffer[ColorChannel_Hue] = 0;
            outputBuffer[ColorChannel_Saturation] = 0;
            outputBuffer[ColorChannel_Value] = max >> 4;

            inputBuffer += 4;
            outputBuffer += 3;
            continue;
        }

        v = max;
        s = (dMax << 12) / max;

        if (max == r)
            h = ((g - b) << 12) / 6 / dMax;
        else if (max == g)
            h = ((b - r) << 12) / 6 / dMax + 1365;
        else
            h = ((r - g) << 12) / 6 / dMax + 2730;

        outputBuffer[ColorChannel_Hue] = (h < 0 ? h + 4096 : h > 4095 ? h - 4096 : h) * 255 / 4095;
        outputBuffer[ColorChannel_Saturation] = s * 255 / 4095;
        outputBuffer[ColorChannel_Value] = v * 255 / 4095;

        inputBuffer += 4;
        outputBuffer += 3;
    }
}

void convertBGRToHSL(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    register int r, g, b, min, max, dMax, minPlusMax, h, s, l;
    while (nPixels--)
    {
        r = inputBuffer[ColorChannel_Red] * 4095 / 255;
        g = inputBuffer[ColorChannel_Green] * 4095 / 255;
        b = inputBuffer[ColorChannel_Blue] * 4095 / 255;

        min = AT_minimum3(r, g, b);
        max = AT_maximum3(r, g, b);
        dMax = max - min;

        minPlusMax = min + max;
        l = minPlusMax >> 1;

        if (dMax == 0)
        {
            outputBuffer[ColorChannel_Hue] = 0;
            outputBuffer[ColorChannel_Saturation] = 0;
            outputBuffer[ColorChannel_Lightness] = l >> 4;

            inputBuffer += 4;
            outputBuffer += 3;
            continue;
        }

        s = (dMax << 12) / (l < 2048 ? minPlusMax : 8191 - minPlusMax);

        if (max == r)
            h = ((g - b) << 12) / 6 / dMax;
        else if (max == g)
            h = ((b - r) << 12) / 6 / dMax + 1365;
        else
            h = ((r - g) << 12) / 6 / dMax + 2730;

        outputBuffer[ColorChannel_Hue] = (h < 0 ? h + 4096 : h > 4095 ? h - 4096 : h) * 255 / 4095;
        outputBuffer[ColorChannel_Saturation] = s * 255 / 4095;
        outputBuffer[ColorChannel_Lightness] = l * 255 / 4095;

        inputBuffer += 4;
        outputBuffer += 3;
    }
}

void convertBGRToLab(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(BGRToLabTransform, inputBuffer, outputBuffer, nPixels);
}

void convertBGRToCMYK(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(BGRToCMYKTransform, inputBuffer, outputBuffer, nPixels);
}

void convertHSVToBGR(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    register int r, g, b, h, s, v, i, v0, v1, v2, v3;
    while (nPixels--)
    {
        if (inputBuffer[ColorChannel_Saturation] == 0)
        {
            outputBuffer[ColorChannel_Blue] = outputBuffer[ColorChannel_Green] = outputBuffer[ColorChannel_Red] =
                    inputBuffer[ColorChannel_Value];

            inputBuffer += 3;
            outputBuffer += 4;
            continue;
        }

        h = inputBuffer[ColorChannel_Hue] * 4095 / 255;
        s = inputBuffer[ColorChannel_Saturation] * 4095 / 255;
        v = inputBuffer[ColorChannel_Value] * 4095 / 255;

        i = (h + 1) * 6;
        if (i == 24576)
            i = 0;
        v0 = i - (i >> 12 << 12);
        v1 = v * (4095 - s) / 4095;
        v2 = v * (4095 - s * v0 / 4095) / 4095;
        v3 = v * (4095 - s * (4095 - v0) / 4095) / 4095;

        if (i > 20479)
        {
            r = v;
            g = v1;
            b = v2;
        }
        else if (i > 16383)
        {
            r = v3;
            g = v1;
            b = v;
        }
        else if (i > 12287)
        {
            r = v1;
            g = v2;
            b = v;
        }
        else if (i > 8191)
        {
            r = v1;
            g = v;
            b = v3;
        }
        else if (i > 4095)
        {
            r = v2;
            g = v;
            b = v1;
        }
        else
        {
            r = v;
            g = v3 ;
            b = v1;
        }

        outputBuffer[ColorChannel_Red] = r * 255 / 4095;
        outputBuffer[ColorChannel_Green] = g * 255 / 4095;
        outputBuffer[ColorChannel_Blue] = b * 255 / 4095;

        inputBuffer += 3;
        outputBuffer += 4;
    }
}

void convertHSVToHSL(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    register int dMax, minPlusMax;
    while (nPixels--)
    {
        dMax = lut01[inputBuffer[ColorChannel_Saturation]][inputBuffer[ColorChannel_Value]];
        minPlusMax = inputBuffer[ColorChannel_Value] - dMax + inputBuffer[ColorChannel_Value];
        outputBuffer[ColorChannel_Value] = minPlusMax >> 1;
        outputBuffer[ColorChannel_Saturation] = outputBuffer[ColorChannel_Value] < 128 ?
                    lut02[dMax][minPlusMax] : lut02[dMax][511 - minPlusMax];
        outputBuffer[ColorChannel_Hue] = inputBuffer[ColorChannel_Hue];

        inputBuffer += 3;
        outputBuffer += 3;
    }
}

void convertHSVToLab(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    convertHSVToBGR(inputBuffer, outputBuffer, nPixels);
    cmsDoTransform(BGRToLabTransform, outputBuffer, outputBuffer, nPixels);
}

void convertHSVToCMYK(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    convertHSVToBGR(inputBuffer, outputBuffer, nPixels);
    cmsDoTransform(BGRToCMYKTransform, outputBuffer, outputBuffer, nPixels);
}

void convertHSLToBGR(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    register int r, g, b, h, s, l, v2, v1, vH;
    while (nPixels--)
    {
        if (inputBuffer[ColorChannel_Saturation] == 0)
        {
            outputBuffer[ColorChannel_Blue] = outputBuffer[ColorChannel_Green] = outputBuffer[ColorChannel_Red] =
                    inputBuffer[ColorChannel_Lightness];

            inputBuffer += 3;
            outputBuffer += 4;
            continue;
        }

        h = inputBuffer[ColorChannel_Hue] * 4095 / 255;
        s = inputBuffer[ColorChannel_Saturation] * 4095 / 255;
        l = inputBuffer[ColorChannel_Lightness] * 4095 / 255;

        v2 = l < 2048 ? l * (4095 + s) / 4095 : l + s - l * s / 4095;
        v1 = (l << 1) - v2;

        vH = h + 1365;
        if (vH > 4095)
            vH -= 4096;
        if (6 * vH < 4096)
            r = v1 + (v2 - v1) * vH * 6 / 4095;
        else if (2 * vH < 4096)
            r = v2;
        else if (3 * vH < 8192)
            r = v1 + (v2 - v1) * (2730 - vH) * 6 / 4095;
        else
            r = v1;

        if (6 * h < 4096)
            g = v1 + (v2 - v1) * h * 6 / 4095;
        else if (2 * h < 4096)
            g = v2;
        else if (3 * h < 8192)
            g = v1 + (v2 - v1) * (2730 - h) * 6 / 4095;
        else
            g = v1;

        vH = h - 1365;
        if (vH < 0)
            vH += 4096;
        if (6 * vH < 4096)
            b = v1 + (v2 - v1) * vH * 6 / 4095;
        else if (2 * vH < 4096)
            b = v2;
        else if (3 * vH < 8192)
            b = v1 + (v2 - v1) * (2730 - vH) * 6 / 4095;
        else
            b = v1;

        outputBuffer[ColorChannel_Red] = r * 255 / 4095;
        outputBuffer[ColorChannel_Green] = g * 255 / 4095;
        outputBuffer[ColorChannel_Blue] = b * 255 / 4095;

        inputBuffer += 3;
        outputBuffer += 4;
    }
}

void convertHSLToHSV(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    register int minPlusMax, dMax;
    while (nPixels--)
    {
        minPlusMax = inputBuffer[ColorChannel_Lightness] << 1;
        dMax = inputBuffer[ColorChannel_Lightness] < 128 ?
               lut01[minPlusMax][inputBuffer[ColorChannel_Saturation]] :
               lut01[511 - minPlusMax][inputBuffer[ColorChannel_Saturation]];
        outputBuffer[ColorChannel_Value] = (minPlusMax + dMax) >> 1;
        outputBuffer[ColorChannel_Saturation] = lut02[dMax][outputBuffer[ColorChannel_Value]];
        outputBuffer[ColorChannel_Hue] = inputBuffer[ColorChannel_Hue];

        inputBuffer += 3;
        outputBuffer += 3;
    }
}

void convertHSLToLab(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    convertHSLToBGR(inputBuffer, outputBuffer, nPixels);
    cmsDoTransform(BGRToLabTransform, outputBuffer, outputBuffer, nPixels);
}

void convertHSLToCMYK(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    convertHSLToBGR(inputBuffer, outputBuffer, nPixels);
    cmsDoTransform(BGRToCMYKTransform, outputBuffer, outputBuffer, nPixels);
}

void convertLabToBGR(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(LabToBGRTransform, inputBuffer, outputBuffer, nPixels);
}

void convertLabToHSV(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(LabToBGRTransform, inputBuffer, outputBuffer, nPixels);
    convertBGRToHSV(outputBuffer, outputBuffer, nPixels);
}

void convertLabToHSL(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(LabToBGRTransform, inputBuffer, outputBuffer, nPixels);
    convertBGRToHSL(outputBuffer, outputBuffer, nPixels);
}

void convertLabToCMYK(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(LabToCMYKTransform, inputBuffer, outputBuffer, nPixels);
}

void convertCMYKToBGR(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(CMYKToBGRTransform, inputBuffer, outputBuffer, nPixels);
}

void convertCMYKToHSV(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(CMYKToBGRTransform, inputBuffer, outputBuffer, nPixels);
    convertBGRToHSV(outputBuffer, outputBuffer, nPixels);
}

void convertCMYKToHSL(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(CMYKToBGRTransform, inputBuffer, outputBuffer, nPixels);
    convertBGRToHSL(outputBuffer, outputBuffer, nPixels);
}

void convertCMYKToLab(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    initColorProfiles();
    cmsDoTransform(CMYKToLabTransform, inputBuffer, outputBuffer, nPixels);
}

void convertIdentity(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    Q_UNUSED(inputBuffer)
    Q_UNUSED(outputBuffer)
    Q_UNUSED(nPixels)
}

void (* convertColors[5][5])(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels) =
{
{ convertIdentity, convertBGRToHSV, convertBGRToHSL, convertBGRToLab, convertBGRToCMYK },
{ convertHSVToBGR, convertIdentity, convertHSVToHSL, convertHSVToLab, convertHSVToCMYK },
{ convertHSLToBGR, convertHSLToHSV, convertIdentity, convertHSLToLab, convertHSLToCMYK },
{ convertLabToBGR, convertLabToHSV, convertLabToHSL, convertIdentity, convertLabToCMYK },
{ convertCMYKToBGR, convertCMYKToHSV, convertCMYKToHSL, convertCMYKToLab, convertIdentity },
};

inline void swapChannels01(unsigned char * inputBuffer)
{
    unsigned char t = inputBuffer[0];
    inputBuffer[0] = inputBuffer[1];
    inputBuffer[1] = t;
}

inline void swapChannels02(unsigned char * inputBuffer)
{
    unsigned char t = inputBuffer[0];
    inputBuffer[0] = inputBuffer[2];
    inputBuffer[2] = t;
}

inline void swapChannels03(unsigned char * inputBuffer)
{
    unsigned char t = inputBuffer[0];
    inputBuffer[0] = inputBuffer[3];
    inputBuffer[3] = t;
}

inline void swapChannels12(unsigned char * inputBuffer)
{
    unsigned char t = inputBuffer[1];
    inputBuffer[1] = inputBuffer[2];
    inputBuffer[2] = t;
}

inline void swapChannels13(unsigned char * inputBuffer)
{
    unsigned char t = inputBuffer[1];
    inputBuffer[1] = inputBuffer[3];
    inputBuffer[3] = t;
}

inline void swapChannels23(unsigned char * inputBuffer)
{
    unsigned char t = inputBuffer[2];
    inputBuffer[2] = inputBuffer[3];
    inputBuffer[3] = t;
}

inline void swapChannelsIdentity(unsigned char * inputBuffer)
{
    Q_UNUSED(inputBuffer)
}

void (* swapChannels[4][4])(unsigned char * inputBuffer) =
{
{ swapChannelsIdentity, swapChannels01, swapChannels02, swapChannels03 },
{ swapChannels01, swapChannelsIdentity, swapChannels12, swapChannels13 },
{ swapChannels02, swapChannels12, swapChannelsIdentity, swapChannels23 },
{ swapChannels03, swapChannels13, swapChannels23, swapChannelsIdentity },
};

}}
