/****************************************************************************
**
** Copyright (C) 2015 Deif Lou
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

#ifndef ANITOOLS_IMGPROC_IMAGEHISTOGRAM_H
#define ANITOOLS_IMGPROC_IMAGEHISTOGRAM_H

#include <QImage>

#include "../misc/probabilitymassfunction.h"

namespace anitools {
namespace imgproc {

using namespace anitools::misc;

class ImageHistogram
{
public:
    enum Channel
    {
        None = 0,
        Channel1 = 1, Channel2 = 2, Channel3 = 4, Channel4 = 8,
        Luma = 16,
        Blue = Channel1, Green = Channel2, Red = Channel3, Alpha = Channel4,
        Hue = Channel1, Saturation = Channel2, Value = Channel3, Lightness = Channel3,
        CIEL = Channel1, CIEa = Channel2, CIEb = Channel3,
        Cyan = Channel1, Magenta = Channel2, Yellow = Channel3, Black = Channel4,
        RGB = Red | Green | Blue,
        RGBA = Red | Green | Blue | Alpha,
        LumaAndRGB = Luma | Red | Green | Blue,
        LumaAndRGBA = Luma | Red | Green | Blue,
        CIELab = CIEL | CIEa | CIEb,
        CMYK = Cyan | Magenta | Yellow | Black
    };
    Q_DECLARE_FLAGS(Channels, Channel)

    ImageHistogram();
    ~ImageHistogram();

    bool computeHistogram(const unsigned char * imageData, int width, int height, Channels c,
                          int bytesPerLine, int bytesPerPixel = 4, bool alphaWeight = true);
    bool hasHistogram(Channels c);
    bool histogram(Channel c, double * h, bool outputProbabilities = true);
    bool cumulativeFunction(Channel c, unsigned char *cf, bool preserveBlack = true);

    double responses(Channel c, double r);
    double probability(Channel c, double r);
    double size(Channel c);

    double mean(Channel c);
    double variance(Channel c);
    double standardDeviation(Channel c);
    double averageDeviation(Channel c);
    double skewness(Channel c);
    double kurtosis(Channel c);
    double excessKurtosis(Channel c);
    double median(Channel c);
    double mode(Channel c);
    double moment(Channel c, int n);
    double centralMoment(Channel c, int n);
    double standardisedMoment(Channel c, int n);

    void setLimits(Channel c, double a, double b);
    void setLimit(Channel c, ProbabilityMassFunction::Limit l, double v);
    double limit(Channel c, ProbabilityMassFunction::Limit l);
    void clearLimits(Channel c);

    double integrate(Channel c);
    double integrate(Channel c, double a, double b);
    double integrate2(Channel c, double b);
    double integrate2(Channel c, double a, double b);

private:
    Channels mChannels;
    ProbabilityMassFunction mProbabilityMassFunctions[5];
};

}}
#endif // ANITOOLS_IMGPROC_IMAGEHISTOGRAM_H
