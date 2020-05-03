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

#include <math.h>

#include "imagehistogram.h"
#include "util.h"

namespace ibp {
namespace imgproc {

#define CHHELPER(n) pmf[n].addResponse(pixel[n], w);
#define CHHELPERLUMA pmf[4].addResponse(IBP_pixelIntensity4(pixel[2], pixel[1], pixel[0]), w);

#define CHHELPERFUNCTION(code, content) \
void chHelperFunction##code(const unsigned char *imageData, int width, int height, \
                            int bytesPerLine, int bytesPerPixel, ProbabilityMassFunction * pmf) \
{ \
    register unsigned char * pixel; \
    register int x, y; \
    const double w = 1.; \
    for (y = 0; y < height; y++) \
    { \
        pixel = (unsigned char *)(imageData + (y * bytesPerLine)); \
        for (x = 0; x < width; x++, pixel += bytesPerPixel) \
        { \
            content \
        } \
    } \
} \
\
void chHelperFunctionW##code(const unsigned char *imageData, int width, int height, \
                             int bytesPerLine, int bytesPerPixel, ProbabilityMassFunction * pmf) \
{ \
   register unsigned char * pixel; \
   register int x, y; \
   register double w; \
   for (y = 0; y < height; y++) \
   { \
       pixel = (unsigned char *)(imageData + (y * bytesPerLine)); \
       for (x = 0; x < width; x++, pixel += bytesPerPixel) \
       { \
           w = pixel[3] / 255.; \
           content \
       } \
   } \
} \

CHHELPERFUNCTION(00000, )
CHHELPERFUNCTION(00001, CHHELPER(0))
CHHELPERFUNCTION(00010, CHHELPER(1))
CHHELPERFUNCTION(00011, CHHELPER(0) CHHELPER(1))
CHHELPERFUNCTION(00100, CHHELPER(2))
CHHELPERFUNCTION(00101, CHHELPER(0) CHHELPER(2))
CHHELPERFUNCTION(00110, CHHELPER(1) CHHELPER(2))
CHHELPERFUNCTION(00111, CHHELPER(0) CHHELPER(1) CHHELPER(2))
CHHELPERFUNCTION(01000, CHHELPER(3))
CHHELPERFUNCTION(01001, CHHELPER(0) CHHELPER(3))
CHHELPERFUNCTION(01010, CHHELPER(1) CHHELPER(3))
CHHELPERFUNCTION(01011, CHHELPER(0) CHHELPER(1) CHHELPER(3))
CHHELPERFUNCTION(01100, CHHELPER(2) CHHELPER(3))
CHHELPERFUNCTION(01101, CHHELPER(0) CHHELPER(2) CHHELPER(3))
CHHELPERFUNCTION(01110, CHHELPER(1) CHHELPER(2) CHHELPER(3))
CHHELPERFUNCTION(01111, CHHELPER(0) CHHELPER(1) CHHELPER(2) CHHELPER(3))
CHHELPERFUNCTION(10000, CHHELPERLUMA)
CHHELPERFUNCTION(10001, CHHELPER(0) CHHELPERLUMA)
CHHELPERFUNCTION(10010, CHHELPER(1) CHHELPERLUMA)
CHHELPERFUNCTION(10011, CHHELPER(0) CHHELPER(1) CHHELPERLUMA)
CHHELPERFUNCTION(10100, CHHELPER(2) CHHELPERLUMA)
CHHELPERFUNCTION(10101, CHHELPER(0) CHHELPER(2) CHHELPERLUMA)
CHHELPERFUNCTION(10110, CHHELPER(1) CHHELPER(2) CHHELPERLUMA)
CHHELPERFUNCTION(10111, CHHELPER(0) CHHELPER(1) CHHELPER(2) CHHELPERLUMA)
CHHELPERFUNCTION(11000, CHHELPER(3) CHHELPERLUMA)
CHHELPERFUNCTION(11001, CHHELPER(0) CHHELPER(3) CHHELPERLUMA)
CHHELPERFUNCTION(11010, CHHELPER(1) CHHELPER(3) CHHELPERLUMA)
CHHELPERFUNCTION(11011, CHHELPER(0) CHHELPER(1) CHHELPER(3) CHHELPERLUMA)
CHHELPERFUNCTION(11100, CHHELPER(2) CHHELPER(3) CHHELPERLUMA)
CHHELPERFUNCTION(11101, CHHELPER(0) CHHELPER(2) CHHELPER(3) CHHELPERLUMA)
CHHELPERFUNCTION(11110, CHHELPER(1) CHHELPER(2) CHHELPER(3) CHHELPERLUMA)
CHHELPERFUNCTION(11111, CHHELPER(0) CHHELPER(1) CHHELPER(2) CHHELPER(3) CHHELPERLUMA)


