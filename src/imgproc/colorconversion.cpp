#include <QResource>
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
    cmsHPROFILE LabProfile = cmsOpenProfileFromMem(res.data(), res.size());
    cmsHPROFILE CMYKProfile = cmsCreateLab4Profile(0);

    BGRToLabTransform = cmsCreateTransform(BGRProfile, TYPE_ARGB_8, LabProfile, TYPE_Lab_8, 0, 0);
    BGRToCMYKTransform = cmsCreateTransform(BGRProfile, TYPE_ARGB_8, CMYKProfile, TYPE_CMYK_8, 0, 0);
    LabToBGRTransform = cmsCreateTransform(LabProfile, TYPE_Lab_8, BGRProfile, TYPE_ARGB_8, 0, 0);
    LabToBGRTransform = cmsCreateTransform(LabProfile, TYPE_Lab_8, BGRProfile, TYPE_CMYK_8, 0, 0);
    CMYKToBGRTransform = cmsCreateTransform(CMYKProfile, TYPE_CMYK_8, BGRProfile, TYPE_ARGB_8, 0, 0);
    CMYKToLabTransform = cmsCreateTransform(CMYKProfile, TYPE_CMYK_8, LabProfile, TYPE_Lab_8, 0, 0);

    cmsCloseProfile(BGRProfile);
    cmsCloseProfile(LabProfile);
    cmsCloseProfile(CMYKProfile);
}

void (* convertColor[5][5])(const unsigned char * inputColor, unsigned char * outputColor) =
{
{ convertIdentity, convertBGRToHSV, convertBGRToHSL, convertBGRToLab, convertBGRToCMYK },
{ convertHSVToBGR, convertIdentity, convertHSVToHSL, convertHSVToLab, convertHSVToCMYK },
{ convertHSLToBGR, convertHSLToHSV, convertIdentity, convertHSLToLab, convertHSLToCMYK },
{ convertLabToBGR, convertLabToHSV, convertLabToHSL, convertIdentity, convertLabToCMYK },
{ convertCMYKToBGR, convertCMYKToHSV, convertCMYKToHSL, convertCMYKToLab, convertIdentity },
};

void convertBGRToHSV(const unsigned char *inputColor, unsigned char *outputColor)
{
    const int min = AT_minimum3(inputColor[ColorChannel_Blue], inputColor[ColorChannel_Green],
                                inputColor[ColorChannel_Red]);
    const int max = AT_maximum3(inputColor[ColorChannel_Blue], inputColor[ColorChannel_Green],
                                inputColor[ColorChannel_Red]);
    const int dMax = max - min;
    int h, s, v;

    if (dMax == 0)
    {
        outputColor[ColorChannel_Saturation] = 0;
        outputColor[ColorChannel_Value] = max;
        return;
    }

    v = max;
    s = lut02[dMax][max];

    const int dMaxOver2 = dMax >> 1;
    if (max == inputColor[ColorChannel_Red])
        h = lut02[lut01[(max - inputColor[ColorChannel_Blue])][42] + dMaxOver2][max] -
            lut02[lut01[(max - inputColor[ColorChannel_Green])][42] + dMaxOver2][max];
    else if (max == inputColor[ColorChannel_Green])
        h = lut02[lut01[(max - inputColor[ColorChannel_Red])][42] + dMaxOver2][max] -
            lut02[lut01[(max - inputColor[ColorChannel_Blue])][42] + dMaxOver2][max] + 85;
    else
        h = lut02[lut01[(max - inputColor[ColorChannel_Green])][42] + dMaxOver2][max] -
            lut02[lut01[(max - inputColor[ColorChannel_Red])][42] + dMaxOver2][max] + 170;

    outputColor[ColorChannel_Hue] = h < 0 ? h + 256 : h > 255 ? h < 256 : h;
    outputColor[ColorChannel_Saturation] = s;
    outputColor[ColorChannel_Value] = v;
}


