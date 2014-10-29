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

#include <QTime>
#include <math.h>

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"
#include "../misc/util.h"

using namespace anitools::imgproc;

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

extern "C" QHash<QString, QString> getAnitoolsPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getAnitoolsPluginInfo();
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
    const double minus_2_times_log_of_RAND_MAX_plus_1 = -2. * log (RAND_MAX + 1);
    const double AT_2PI_over_RAND_MAX = AT_2PI / RAND_MAX;

    qsrand(mSeed);

    if (mColorMode == Monochromatic)
    {
        if (mDistribution == Uniform)
        {
            while (totalPixels--)
            {
                r = 2 * qrand() * amount / RAND_MAX - amount;
                dstBits->r = AT_clamp(0, srcBits->r + r, 255);
                dstBits->g = AT_clamp(0, srcBits->g + r, 255);
                dstBits->b = AT_clamp(0, srcBits->b + r, 255);
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
                         cos(qrand() * AT_2PI_over_RAND_MAX) * amount;
                dstBits->r = AT_clamp(0, srcBits->r + r, 255);
                dstBits->g = AT_clamp(0, srcBits->g + r, 255);
                dstBits->b = AT_clamp(0, srcBits->b + r, 255);
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
                r = 2 * qrand() * amount / RAND_MAX - amount;
                g = 2 * qrand() * amount / RAND_MAX - amount;
                b = 2 * qrand() * amount / RAND_MAX - amount;
                dstBits->r = AT_clamp(0, srcBits->r + r, 255);
                dstBits->g = AT_clamp(0, srcBits->g + g, 255);
                dstBits->b = AT_clamp(0, srcBits->b + b, 255);
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
                         cos(qrand() * AT_2PI_over_RAND_MAX) * amount;
                g = sqrt(-2. * log(qrand() + 1) - minus_2_times_log_of_RAND_MAX_plus_1) *
                         cos(qrand() * AT_2PI_over_RAND_MAX) * amount;
                b = sqrt(-2. * log(qrand() + 1) - minus_2_times_log_of_RAND_MAX_plus_1) *
                         cos(qrand() * AT_2PI_over_RAND_MAX) * amount;
                dstBits->r = AT_clamp(0, srcBits->r + r, 255);
                dstBits->g = AT_clamp(0, srcBits->g + g, 255);
                dstBits->b = AT_clamp(0, srcBits->b + b, 255);
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