void (* chHelperFunctions[2][32])(const unsigned char *imageData, int width, int height,
                               int bytesPerLine, int bytesPerPixel, ProbabilityMassFunction * pmf) =
{
    {
        chHelperFunction00000, chHelperFunction00001, chHelperFunction00010, chHelperFunction00011,
        chHelperFunction00100, chHelperFunction00101, chHelperFunction00110, chHelperFunction00111,
        chHelperFunction01000, chHelperFunction01001, chHelperFunction01010, chHelperFunction01011,
        chHelperFunction01100, chHelperFunction01101, chHelperFunction01110, chHelperFunction01111,
        chHelperFunction10000, chHelperFunction10001, chHelperFunction10010, chHelperFunction10011,
        chHelperFunction10100, chHelperFunction10101, chHelperFunction10110, chHelperFunction10111,
        chHelperFunction11000, chHelperFunction11001, chHelperFunction11010, chHelperFunction11011,
        chHelperFunction11100, chHelperFunction11101, chHelperFunction11110, chHelperFunction11111
    },
    {
        chHelperFunctionW00000, chHelperFunctionW00001, chHelperFunctionW00010, chHelperFunctionW00011,
        chHelperFunctionW00100, chHelperFunctionW00101, chHelperFunctionW00110, chHelperFunctionW00111,
        chHelperFunctionW01000, chHelperFunctionW01001, chHelperFunctionW01010, chHelperFunctionW01011,
        chHelperFunctionW01100, chHelperFunctionW01101, chHelperFunctionW01110, chHelperFunctionW01111,
        chHelperFunctionW10000, chHelperFunctionW10001, chHelperFunctionW10010, chHelperFunctionW10011,
        chHelperFunctionW10100, chHelperFunctionW10101, chHelperFunctionW10110, chHelperFunctionW10111,
        chHelperFunctionW11000, chHelperFunctionW11001, chHelperFunctionW11010, chHelperFunctionW11011,
        chHelperFunctionW11100, chHelperFunctionW11101, chHelperFunctionW11110, chHelperFunctionW11111
    }
};

ImageHistogram::ImageHistogram() :
    mChannels(None)
{
}

ImageHistogram::~ImageHistogram()
{
}

bool ImageHistogram::computeHistogram(const unsigned char *imageData, int width, int height, Channels c,
                                      int bytesPerLine, int bytesPerPixel, bool alphaWeight)
{
    if (!imageData || width < 1 || height < 1 || bytesPerPixel < 1 || c < 0x01 || c > 0x1F)
        return false;
    if (bytesPerLine < bytesPerPixel * width)
        return false;
    if ((bytesPerPixel == 1 && c != Channel1) ||
        (bytesPerPixel == 2 && c > 0x03) ||
        (bytesPerPixel == 3 && (c & Channel4)))
        return false;

    if (bytesPerPixel < 4)
        alphaWeight = false;

    for (int i = 0; i < 5; i++)
        mProbabilityMassFunctions[i].clearResponses();

    chHelperFunctions[alphaWeight ? 1 : 0][c](imageData, width, height, bytesPerLine,
                                              bytesPerPixel, mProbabilityMassFunctions);

    mChannels = c;

    return true;
}

bool ImageHistogram::hasHistogram(Channels c)
{
    return (mChannels & c);
}