void convertBGRToHSL(const unsigned char *inputColor, unsigned char *outputColor)
{
    const int min = AT_minimum3(inputColor[ColorChannel_Blue], inputColor[ColorChannel_Green],
                                inputColor[ColorChannel_Red]);
    const int max = AT_maximum3(inputColor[ColorChannel_Blue], inputColor[ColorChannel_Green],
                                inputColor[ColorChannel_Red]);
    const int dMax = max - min;
    const int minPlusMax = min + max;
    int h, s, l;

    if (dMax == 0)
    {
        outputColor[ColorChannel_Saturation] = 0;
        outputColor[ColorChannel_Lightness] = minPlusMax >> 1;
        return;
    }

    l = minPlusMax >> 1;
    s = l < 128 ? lut02[dMax][minPlusMax] : lut02[dMax][511 - minPlusMax];

    const int dMaxOver2 = dMax >> 1;
    if (max == inputColor[ColorChannel_Red])
        h = lut02[lut01[(max - inputColor[ColorChannel_Blue])][42] + dMaxOver2][max] -
            lut02[lut01[(max - inputColor[ColorChannel_Green])][42] + dMaxOver2][max];
    else if (max == inputColor[ColorChannel_Green])
        h = lut02[lut01[(max - inputColor[ColorChannel_Red])][42] + dMaxOver2][max] -
            lut02[lut01[(max - inputColor[ColorChannel_Blue])][42] + dMaxOver2][max] + 85;
    else
        h = lut02[lut01[(max - inputColor[ColorChannel_Green])][42] + dMaxOver2][max] -
            lut02[lut01[(max - inputColor[ColorChannel_Red])][42] + dMaxOver2][max] + 170;

    outputColor[ColorChannel_Hue] = h < 0 ? h + 256 : h > 255 ? h - 256 : h;
    outputColor[ColorChannel_Saturation] = s;
    outputColor[ColorChannel_Lightness] = l;
}


void convertBGRToLab(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(BGRToLabTransform, inputColor, outputColor, 1);
}


void convertBGRToCMYK(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(BGRToCMYKTransform, inputColor, outputColor, 1);
}


void convertHSVToBGR(const unsigned char *inputColor, unsigned char *outputColor)
{
    if (inputColor[ColorChannel_Saturation] == 0)
    {
        outputColor[ColorChannel_Blue] = outputColor[ColorChannel_Green] = outputColor[ColorChannel_Red] =
                inputColor[ColorChannel_Value];
        return;
    }

    const int i = (inputColor[ColorChannel_Hue] + 1) * 6;
    const int v0 = i - (i >> 8 << 8);
    const int v1 = lut01[inputColor[ColorChannel_Value]][255 - inputColor[ColorChannel_Saturation]];
    const int v2 = lut01[inputColor[ColorChannel_Value]][lut01[255 - inputColor[ColorChannel_Saturation]][v0]];
    const int v3 = lut01[inputColor[ColorChannel_Value]][lut01[255 - inputColor[ColorChannel_Saturation]][255 - v0]];

    if (i > 213)
    {
        outputColor[ColorChannel_Red] = inputColor[ColorChannel_Value];
        outputColor[ColorChannel_Green] = v1;
        outputColor[ColorChannel_Blue] = v2;
    }
    else if (i > 170)
    {
        outputColor[ColorChannel_Red] = v3;
        outputColor[ColorChannel_Green] = v1;
        outputColor[ColorChannel_Blue] = inputColor[ColorChannel_Value];
    }
    else if (i > 127)
    {
        outputColor[ColorChannel_Red] = v1;
        outputColor[ColorChannel_Green] = v2;
        outputColor[ColorChannel_Blue] = inputColor[ColorChannel_Value];
    }
    else if (i > 85)
    {
        outputColor[ColorChannel_Red] = v1;
        outputColor[ColorChannel_Green] = inputColor[ColorChannel_Value];
        outputColor[ColorChannel_Blue] = v3;
    }
    else if (i > 42)
    {
        outputColor[ColorChannel_Red] = v2;
        outputColor[ColorChannel_Green] = inputColor[ColorChannel_Value];
        outputColor[ColorChannel_Blue] = v1;
    }
    else
    {
        outputColor[ColorChannel_Red] = inputColor[ColorChannel_Value];
        outputColor[ColorChannel_Green] = v3 ;
        outputColor[ColorChannel_Blue] = v1;
    }
}


