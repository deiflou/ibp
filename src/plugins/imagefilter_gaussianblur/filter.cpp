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

Filter::Filter() :
    mRadius(0.0),
    mBlurRGB(true),
    mBlurAlpha(true)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mRadius = mRadius;
    f->mBlurRGB = mBlurRGB;
    f->mBlurAlpha = mBlurAlpha;
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

    if (!mBlurRGB && !mBlurAlpha)
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);

    cv::Mat mSrc(inputImage.height(), inputImage.width(), CV_8UC4, (void *)inputImage.bits());
    cv::Mat mDst(i.height(), i.width(), CV_8UC4, i.bits());

    double sigma = (mRadius + .5) / 2.45;

    // split the image channels
    cv::Mat mRGB(inputImage.height(), inputImage.width(), CV_8UC3);
    cv::Mat mAlpha(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mOutSplit[] = { mRGB, mAlpha };
    int fromTo[] = { 0, 0, 1, 1, 2, 2, 3, 3 };
    cv::mixChannels(&mSrc, 1, mOutSplit, 2, fromTo, 4);

    // apply gaussian blur
    cv::Mat mRGBBlurred;
    cv::Mat mAlphaBlurred;
    if (mBlurRGB)
    {
        mRGBBlurred = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC3);
        cv::GaussianBlur(mRGB, mRGBBlurred, cv::Size(0, 0), sigma);
    }
    else
        mRGBBlurred = mRGB;
    if (mBlurAlpha)
    {
        mAlphaBlurred = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC1);
        cv::GaussianBlur(mAlpha, mAlphaBlurred, cv::Size(0, 0), sigma);
    }
    else
        mAlphaBlurred = mAlpha;

    // merge image channels
    cv::Mat mOutMerge[] = { mRGBBlurred, mAlphaBlurred };
    cv::mixChannels(mOutMerge, 2, &mDst, 1, fromTo, 4);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    double radius;
    bool blurRGB, blurAlpha;
    bool ok;
    radius = s.value("radius", 0.0).toDouble(&ok);
    if (!ok)
        return false;
    blurRGB = s.value("blurrgb", true).toBool();
    blurAlpha = s.value("bluralpha", true).toBool();
    setRadius(radius);
    setBlurRGB(blurRGB);
    setBlurAlpha(blurAlpha);
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("radius", mRadius);
    s.setValue("blurrgb", mBlurRGB);
    s.setValue("bluralpha", mBlurAlpha);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setRadius(mRadius);
    fw->setBlurRGB(mBlurRGB);
    fw->setBlurAlpha(mBlurAlpha);

    connect(this, SIGNAL(radiusChanged(double)), fw, SLOT(setRadius(double)));
    connect(this, SIGNAL(blurRGBChanged(bool)), fw, SLOT(setBlurRGB(bool)));
    connect(this, SIGNAL(blurAlphaChanged(bool)), fw, SLOT(setBlurAlpha(bool)));

    connect(fw, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
    connect(fw, SIGNAL(blurRGBChanged(bool)), this, SLOT(setBlurRGB(bool)));
    connect(fw, SIGNAL(blurAlphaChanged(bool)), this, SLOT(setBlurAlpha(bool)));

    return fw;
}

void Filter::setRadius(double s)
{
    if (s == mRadius)
        return;
    mRadius = s;
    emit radiusChanged(s);
    emit parametersChanged();
}

void Filter::setBlurRGB(bool v)
{
    if (v == mBlurRGB)
        return;
    mBlurRGB = v;
    emit blurRGBChanged(v);
    emit parametersChanged();
}

void Filter::setBlurAlpha(bool v)
{
    if (v == mBlurAlpha)
        return;
    mBlurAlpha = v;
    emit blurAlphaChanged(v);
    emit parametersChanged();
}
