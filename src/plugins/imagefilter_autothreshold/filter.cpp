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
#include <vector>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/types.h>
#include <imgproc/thresholding.h>
#include <imgproc/util.h>

Filter::Filter() :
    mThresholdMode(0),
    mColorMode(0)
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
    f->mColorMode = mColorMode;
    for (int i = 0; i < 5; i++)
        f->mAffectedChannel[i] = mAffectedChannel[i];
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

    QImage i = inputImage.copy();
    cv::Mat dstMat(i.height(), i.width(), CV_8UC4, i.bits(), i.bytesPerLine());
    const int radius = 10;
    const int windowSize = radius * 2 + 1;
    const double k = .05;

    if (mColorMode == 0)
    {
        if (!mAffectedChannel[0] && !mAffectedChannel[4])
            return i;

        if (mAffectedChannel[0])
        {
            register BGRA * bitsSrc = (BGRA *)inputImage.bits();
            register BGRA * bitsDst = (BGRA *)i.bits();
            register int totalPixels = i.width() * i.height();
            while (totalPixels--)
            {
                bitsDst->b = IBP_pixelIntensity4(bitsSrc->r, bitsSrc->g, bitsSrc->b);
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
    QString thresholdModeStr, colorModeStr, affectedChannelStr;
    int thresholdMode, colorMode;
    QStringList affectedChannelList;
    bool affectedChannel[5] =  { false };

    thresholdModeStr = s.value("thresholdmode", "global").toString();
    if (thresholdModeStr == "global")
        thresholdMode = 0;
    else if (thresholdModeStr == "local")
        thresholdMode = 1;
    else
        return false;

    colorModeStr = s.value("colormode", "luma").toString();
    if (colorModeStr == "luma")
        colorMode = 0;
    else if (colorModeStr == "rgb")
        colorMode = 1;
    else
        return false;

    affectedChannelStr = s.value("affectedchannels", "").toString();
    affectedChannelList = affectedChannelStr.split(" ", QString::SkipEmptyParts);
    for (int i = 0; i < affectedChannelList.size(); i++)
    {
        affectedChannelStr = affectedChannelList.at(i);
        if (affectedChannelList.at(i) == "luma")
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
    setColorMode(colorMode);
    for (int i = 0; i < 5; i++)
        setAffectedChannel(i, affectedChannel[i]);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("thresholdmode", mThresholdMode == 0 ? "global" : "local");

    s.setValue("colormode", mColorMode == 0 ? "luma" : "rgb");

    QStringList affectedChannelList;
    if (mAffectedChannel[0])
        affectedChannelList.append("luma");
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
    fw->setColorMode(mColorMode);
    for (int i = 0; i < 5; i++)
        fw->setAffectedChannel(i, mAffectedChannel[i]);
    connect(this, SIGNAL(thresholdModeChanged(int)), fw, SLOT(setThresholdMode(int)));
    connect(this, SIGNAL(colorModeChanged(int)), fw, SLOT(setColorMode(int)));
    connect(this, SIGNAL(affectedChannelChanged(int,bool)), fw, SLOT(setAffectedChannel(int,bool)));
    connect(fw, SIGNAL(thresholdModeChanged(int)), this, SLOT(setThresholdMode(int)));
    connect(fw, SIGNAL(colorModeChanged(int)), this, SLOT(setColorMode(int)));
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

void Filter::setColorMode(int m)
{
    if (m == mColorMode)
        return;
    mColorMode = m;
    emit colorModeChanged(m);
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

