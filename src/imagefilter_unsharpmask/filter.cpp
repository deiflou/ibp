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

#include <opencv2/imgproc.hpp>

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"
#include "../misc/util.h"

#define EXPONENTIALSIGMOIDSIZE 7.5
#define THRESHOLDSIGMOIDSIZE 128

using namespace anitools::imgproc;

Filter::Filter() :
    mRadius(0.0),
    mAmount(0),
    mThreshold(0)
{
    makeThresholdLut();
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mRadius = mRadius;
    f->mAmount = mAmount;
    f->mThreshold = mThreshold;
    f->makeThresholdLut();
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

    if (qFuzzyIsNull(mRadius))
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);

    cv::Mat mInput(inputImage.height(), inputImage.width(), CV_8UC4, (void *)inputImage.bits());
    cv::Mat mBlurred(i.height(), i.width(), CV_8UC4, i.bits());

    double sigma = (mRadius + .5) / 2.45;
    cv::GaussianBlur(mInput, mBlurred, cv::Size(0, 0), sigma);

    BGRA * bits81 = (BGRA *)inputImage.bits();
    BGRA * bits82 = (BGRA *)i.bits();
    register int totalPixels = inputImage.height() * inputImage.width();
    register int diffR, diffG, diffB;
    register int amount = mAmount;

    while (totalPixels--)
    {
        diffR = bits81->r - bits82->r;
        diffG = bits81->g - bits82->g;
        diffB = bits81->b - bits82->b;

        bits82->r = AT_clamp(0, bits81->r + diffR * amount * mThresholdLut[abs(diffR)] / 25500, 255);
        bits82->g = AT_clamp(0, bits81->g + diffG * amount * mThresholdLut[abs(diffG)] / 25500, 255);
        bits82->b = AT_clamp(0, bits81->b + diffB * amount * mThresholdLut[abs(diffB)] / 25500, 255);

        bits82->a = bits81->a;

        bits81++;
        bits82++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    double radius;
    int amount;
    int threshold;
    bool ok;
    radius = s.value("radius", 0.0).toDouble(&ok);
    if (!ok || radius < 0 || radius > 100)
        return false;
    amount = s.value("amount", 0).toInt(&ok);
    if (!ok || amount < 0 || amount > 500)
        return false;
    threshold = s.value("threshold", 0).toInt(&ok);
    if (!ok || threshold < 0 || threshold > 255)
        return false;
    setRadius(radius);
    setAmount(amount);
    setThreshold(threshold);
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("radius", mRadius);
    s.setValue("amount", mAmount);
    s.setValue("threshold", mThreshold);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setRadius(mRadius);
    fw->setAmount(mAmount);
    fw->setThreshold(mThreshold);
    connect(this, SIGNAL(radiusChanged(double)), fw, SLOT(setRadius(double)));
    connect(this, SIGNAL(amountChanged(int)), fw, SLOT(setAmount(int)));
    connect(this, SIGNAL(thresholdChanged(int)), fw, SLOT(setThreshold(int)));
    connect(fw, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
    connect(fw, SIGNAL(amountChanged(int)), this, SLOT(setAmount(int)));
    connect(fw, SIGNAL(thresholdChanged(int)), this, SLOT(setThreshold(int)));
    return fw;
}

void Filter::makeThresholdLut()
{
    for (int i = 0; i < 256; i++)
        mThresholdLut[i] = round(255. / (1. + exp(-EXPONENTIALSIGMOIDSIZE * 2. *
                                                  (i - mThreshold) / THRESHOLDSIGMOIDSIZE)));
}

void Filter::setRadius(double v)
{
    if (v == mRadius)
        return;
    mRadius = v;
    emit radiusChanged(v);
    emit parametersChanged();
}

void Filter::setAmount(int v)
{
    if (v == mAmount)
        return;
    mAmount = v;
    emit amountChanged(v);
    emit parametersChanged();
}

void Filter::setThreshold(int v)
{
    if (v == mThreshold)
        return;
    mThreshold = v;
    makeThresholdLut();
    emit thresholdChanged(v);
    emit parametersChanged();
}
