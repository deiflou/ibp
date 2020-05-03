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

#include <opencv2/photo.hpp>
#include <vector>

#include "filter.h"
#include "filterwidget.h"
#include <misc/util.h>

Filter::Filter() :
    mStrength(0.),
    mIterations(30)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mStrength = mStrength;
    f->mIterations = mIterations;
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

    if (qFuzzyCompare(mStrength, 0.))
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    cv::Mat mSrc(inputImage.height(), inputImage.width(), CV_8UC4, (void *)inputImage.bits());
    cv::Mat mDst(i.height(), i.width(), CV_8UC4, i.bits());
    cv::Mat mRed(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mGreen(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mBlue(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mAlpha(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mRedDenoised, mGreenDenoised, mBlueDenoised;
    double lambda = pow(IBP_maximum(100. - mStrength, .001) / 100., 3) * 10.;
    int fromTo[] = { 0, 0, 1, 1, 2, 2, 3, 3 };

    // split the image channels
    cv::Mat mOutSplit[] = { mBlue, mGreen, mRed, mAlpha };
    cv::mixChannels(&mSrc, 1, mOutSplit, 4, fromTo, 4);

    // denoise
    std::vector<cv::Mat> observations;
    observations.push_back(mBlue);
    cv::denoise_TVL1(observations, mBlueDenoised, lambda, mIterations);

    observations.clear();
    observations.push_back(mGreen);
    cv::denoise_TVL1(observations, mGreenDenoised, lambda, mIterations);

    observations.clear();
    observations.push_back(mRed);
    cv::denoise_TVL1(observations, mRedDenoised, lambda, mIterations);

    // merge image channels
    cv::Mat mOutMerge[] = { mBlueDenoised, mGreenDenoised, mRedDenoised, mAlpha };
    cv::mixChannels(mOutMerge, 4, &mDst, 1, fromTo, 4);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    double strength;
    int iterations;
    bool ok;
    strength = s.value("strength", 0.).toDouble(&ok);
    if (!ok || strength < 0 || strength > 100)
        return false;
    iterations = s.value("iterations", 30).toInt(&ok);
    if (!ok || iterations < 1 || iterations > 100)
        return false;
    setStrength(strength);
    setIterations(iterations);
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("strength", mStrength);
    s.setValue("iterations", mIterations);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setStrength(mStrength);
    fw->setIterations(mIterations);
    connect(this, SIGNAL(strengthChanged(double)), fw, SLOT(setStrength(double)));
    connect(this, SIGNAL(iterationsChanged(int)), fw, SLOT(setIterations(int)));
    connect(fw, SIGNAL(strengthChanged(double)), this, SLOT(setStrength(double)));
    connect(fw, SIGNAL(iterationsChanged(int)), this, SLOT(setIterations(int)));
    return fw;
}

void Filter::setStrength(double s)
{
    if (s == mStrength)
        return;
    mStrength = s;
    emit strengthChanged(s);
    emit parametersChanged();
}

void Filter::setIterations(int i)
{
    if (i == mIterations)
        return;
    mIterations = i;
    emit iterationsChanged(i);
    emit parametersChanged();
}