bool ImageHistogram::histogram(ImageHistogram::Channel c, double * h, bool outputProbabilities)
{
    if (!hasHistogram(c) || !h)
        return false;

    ProbabilityMassFunction * pmf = c == Channel1 ? &mProbabilityMassFunctions[0] :
                                    c == Channel2 ? &mProbabilityMassFunctions[1] :
                                    c == Channel3 ? &mProbabilityMassFunctions[2] :
                                    c == Channel4 ? &mProbabilityMassFunctions[3] :
                                    c == Luma     ? &mProbabilityMassFunctions[4] :
                                                    0;
    if (!pmf)
        return false;

    if (outputProbabilities)
        for (int i = 0; i < 256; i++, h++)
            *h = pmf->probability(i);
    else
        for (int i = 0; i < 256; i++, h++)
            *h = pmf->responses(i);

    return true;
}

bool ImageHistogram::cumulativeFunction(ImageHistogram::Channel c, unsigned char * cf, bool preserveBlack)
{
    if (!hasHistogram(c) || !cf)
        return false;

    ProbabilityMassFunction * pmf = c == Channel1 ? &mProbabilityMassFunctions[0] :
                                    c == Channel2 ? &mProbabilityMassFunctions[1] :
                                    c == Channel3 ? &mProbabilityMassFunctions[2] :
                                    c == Channel4 ? &mProbabilityMassFunctions[3] :
                                    c == Luma     ? &mProbabilityMassFunctions[4] :
                                                    0;
    if (!pmf)
        return false;

    double cumulatedProbability = 0.;
    double cumulativeFunction[256];
    for (int i = 0; i < 256; i++)
    {
        cumulatedProbability += pmf->probability(i);
        cumulativeFunction[i] = cumulatedProbability;
    }

    if (preserveBlack)
    {
        double a = cumulativeFunction[0], b = cumulativeFunction[255] - cumulativeFunction[0];
        for (int i = 0; i < 256; i++, cf++)
            *cf = round((cumulativeFunction[i] - a) / b * 255.);
    }
    else
    {
        for (int i = 0; i < 256; i++, cf++)
            *cf = round(cumulativeFunction[i] * 255.);
    }

    return true;
}

double ImageHistogram::responses(Channel c, double r)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].responses(r) :
           c == Channel2 ? mProbabilityMassFunctions[1].responses(r) :
           c == Channel3 ? mProbabilityMassFunctions[2].responses(r) :
           c == Channel4 ? mProbabilityMassFunctions[3].responses(r) :
           c == Luma     ? mProbabilityMassFunctions[4].responses(r) :
                           -1;
}

double ImageHistogram::probability(Channel c, double r)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].probability(r) :
           c == Channel2 ? mProbabilityMassFunctions[1].probability(r) :
           c == Channel3 ? mProbabilityMassFunctions[2].probability(r) :
           c == Channel4 ? mProbabilityMassFunctions[3].probability(r) :
           c == Luma     ? mProbabilityMassFunctions[4].probability(r) :
                           qQNaN();
}

double ImageHistogram::size(Channel c)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].size() :
           c == Channel2 ? mProbabilityMassFunctions[1].size() :
           c == Channel3 ? mProbabilityMassFunctions[2].size() :
           c == Channel4 ? mProbabilityMassFunctions[3].size() :
           c == Luma     ? mProbabilityMassFunctions[4].size() :
                           -1;
}

double ImageHistogram::mean(Channel c)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].mean() :
           c == Channel2 ? mProbabilityMassFunctions[1].mean() :
           c == Channel3 ? mProbabilityMassFunctions[2].mean() :
           c == Channel4 ? mProbabilityMassFunctions[3].mean() :
           c == Luma     ? mProbabilityMassFunctions[4].mean() :
                           qQNaN();
}

double ImageHistogram::variance(Channel c)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].variance() :
           c == Channel2 ? mProbabilityMassFunctions[1].variance() :
           c == Channel3 ? mProbabilityMassFunctions[2].variance() :
           c == Channel4 ? mProbabilityMassFunctions[3].variance() :
           c == Luma     ? mProbabilityMassFunctions[4].variance() :
                           qQNaN();
}

double ImageHistogram::standardDeviation(Channel c)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].standardDeviation() :
           c == Channel2 ? mProbabilityMassFunctions[1].standardDeviation() :
           c == Channel3 ? mProbabilityMassFunctions[2].standardDeviation() :
           c == Channel4 ? mProbabilityMassFunctions[3].standardDeviation() :
           c == Luma     ? mProbabilityMassFunctions[4].standardDeviation() :
                           qQNaN();
}