void convertHSVToHSL(const unsigned char *inputColor, unsigned char *outputColor)
{
    const int dMax = lut01[inputColor[ColorChannel_Saturation]][inputColor[ColorChannel_Value]];
    const int minPlusMax = inputColor[ColorChannel_Value] - dMax + inputColor[ColorChannel_Value];
    outputColor[ColorChannel_Value] = minPlusMax >> 1;
    outputColor[ColorChannel_Saturation] = outputColor[ColorChannel_Value] < 128 ?
                lut02[dMax][minPlusMax] : lut02[dMax][511 - minPlusMax];
    outputColor[ColorChannel_Hue] = inputColor[ColorChannel_Hue];
}


void convertHSVToLab(const unsigned char *inputColor, unsigned char *outputColor)
{
    convertHSVToBGR(inputColor, outputColor);
    cmsDoTransform(BGRToLabTransform, outputColor, outputColor, 1);
}


void convertHSVToCMYK(const unsigned char *inputColor, unsigned char *outputColor)
{
    convertHSVToBGR(inputColor, outputColor);
    cmsDoTransform(BGRToCMYKTransform, outputColor, outputColor, 1);
}


void convertHSLToBGR(const unsigned char *inputColor, unsigned char *outputColor)
{
    if (inputColor[ColorChannel_Saturation] == 0)
    {
        outputColor[ColorChannel_Blue] = outputColor[ColorChannel_Green] = outputColor[ColorChannel_Red] =
                inputColor[ColorChannel_Lightness];
        return;
    }

    const int v2 = (inputColor[ColorChannel_Lightness] < 128) ?
                    inputColor[ColorChannel_Lightness] * (255 + inputColor[ColorChannel_Saturation]) / 255 :
                    inputColor[ColorChannel_Lightness] + inputColor[ColorChannel_Saturation] -
                    lut01[inputColor[ColorChannel_Saturation]][inputColor[ColorChannel_Lightness]];
    const int v1 = (inputColor[ColorChannel_Lightness] << 1) - v2;

    const int h = inputColor[ColorChannel_Hue];

    int vH = h + 85;
    if (vH > 255)
        vH -= 256;
    if (6 * vH < 256)
        outputColor[ColorChannel_Red] = v1 + lut01[v2 - v1][vH * 6];
    else if (2 * vH < 256)
        outputColor[ColorChannel_Red] = v2;
    else if (3 * vH < 512)
        outputColor[ColorChannel_Red] = v1 + lut01[v2 - v1][(170 - vH) * 6];
    else
        outputColor[ColorChannel_Red] = v1;

    if (6 * h < 256)
        outputColor[ColorChannel_Green] = v1 + lut01[v2 - v1][h * 6];
    else if (2 * h < 256)
        outputColor[ColorChannel_Green] = v2;
    else if (3 * h < 512)
        outputColor[ColorChannel_Green] = v1 + lut01[v2 - v1][(170 - h) * 6];
    else
        outputColor[ColorChannel_Green] = v1;

    vH = h - 85;
    if (vH < 0)
        vH += 256;
    if (6 * vH < 256)
        outputColor[ColorChannel_Blue] = v1 + lut01[v2 - v1][vH * 6];
    else if (2 * vH < 256)
        outputColor[ColorChannel_Blue] = v2;
    else if (3 * vH < 512)
        outputColor[ColorChannel_Blue] = v1 + lut01[v2 - v1][(170 - vH) * 6];
    else
        outputColor[ColorChannel_Blue] = v1;
}


