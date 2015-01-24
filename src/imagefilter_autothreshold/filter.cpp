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
#include <vector>

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"
#include "../imgproc/thresholding.h"

Filter::Filter() :
    mThresholdMode(0),
    mRGBMode(0)
{
    for (int i = 0; i < 5; i++)
        mAffectedChannel[i] = false;
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mThresholdMode = mThresholdMode;
    f->mRGBMode = mRGBMode;
    for (int i = 0; i < 5; i++)
        f->mAffectedChannel[i] = mAffectedChannel[i];
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

    QImage i = inputImage.copy();
    cv::Mat dstMat(i.height(), i.width(), CV_8UC4, i.bits(), i.bytesPerLine());
    const int radius = 10;
    const int windowSize = radius * 2 + 1;
    const double k = .05;

    if (mRGBMode == 0)
    {
        if (!mAffectedChannel[0] && !mAffectedChannel[4])
            return i;

        if (mAffectedChannel[0])
        {
            const unsigned short gR = .2126 * 0x10000;
            const unsigned short gG = .7152 * 0x10000;
            const unsigned short gB = .0722 * 0x10000;
            register BGRA * bitsSrc = (BGRA *)inputImage.bits();
            register BGRA * bitsDst = (BGRA *)i.bits();
            register int totalPixels = i.width() * i.height();
            while (totalPixels--)
            {
                bitsDst->b = (bitsSrc->r * gR >> 16) + (bitsSrc->g * gG >> 16) + (bitsSrc->b * gB >> 16);
                bitsSrc++;
                bitsDst++;
            }
        }

        cv::Mat mSrcGray, mSrcAlpha;
        if (mAffectedChannel[0])
        {
            mSrcGray = cv::Mat(dstMat.rows, dstMat.cols, CV_8UC1);
            int from_to[] = { 0,0 };
            cv::mixChannels(&dstMat, 1, &mSrcGray, 1, from_to, 1);
        }
        if (mAffectedChannel[4])
        {
            mSrcAlpha = cv::Mat(dstMat.rows, dstMat.cols, CV_8UC1);
            int from_to[] = { 3,0 };
            cv::mixChannels(&dstMat, 1, &mSrcAlpha, 1, from_to, 1);
        }

        if (mThresholdMode == 0)
        {
            if (mAffectedChannel[0])
                cv::threshold(mSrcGray, mSrcGray, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            if (mAffectedChannel[4])
                cv::threshold(mSrcAlpha, mSrcAlpha, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        }
        else
        {
            if (mAffectedChannel[0])
                adaptiveThresholdIntegral(mSrcGray, mSrcGray, windowSize, k);
            if (mAffectedChannel[4])
                adaptiveThresholdIntegral(mSrcAlpha, mSrcAlpha, windowSize, k);
        }

        if (mAffectedChannel[0])
        {
            int from_to[] = { 0,0, 0,1, 0,2 };
            cv::mixChannels(&mSrcGray, 1, &dstMat, 1, from_to, 3);
        }
        if (mAffectedChannel[4])
        {
            int from_to[] = { 0,3 };
            cv::mixChannels(&mSrcAlpha, 1, &dstMat, 1, from_to, 1);
        }
    }
    else
    {
        if (!mAffectedChannel[1] && !mAffectedChannel[2] &&
            !mAffectedChannel[3] && !mAffectedChannel[4])
            return i;

        cv::Mat mSrcRed, mSrcGreen, mSrcBlue, mSrcAlpha;
        if (mAffectedChannel[1])
        {
            mSrcBlue = cv::Mat(dstMat.rows, dstMat.cols, CV_8UC1);
            int from_to[] = { 0,0 };
            cv::mixChannels(&dstMat, 1, &mSrcBlue, 1, from_to, 1);
        }
        if (mAffectedChannel[2])
        {
            mSrcGreen = cv::Mat(dstMat.rows, dstMat.cols, CV_8UC1);
            int from_to[] = { 1,0 };
            cv::mixChannels(&dstMat, 1, &mSrcGreen, 1, from_to, 1);
        }
        if (mAffectedChannel[3])
        {
            mSrcRed = cv::Mat(dstMat.rows, dstMat.cols, CV_8UC1);
            int from_to[] = { 2,0 };
            cv::mixChannels(&dstMat, 1, &mSrcRed, 1, from_to, 1);
        }
        if (mAffectedChannel[4])
        {
            mSrcAlpha = cv::Mat(dstMat.rows, dstMat.cols, CV_8UC1);
            int from_to[] = { 3,0 };
            cv::mixChannels(&dstMat, 1, &mSrcAlpha, 1, from_to, 1);
        }

        if (mThresholdMode == 0)
        {
            if (mAffectedChannel[1])
                cv::threshold(mSrcBlue, mSrcBlue, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            if (mAffectedChannel[2])
                cv::threshold(mSrcGreen, mSrcGreen, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            if (mAffectedChannel[3])
                cv::threshold(mSrcRed, mSrcRed, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            if (mAffectedChannel[4])
                cv::threshold(mSrcAlpha, mSrcAlpha, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        }
        else
        {
            if (mAffectedChannel[1])
                adaptiveThresholdIntegral(mSrcBlue, mSrcBlue, windowSize, k);
            if (mAffectedChannel[2])
                adaptiveThresholdIntegral(mSrcGreen, mSrcGreen, windowSize, k);
            if (mAffectedChannel[3])
                adaptiveThresholdIntegral(mSrcRed, mSrcRed, windowSize, k);
            if (mAffectedChannel[4])
                adaptiveThresholdIntegral(mSrcAlpha, mSrcAlpha, windowSize, k);
        }

        if (mAffectedChannel[1])
        {
            int from_to[] = { 0,0 };
            cv::mixChannels(&mSrcBlue, 1, &dstMat, 1, from_to, 1);
        }
        if (mAffectedChannel[2])
        {
            int from_to[] = { 0,1 };
            cv::mixChannels(&mSrcGreen, 1, &dstMat, 1, from_to, 1);
        }
        if (mAffectedChannel[3])
        {
            int from_to[] = { 0,2 };
            cv::mixChannels(&mSrcRed, 1, &dstMat, 1, from_to, 1);
        }
        if (mAffectedChannel[4])
        {
            int from_to[] = { 0,3 };
            cv::mixChannels(&mSrcAlpha, 1, &dstMat, 1, from_to, 1);
        }
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString thresholdModeStr, rgbModeStr, affectedChannelStr;
    int thresholdMode, rgbMode;
    QStringList affectedChannelList;
    bool affectedChannel[5] =  { false };

    thresholdModeStr = s.value("thresholdmode", "global").toString();
    if (thresholdModeStr == "global")
        thresholdMode = 0;
    else if (thresholdModeStr == "local")
        thresholdMode = 1;
    else
        return false;

    rgbModeStr = s.value("rgbmode", "tight").toString();
    if (rgbModeStr == "tight")
        rgbMode = 0;
    else if (rgbModeStr == "independent")
        rgbMode = 1;
    else
        return false;

    affectedChannelStr = s.value("affectedchannels", "").toString();
    affectedChannelList = affectedChannelStr.split(" ", QString::SkipEmptyParts);
    for (int i = 0; i < affectedChannelList.size(); i++)
    {
        affectedChannelStr = affectedChannelList.at(i);
        if (affectedChannelList.at(i) == "rgb")
            affectedChannel[0] = true;
        else if (affectedChannelList.at(i) == "red")
            affectedChannel[1] = true;
        else if (affectedChannelList.at(i) == "green")
            affectedChannel[2] = true;
        else if (affectedChannelList.at(i) == "blue")
            affectedChannel[3] = true;
        else if (affectedChannelList.at(i) == "alpha")
            affectedChannel[4] = true;
        else
            return false;
    }

    setThresholdMode(thresholdMode);
    setRGBMode(rgbMode);
    for (int i = 0; i < 5; i++)
        setAffectedChannel(i, affectedChannel[i]);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("thresholdmode", mThresholdMode == 0 ? "global" : "local");

    s.setValue("rgbmode", mRGBMode == 0 ? "tight" : "independent");

    QStringList affectedChannelList;
    if (mAffectedChannel[0])
        affectedChannelList.append("rgb");
    if (mAffectedChannel[1])
        affectedChannelList.append("red");
    if (mAffectedChannel[2])
        affectedChannelList.append("green");
    if (mAffectedChannel[3])
        affectedChannelList.append("blue");
    if (mAffectedChannel[4])
        affectedChannelList.append("alpha");
    s.setValue("affectedchannels", affectedChannelList.join(" "));

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setThresholdMode(mThresholdMode);
    fw->setRGBMode(mRGBMode);
    for (int i = 0; i < 5; i++)
        fw->setAffectedChannel(i, mAffectedChannel[i]);
    connect(this, SIGNAL(thresholdModeChanged(int)), fw, SLOT(setThresholdMode(int)));
    connect(this, SIGNAL(rgbModeChanged(int)), fw, SLOT(setRGBMode(int)));
    connect(this, SIGNAL(affectedChannelChanged(int,bool)), fw, SLOT(setAffectedChannel(int,bool)));
    connect(fw, SIGNAL(thresholdModeChanged(int)), this, SLOT(setThresholdMode(int)));
    connect(fw, SIGNAL(rgbModeChanged(int)), this, SLOT(setRGBMode(int)));
    connect(fw, SIGNAL(affectedChannelChanged(int,bool)), this, SLOT(setAffectedChannel(int,bool)));
    return fw;
}

void Filter::setThresholdMode(int m)
{
    if (m == mThresholdMode)
        return;
    mThresholdMode = m;
    emit thresholdModeChanged(m);
    emit parametersChanged();
}

void Filter::setRGBMode(int m)
{
    if (m == mRGBMode)
        return;
    mRGBMode = m;
    emit rgbModeChanged(m);
    emit parametersChanged();
}

void Filter::setAffectedChannel(int c, bool a)
{
    if (a == mAffectedChannel[c])
        return;
    mAffectedChannel[c] = a;
    emit affectedChannelChanged(c, a);
    emit parametersChanged();
}

