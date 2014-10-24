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
