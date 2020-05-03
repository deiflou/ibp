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

#include <QTime>
#include <math.h>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/types.h>
#include <misc/util.h>

using namespace ibp::imgproc;

Filter::Filter() :
    mAmount(0.0),
    mDistribution(Uniform),
    mColorMode(Monochromatic)
{
    mSeed = (unsigned int)QTime::currentTime().msec();
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mAmount = mAmount;
    f->mDistribution = mDistribution;
    f->mColorMode = mColorMode;
    f->mSeed = mSeed;
    return f;
}

extern "C" QHash<QString, QString> getIBPPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getIBPPluginInfo();
}

QImage Filter::process(const QImage &inputImage)
{
    if (inputImage.isNull() || inputImage.format() != QImage::Format_ARGB32)
        return inputImage;

    if (qFuzzyIsNull(mAmount))
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    register int totalPixels = inputImage.width() * inputImage.height();
    register BGRA * srcBits = (BGRA *)inputImage.bits();
    register BGRA * dstBits = (BGRA *)i.bits();
    register int r, g, b;
    register int amount = mAmount * 255 / 100;
    const double minus_2_times_log_of_RAND_MAX_plus_1 = -2. * log (RAND_MAX + 1.0);
    const double IBP_2PI_over_RAND_MAX = IBP_2PI / RAND_MAX;

    qsrand(mSeed);

    if (mColorMode == Monochromatic)
    {
        if (mDistribution == Uniform)
        {
            while (totalPixels--)
            {
                r = 2 * (long)qrand() * amount / RAND_MAX - amount;
                dstBits->r = IBP_clamp(0, srcBits->r + r, 255);
                dstBits->g = IBP_clamp(0, srcBits->g + r, 255);
                dstBits->b = IBP_clamp(0, srcBits->b + r, 255);
                dstBits->a = srcBits->a;
                srcBits++;
                dstBits++;
            }
        }
        else
        {
            while (totalPixels--)
            {
                r = sqrt(-2. * log(qrand() + 1) - minus_2_times_log_of_RAND_MAX_plus_1) *
                         cos(qrand() * IBP_2PI_over_RAND_MAX) * amount;
                dstBits->r = IBP_clamp(0, srcBits->r + r, 255);
                dstBits->g = IBP_clamp(0, srcBits->g + r, 255);
                dstBits->b = IBP_clamp(0, srcBits->b + r, 255);
                dstBits->a = srcBits->a;
                srcBits++;
                dstBits++;
            }
        }
    }
    else
    {
        if (mDistribution == Uniform)
        {
            while (totalPixels--)
            {
                r = 2 * (long)qrand() * amount / RAND_MAX - amount;
                g = 2 * (long)qrand() * amount / RAND_MAX - amount;
                b = 2 * (long)qrand() * amount / RAND_MAX - amount;
                dstBits->r = IBP_clamp(0, srcBits->r + r, 255);
                dstBits->g = IBP_clamp(0, srcBits->g + g, 255);
                dstBits->b = IBP_clamp(0, srcBits->b + b, 255);
                dstBits->a = srcBits->a;
                srcBits++;
                dstBits++;
            }
        }
        else
        {
            while (totalPixels--)
            {
                r = sqrt(-2. * log(qrand() + 1) - minus_2_times_log_of_RAND_MAX_plus_1) *
                         cos(qrand() * IBP_2PI_over_RAND_MAX) * amount;
                g = sqrt(-2. * log(qrand() + 1) - minus_2_times_log_of_RAND_MAX_plus_1) *
                         cos(qrand() * IBP_2PI_over_RAND_MAX) * amount;
                b = sqrt(-2. * log(qrand() + 1) - minus_2_times_log_of_RAND_MAX_plus_1) *
                         cos(qrand() * IBP_2PI_over_RAND_MAX) * amount;
                dstBits->r = IBP_clamp(0, srcBits->r + r, 255);
                dstBits->g = IBP_clamp(0, srcBits->g + g, 255);
                dstBits->b = IBP_clamp(0, srcBits->b + b, 255);
                dstBits->a = srcBits->a;
                srcBits++;
                dstBits++;
            }
        }
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    double amount;
    QString distributionStr, colormodeStr;
    Distribution distribution;
    ColorMode colormode;
    unsigned int seed;
    bool ok;

    amount = s.value("amount", 0.0).toDouble(&ok);
    if (!ok || amount < 0. || amount > 400.)
        return false;

    distributionStr = s.value("distribution", "uniform").toString();
    if (distributionStr == "uniform")
        distribution = Uniform;
    else if (distributionStr == "gaussian")
        distribution = Gaussian;
    else
        return false;

    colormodeStr = s.value("colormode", "monochromatic").toString();
    if (colormodeStr == "monochromatic")
        colormode = Monochromatic;
    else if (colormodeStr == "color")
        colormode = Color;
    else
        return false;

    seed = s.value("seed", 0).toUInt(&ok);
    if (!ok)
        return false;

    mSeed = seed;
    setAmount(amount);
    setDistribution(distribution);
    setColorMode(colormode);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("amount", mAmount);
    s.setValue("distribution", mDistribution == Uniform ? "uniform" : "gaussian");
    s.setValue("colormode", mColorMode == Monochromatic ? "monochromatic" : "color");
    s.setValue("seed", mSeed);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setAmount(mAmount);
    fw->setDistribution(mDistribution);
    fw->setColorMode(mColorMode);
    connect(this, SIGNAL(amountChanged(double)), fw, SLOT(setAmount(double)));
    connect(this, SIGNAL(distributionChanged(Filter::Distribution)),
            fw, SLOT(setDistribution(Filter::Distribution)));
    connect(this, SIGNAL(colorModeChanged(Filter::ColorMode)), fw, SLOT(setColorMode(Filter::ColorMode)));
    connect(fw, SIGNAL(amountChanged(double)), this, SLOT(setAmount(double)));
    connect(fw, SIGNAL(distributionChanged(Filter::Distribution)),
            this, SLOT(setDistribution(Filter::Distribution)));
    connect(fw, SIGNAL(colorModeChanged(Filter::ColorMode)), this, SLOT(setColorMode(Filter::ColorMode)));
    return fw;
}

void Filter::setAmount(double v)
{
    if (qFuzzyCompare(v, mAmount))
        return;
    mAmount = v;
    emit amountChanged(v);
    emit parametersChanged();
}

void Filter::setDistribution(Filter::Distribution v)
{
    if (v == mDistribution)
        return;
    mDistribution = v;
    emit distributionChanged(v);
    emit parametersChanged();
}

void Filter::setColorMode(Filter::ColorMode v)
{
    if (v == mColorMode)
        return;
    mColorMode = v;
    emit colorModeChanged(v);
    emit parametersChanged();
}
