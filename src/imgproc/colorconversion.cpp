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
    register int min, max, dMax, h, s, v, dMaxOver2;
    while (nPixels--)
    {
        min = AT_minimum3(inputBuffer[ColorChannel_Blue], inputBuffer[ColorChannel_Green],
                                    inputBuffer[ColorChannel_Red]);
        max = AT_maximum3(inputBuffer[ColorChannel_Blue], inputBuffer[ColorChannel_Green],
                                    inputBuffer[ColorChannel_Red]);
        dMax = max - min;

        if (dMax == 0)
        {
            outputBuffer[ColorChannel_Hue] = 0;
            outputBuffer[ColorChannel_Saturation] = 0;
            outputBuffer[ColorChannel_Value] = max;

            inputBuffer += 4;
            outputBuffer += 3;
            continue;
        }

        v = max;
        s = lut02[dMax][max];

        dMaxOver2 = dMax >> 1;
        if (max == inputBuffer[ColorChannel_Red])
            h = lut02[lut01[(max - inputBuffer[ColorChannel_Blue])][42] + dMaxOver2][dMax] -
                lut02[lut01[(max - inputBuffer[ColorChannel_Green])][42] + dMaxOver2][dMax];
        else if (max == inputBuffer[ColorChannel_Green])
            h = lut02[lut01[(max - inputBuffer[ColorChannel_Red])][42] + dMaxOver2][dMax] -
                lut02[lut01[(max - inputBuffer[ColorChannel_Blue])][42] + dMaxOver2][dMax] + 85;
        else
            h = lut02[lut01[(max - inputBuffer[ColorChannel_Green])][42] + dMaxOver2][dMax] -
                lut02[lut01[(max - inputBuffer[ColorChannel_Red])][42] + dMaxOver2][dMax] + 170;

        outputBuffer[ColorChannel_Hue] = h < 0 ? h + 256 : h > 255 ? h - 256 : h;
        outputBuffer[ColorChannel_Saturation] = s;
        outputBuffer[ColorChannel_Value] = v;

        inputBuffer += 4;
        outputBuffer += 3;
    }
}

