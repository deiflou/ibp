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

#include <math.h>

#include "filter.h"
#include "../imgproc/types.h"

Filter::Filter()
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    return new Filter();
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

    double histogram[256];
    register int totalPixels;
    register double totalWeightedPixels;
    register double cumulatedPixels;
    double weightedLUT[256];
    register int gray;
    register BGRA * srcBits;
    register BGRA * dstBits;
    register unsigned int gR = .2126 * 0x10000;
    register unsigned int gG = .7152 * 0x10000;
    register unsigned int gB = .0722 * 0x10000;
    QImage i(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    unsigned char mLut[256];

    memset(histogram, 0, 256 * sizeof(double));
    totalPixels = inputImage.width() * inputImage.height();
    totalWeightedPixels = 0.0;
    srcBits = (BGRA*)inputImage.bits();

    // Make Weighted alpha LUT
    for (int i = 0; i < 256; i++)
        weightedLUT[i] = pow(i / 255.0, 4);

    // Make histogram
    for (int i = 0; i < totalPixels; i++)
    {
        gray = (srcBits->r * gR >> 16) + (srcBits->g * gG >> 16) + (srcBits->b * gB >> 16);
        histogram[gray] += weightedLUT[srcBits->a];
        totalWeightedPixels += weightedLUT[srcBits->a];
        srcBits++;
    }

    // Calculate cumulative function
    cumulatedPixels = 0.0;
    for (int i = 0; i < 256; i++)
    {
        cumulatedPixels += histogram[i];
        mLut[i] = round(cumulatedPixels * 255.0 / totalWeightedPixels);
    }

    // Apply cumulative function
    srcBits = (BGRA*)inputImage.bits();
    dstBits = (BGRA*)i.bits();
    for (int i = 0; i < totalPixels; i++)
    {
        dstBits->r = mLut[srcBits->r];
        dstBits->g = mLut[srcBits->g];
        dstBits->b = mLut[srcBits->b];
        dstBits->a = srcBits->a;
        srcBits++;
        dstBits++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    Q_UNUSED(parent)
    return 0;
}
