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

#ifndef IBP_IMGPROC_IMAGEHISTOGRAM_H
#define IBP_IMGPROC_IMAGEHISTOGRAM_H

#include <QImage>

#include "../misc/probabilitymassfunction.h"

namespace ibp {
namespace imgproc {

using namespace ibp::misc;

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
#endif // IBP_IMGPROC_IMAGEHISTOGRAM_H