void convertBGRToHSL(const unsigned char *inputBuffer, unsigned char *outputBuffer, int nPixels)
{
    register int min, max, dMax, minPlusMax, h, s, l, dMaxOver2;
    while (nPixels--)
    {
        min = AT_minimum3(inputBuffer[ColorChannel_Blue], inputBuffer[ColorChannel_Green],
                                    inputBuffer[ColorChannel_Red]);
        max = AT_maximum3(inputBuffer[ColorChannel_Blue], inputBuffer[ColorChannel_Green],
                                    inputBuffer[ColorChannel_Red]);
        dMax = max - min;
        minPlusMax = min + max;

        if (dMax == 0)
        {
            outputBuffer[ColorChannel_Hue] = 0;
            outputBuffer[ColorChannel_Saturation] = 0;
            outputBuffer[ColorChannel_Lightness] = minPlusMax >> 1;

            inputBuffer += 4;
            outputBuffer += 3;
            continue;
        }

        l = minPlusMax >> 1;
        s = l < 128 ? lut02[dMax][minPlusMax] : lut02[dMax][511 - minPlusMax];

        dMaxOver2 = dMax >> 1;
        if (max == inputBuffer[ColorChannel_Red])
            h = lut02[lut01[(max - inputBuffer[ColorChannel_Blue])][42] + dMaxOver2][dMax] -
                lut02[lut01[(max - inputBuffer[ColorChannel_Green])][42] + dMaxOver2][dMax];
        else if (max == inputBuffer[ColorChannel_Green])
            h = lut02[lut01[(max - inputBuffer[ColorChannel_Red])][42] + dMaxOver2][dMax] -
                lut02[lut01[(max - inputBuffer[ColorChannel_Blue])][42] + dMaxOver2][dMax] + 85;
        else
            h = lut02[lut01[(max - inputBuffer[ColorChannel_Green])][42] + dMaxOver2][dMax] -
                lut02[lut01[(max - inputBuffer[ColorChannel_Red])][42] + dMaxOver2][dMax] + 170;

        outputBuffer[ColorChannel_Hue] = h < 0 ? h + 256 : h > 255 ? h - 256 : h;
        outputBuffer[ColorChannel_Saturation] = s;
        outputBuffer[ColorChannel_Lightness] = l;

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
    register int i, v0, v1, v2, v3;
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

        i = (inputBuffer[ColorChannel_Hue] + 1) * 6;
        v0 = i - (i >> 8 << 8);
        v1 = lut01[inputBuffer[ColorChannel_Value]][255 - inputBuffer[ColorChannel_Saturation]];
        v2 = lut01[inputBuffer[ColorChannel_Value]][255 - lut01[inputBuffer[ColorChannel_Saturation]][v0]];
        v3 = lut01[inputBuffer[ColorChannel_Value]][255 - lut01[inputBuffer[ColorChannel_Saturation]][255 - v0]];

        if (i > 1279)
        {
            outputBuffer[ColorChannel_Red] = inputBuffer[ColorChannel_Value];
            outputBuffer[ColorChannel_Green] = v1;
            outputBuffer[ColorChannel_Blue] = v2;
        }
        else if (i > 1023)
        {
            outputBuffer[ColorChannel_Red] = v3;
            outputBuffer[ColorChannel_Green] = v1;
            outputBuffer[ColorChannel_Blue] = inputBuffer[ColorChannel_Value];
        }
        else if (i > 767)
        {
            outputBuffer[ColorChannel_Red] = v1;
            outputBuffer[ColorChannel_Green] = v2;
            outputBuffer[ColorChannel_Blue] = inputBuffer[ColorChannel_Value];
        }
        else if (i > 511)
        {
            outputBuffer[ColorChannel_Red] = v1;
            outputBuffer[ColorChannel_Green] = inputBuffer[ColorChannel_Value];
            outputBuffer[ColorChannel_Blue] = v3;
        }
        else if (i > 255)
        {
            outputBuffer[ColorChannel_Red] = v2;
            outputBuffer[ColorChannel_Green] = inputBuffer[ColorChannel_Value];
            outputBuffer[ColorChannel_Blue] = v1;
        }
        else
        {
            outputBuffer[ColorChannel_Red] = inputBuffer[ColorChannel_Value];
            outputBuffer[ColorChannel_Green] = v3 ;
            outputBuffer[ColorChannel_Blue] = v1;
        }

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
    register int v2, v1, h, vH;
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

        v2 = (inputBuffer[ColorChannel_Lightness] < 128) ?
             inputBuffer[ColorChannel_Lightness] * (255 + inputBuffer[ColorChannel_Saturation]) / 255 :
             inputBuffer[ColorChannel_Lightness] + inputBuffer[ColorChannel_Saturation] -
             lut01[inputBuffer[ColorChannel_Saturation]][inputBuffer[ColorChannel_Lightness]];
        v1 = (inputBuffer[ColorChannel_Lightness] << 1) - v2;

        h = inputBuffer[ColorChannel_Hue];

        vH = h + 85;
        if (vH > 255)
            vH -= 256;
        if (6 * vH < 256)
            outputBuffer[ColorChannel_Red] = v1 + lut01[v2 - v1][vH * 6];
        else if (2 * vH < 256)
            outputBuffer[ColorChannel_Red] = v2;
        else if (3 * vH < 512)
            outputBuffer[ColorChannel_Red] = v1 + lut01[v2 - v1][(170 - vH) * 6];
        else
            outputBuffer[ColorChannel_Red] = v1;

        if (6 * h < 256)
            outputBuffer[ColorChannel_Green] = v1 + lut01[v2 - v1][h * 6];
        else if (2 * h < 256)
            outputBuffer[ColorChannel_Green] = v2;
        else if (3 * h < 512)
            outputBuffer[ColorChannel_Green] = v1 + lut01[v2 - v1][(170 - h) * 6];
        else
            outputBuffer[ColorChannel_Green] = v1;

        vH = h - 85;
        if (vH < 0)
            vH += 256;
        if (6 * vH < 256)
            outputBuffer[ColorChannel_Blue] = v1 + lut01[v2 - v1][vH * 6];
        else if (2 * vH < 256)
            outputBuffer[ColorChannel_Blue] = v2;
        else if (3 * vH < 512)
            outputBuffer[ColorChannel_Blue] = v1 + lut01[v2 - v1][(170 - vH) * 6];
        else
            outputBuffer[ColorChannel_Blue] = v1;

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
