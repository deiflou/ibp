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

#include <opencv2/photo.hpp>

#include "filter.h"

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

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    cv::Mat mSrc(inputImage.height(), inputImage.width(), CV_8UC4, (void *)inputImage.bits());
    cv::Mat mDst(i.height(), i.width(), CV_8UC4, i.bits());
    cv::Mat mRGB(inputImage.height(), inputImage.width(), CV_8UC3);
    cv::Mat mAlpha(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mGray, mTmp;
    int fromTo[] = { 0, 0, 1, 1, 2, 2, 3, 3 };

    // split the image channels
    cv::Mat mOutSplit[] = { mRGB, mAlpha };
    cv::mixChannels(&mSrc, 1, mOutSplit, 2, fromTo, 4);

    // denoise
    cv::decolor(mRGB, mGray, mTmp);

    // merge image channels
    cv::Mat mOutMerge[] = { mGray, mGray, mGray, mAlpha };
    cv::mixChannels(mOutMerge, 4, &mDst, 1, fromTo, 4);

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
