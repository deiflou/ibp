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

#include <opencv2/imgproc.hpp>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/types.h>
#include <misc/util.h>

#define EXPONENTIALSIGMOIDSIZE 7.5
#define THRESHOLDSIGMOIDSIZE 128

using namespace ibp::imgproc;

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

extern "C" QHash<QString, QString> getIBPPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getIBPPluginInfo();
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

        bits82->r = IBP_clamp(0, bits81->r + diffR * amount * mThresholdLut[abs(diffR)] / 25500, 255);
        bits82->g = IBP_clamp(0, bits81->g + diffG * amount * mThresholdLut[abs(diffG)] / 25500, 255);
        bits82->b = IBP_clamp(0, bits81->b + diffB * amount * mThresholdLut[abs(diffB)] / 25500, 255);

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
