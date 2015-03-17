/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
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

#include <opencv2/photo.hpp>

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

extern "C" QHash<QString, QString> getAnitoolsPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getAnitoolsPluginInfo();
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
    cv::Mat mRGBDenoised;
    double sigma, h, hColor;
    int templateWindowSize, searchWindowSize;
    int fromTo[] = { 0, 0, 1, 1, 2, 2, 3, 3 };

    // calculate parameters
    sigma = mStrength;
    h = sigma * (sigma <= 30. ? .4 : sigma <= 75. ? .35 : .3);
    hColor = sigma * (sigma <= 25. ? .55 : sigma <= 55. ? .4 : .35);
    templateWindowSize = sigma <= 15. ? 3 : sigma <= 30. ? 5 : sigma <= 45. ? 7 : sigma <= 75. ? 9 : 11;
    searchWindowSize = sigma <= 37.5 ? 21 : 35;

    // split the image channels
    cv::Mat mOutSplit[] = { mRGB, mAlpha };
    cv::mixChannels(&mSrc, 1, mOutSplit, 2, fromTo, 4);

    // denoise
    //cv::fastNlMeansDenoisingColored(mRGB, mRGBDenoised, h, hColor, templateWindowSize, searchWindowSize);
    cv::fastNlMeansDenoising(mRGB, mRGBDenoised, h, templateWindowSize, searchWindowSize);

    // merge image channels
    cv::Mat mOutMerge[] = { mRGBDenoised, mAlpha };
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