double ImageHistogram::averageDeviation(Channel c)
    {
        return c == Channel1 ? mProbabilityMassFunctions[0].averageDeviation() :
               c == Channel2 ? mProbabilityMassFunctions[1].averageDeviation() :
               c == Channel3 ? mProbabilityMassFunctions[2].averageDeviation() :
               c == Channel4 ? mProbabilityMassFunctions[3].averageDeviation() :
               c == Luma     ? mProbabilityMassFunctions[4].averageDeviation() :
                               qQNaN();
    }

double ImageHistogram::skewness(Channel c)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].skewness() :
           c == Channel2 ? mProbabilityMassFunctions[1].skewness() :
           c == Channel3 ? mProbabilityMassFunctions[2].skewness() :
           c == Channel4 ? mProbabilityMassFunctions[3].skewness() :
           c == Luma     ? mProbabilityMassFunctions[4].skewness() :
                           qQNaN();
}

double ImageHistogram::kurtosis(Channel c)
    {
        return c == Channel1 ? mProbabilityMassFunctions[0].kurtosis() :
               c == Channel2 ? mProbabilityMassFunctions[1].kurtosis() :
               c == Channel3 ? mProbabilityMassFunctions[2].kurtosis() :
               c == Channel4 ? mProbabilityMassFunctions[3].kurtosis() :
               c == Luma     ? mProbabilityMassFunctions[4].kurtosis() :
                               qQNaN();
    }

double ImageHistogram::excessKurtosis(Channel c)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].excessKurtosis() :
           c == Channel2 ? mProbabilityMassFunctions[1].excessKurtosis() :
           c == Channel3 ? mProbabilityMassFunctions[2].excessKurtosis() :
           c == Channel4 ? mProbabilityMassFunctions[3].excessKurtosis() :
           c == Luma     ? mProbabilityMassFunctions[4].excessKurtosis() :
                           qQNaN();
}

double ImageHistogram::median(Channel c)
    {
        return c == Channel1 ? mProbabilityMassFunctions[0].median() :
               c == Channel2 ? mProbabilityMassFunctions[1].median() :
               c == Channel3 ? mProbabilityMassFunctions[2].median() :
               c == Channel4 ? mProbabilityMassFunctions[3].median() :
               c == Luma     ? mProbabilityMassFunctions[4].median() :
                               qQNaN();
    }

double ImageHistogram::mode(Channel c)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].mode() :
           c == Channel2 ? mProbabilityMassFunctions[1].mode() :
           c == Channel3 ? mProbabilityMassFunctions[2].mode() :
           c == Channel4 ? mProbabilityMassFunctions[3].mode() :
           c == Luma     ? mProbabilityMassFunctions[4].mode() :
                           qQNaN();
}

double ImageHistogram::moment(Channel c, int n)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].moment(n) :
           c == Channel2 ? mProbabilityMassFunctions[1].moment(n) :
           c == Channel3 ? mProbabilityMassFunctions[2].moment(n) :
           c == Channel4 ? mProbabilityMassFunctions[3].moment(n) :
           c == Luma     ? mProbabilityMassFunctions[4].moment(n) :
                           qQNaN();
}

double ImageHistogram::centralMoment(Channel c, int n)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].centralMoment(n) :
           c == Channel2 ? mProbabilityMassFunctions[1].centralMoment(n) :
           c == Channel3 ? mProbabilityMassFunctions[2].centralMoment(n) :
           c == Channel4 ? mProbabilityMassFunctions[3].centralMoment(n) :
           c == Luma     ? mProbabilityMassFunctions[4].centralMoment(n) :
                           qQNaN();
}

double ImageHistogram::standardisedMoment(Channel c, int n)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].standardisedMoment(n) :
           c == Channel2 ? mProbabilityMassFunctions[1].standardisedMoment(n) :
           c == Channel3 ? mProbabilityMassFunctions[2].standardisedMoment(n) :
           c == Channel4 ? mProbabilityMassFunctions[3].standardisedMoment(n) :
           c == Luma     ? mProbabilityMassFunctions[4].standardisedMoment(n) :
           qQNaN();
}

