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

#include <opencv2/xphoto.hpp>

#include "filter.h"
#include "filterwidget.h"

Filter::Filter() :
    mStrength(0.)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mStrength = mStrength;
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
    cv::Mat mRGB(inputImage.height(), inputImage.width(), CV_8UC3);
    cv::Mat mAlpha(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mRGBDenoised(inputImage.height(), inputImage.width(), CV_8UC3);
    double sigma = mStrength;
    cv::Mat mOutSplit[] = { mRGB, mAlpha };
    cv::Mat mOutMerge[] = { mRGBDenoised, mAlpha };
    int fromTo[] = { 0, 0, 1, 1, 2, 2, 3, 3 };

    // split the image channels
    cv::mixChannels(&mSrc, 1, mOutSplit, 2, fromTo, 4);

    // denoise
    cv::xphoto::dctDenoising(mRGB, mRGBDenoised, sigma);

    // merge image channels
    cv::mixChannels(mOutMerge, 2, &mDst, 1, fromTo, 4);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    double strength;
    bool ok;
    strength = s.value("strength", 0.).toDouble(&ok);
    if (!ok || strength < 0. || strength > 100.)
        return false;
    setStrength(strength);
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("strength", mStrength);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setStrength(mStrength);
    connect(this, SIGNAL(strengthChanged(double)), fw, SLOT(setStrength(double)));
    connect(fw, SIGNAL(strengthChanged(double)), this, SLOT(setStrength(double)));
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