void convertHSLToHSV(const unsigned char *inputColor, unsigned char *outputColor)
{
    const int minPlusMax = inputColor[ColorChannel_Lightness] << 1;
    const int dMax = inputColor[ColorChannel_Lightness] < 128 ?
                     lut01[minPlusMax][inputColor[ColorChannel_Saturation]] :
                     lut01[511 - minPlusMax][inputColor[ColorChannel_Saturation]];
    outputColor[ColorChannel_Value] = (minPlusMax + dMax) >> 1;
    outputColor[ColorChannel_Saturation] = lut02[dMax][outputColor[ColorChannel_Value]];
    outputColor[ColorChannel_Hue] = inputColor[ColorChannel_Hue];
}


void convertHSLToLab(const unsigned char *inputColor, unsigned char *outputColor)
{
    convertHSLToBGR(inputColor, outputColor);
    cmsDoTransform(BGRToLabTransform, outputColor, outputColor, 1);
}


void convertHSLToCMYK(const unsigned char *inputColor, unsigned char *outputColor)
{
    convertHSLToBGR(inputColor, outputColor);
    cmsDoTransform(BGRToCMYKTransform, outputColor, outputColor, 1);
}


void convertLabToBGR(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(LabToBGRTransform, inputColor, outputColor, 1);
}


void convertLabToHSV(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(LabToBGRTransform, inputColor, outputColor, 1);
    convertBGRToHSV(outputColor, outputColor);
}


void convertLabToHSL(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(LabToBGRTransform, inputColor, outputColor, 1);
    convertBGRToHSL(outputColor, outputColor);
}


void convertLabToCMYK(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(LabToCMYKTransform, inputColor, outputColor, 1);
}


void convertCMYKToBGR(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(CMYKToBGRTransform, inputColor, outputColor, 1);
}


void convertCMYKToHSV(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(CMYKToBGRTransform, inputColor, outputColor, 1);
    convertBGRToHSV(outputColor, outputColor);
}


void convertCMYKToHSL(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(CMYKToBGRTransform, inputColor, outputColor, 1);
    convertBGRToHSL(outputColor, outputColor);
}


void convertCMYKToLab(const unsigned char *inputColor, unsigned char *outputColor)
{
    cmsDoTransform(CMYKToLabTransform, inputColor, outputColor, 1);
}


void convertIdentity(const unsigned char *inputColor, unsigned char *outputColor)
{
    *((unsigned int *)outputColor) = *((unsigned int *)inputColor);
}

void (* swapChannels[4][4])(unsigned char * inputColor) =
{
{ swapChannelsIdentity, swapChannels01, swapChannels02, swapChannels03 },
{ swapChannels01, swapChannelsIdentity, swapChannels12, swapChannels13 },
{ swapChannels02, swapChannels12, swapChannelsIdentity, swapChannels23 },
{ swapChannels03, swapChannels13, swapChannels23, swapChannelsIdentity },
};

inline void swapChannels01(unsigned char * inputColor)
{
    unsigned char t = inputColor[0];
    inputColor[0] = inputColor[1];
    inputColor[1] = t;
}

inline void swapChannels02(unsigned char * inputColor)
{
    unsigned char t = inputColor[0];
    inputColor[0] = inputColor[2];
    inputColor[2] = t;
}

inline void swapChannels03(unsigned char * inputColor)
{
    unsigned char t = inputColor[0];
    inputColor[0] = inputColor[3];
    inputColor[3] = t;
}

inline void swapChannels12(unsigned char * inputColor)
{
    unsigned char t = inputColor[1];
    inputColor[1] = inputColor[2];
    inputColor[2] = t;
}

inline void swapChannels13(unsigned char * inputColor)
{
    unsigned char t = inputColor[1];
    inputColor[1] = inputColor[3];
    inputColor[3] = t;
}

inline void swapChannels23(unsigned char * inputColor)
{
    unsigned char t = inputColor[2];
    inputColor[2] = inputColor[3];
    inputColor[3] = t;
}

inline void swapChannelsIdentity(unsigned char * inputColor)
{
    Q_UNUSED(inputColor)
}

}}