void ImageHistogram::setLimits(ImageHistogram::Channel c, double a, double b)
{
    if (c == Channel1)
        mProbabilityMassFunctions[0].setLimits(a, b);
    else if (c == Channel2)
        mProbabilityMassFunctions[1].setLimits(a, b);
    else if (c == Channel3)
        mProbabilityMassFunctions[2].setLimits(a, b);
    else if (c == Channel4)
        mProbabilityMassFunctions[3].setLimits(a, b);
    else if (c == Luma)
        mProbabilityMassFunctions[4].setLimits(a, b);
}

void ImageHistogram::setLimit(ImageHistogram::Channel c, ProbabilityMassFunction::Limit l, double v)
{
    if (c == Channel1)
        mProbabilityMassFunctions[0].setLimit(l, v);
    else if (c == Channel2)
        mProbabilityMassFunctions[1].setLimit(l, v);
    else if (c == Channel3)
        mProbabilityMassFunctions[2].setLimit(l, v);
    else if (c == Channel4)
        mProbabilityMassFunctions[3].setLimit(l, v);
    else if (c == Luma)
        mProbabilityMassFunctions[4].setLimit(l, v);
}

double ImageHistogram::limit(ImageHistogram::Channel c, ProbabilityMassFunction::Limit l)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].limit(l) :
           c == Channel2 ? mProbabilityMassFunctions[1].limit(l) :
           c == Channel3 ? mProbabilityMassFunctions[2].limit(l) :
           c == Channel4 ? mProbabilityMassFunctions[3].limit(l) :
           c == Luma     ? mProbabilityMassFunctions[4].limit(l) :
           qQNaN();
}

void ImageHistogram::clearLimits(Channel c)
{
    if (c == Channel1)
        mProbabilityMassFunctions[0].clearLimits();
    else if (c == Channel2)
        mProbabilityMassFunctions[1].clearLimits();
    else if (c == Channel3)
        mProbabilityMassFunctions[2].clearLimits();
    else if (c == Channel4)
        mProbabilityMassFunctions[3].clearLimits();
    else if (c == Luma)
        mProbabilityMassFunctions[4].clearLimits();
}

double ImageHistogram::integrate(ImageHistogram::Channel c)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].integrate() :
           c == Channel2 ? mProbabilityMassFunctions[1].integrate() :
           c == Channel3 ? mProbabilityMassFunctions[2].integrate() :
           c == Channel4 ? mProbabilityMassFunctions[3].integrate() :
           c == Luma     ? mProbabilityMassFunctions[4].integrate() :
                           qQNaN();
}

double ImageHistogram::integrate(Channel c, double a, double b)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].integrate(a, b) :
           c == Channel2 ? mProbabilityMassFunctions[1].integrate(a, b) :
           c == Channel3 ? mProbabilityMassFunctions[2].integrate(a, b) :
           c == Channel4 ? mProbabilityMassFunctions[3].integrate(a, b) :
           c == Luma     ? mProbabilityMassFunctions[4].integrate(a, b) :
    qQNaN();
}

double ImageHistogram::integrate2(ImageHistogram::Channel c, double b)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].integrate2(b) :
           c == Channel2 ? mProbabilityMassFunctions[1].integrate2(b) :
           c == Channel3 ? mProbabilityMassFunctions[2].integrate2(b) :
           c == Channel4 ? mProbabilityMassFunctions[3].integrate2(b) :
           c == Luma     ? mProbabilityMassFunctions[4].integrate2(b) :
                           qQNaN();
}

double ImageHistogram::integrate2(Channel c, double a, double b)
{
    return c == Channel1 ? mProbabilityMassFunctions[0].integrate2(a, b) :
           c == Channel2 ? mProbabilityMassFunctions[1].integrate2(a, b) :
           c == Channel3 ? mProbabilityMassFunctions[2].integrate2(a, b) :
           c == Channel4 ? mProbabilityMassFunctions[3].integrate2(a, b) :
           c == Luma     ? mProbabilityMassFunctions[4].integrate2(a, b) :
                           qQNaN();
}

}}
