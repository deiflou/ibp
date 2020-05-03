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

Filter::Filter() :
    mWidth(100),
    mHeight(100),
    mWidthMode(Percent),
    mHeightMode(Percent),
    mResamplingMode(Bicubic)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mWidth = mWidth;
    f->mHeight = mHeight;
    f->mWidthMode = mWidthMode;
    f->mHeightMode = mHeightMode;
    f->mResamplingMode = mResamplingMode;
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

    cv::Mat srcM(inputImage.height(), inputImage.width(), CV_8UC4,
                 (void *)inputImage.bits(), inputImage.bytesPerLine());
    cv::Mat dstM;
    int width = 0, height = 0;

    if (mWidthMode == Percent)
        width = inputImage.width() * mWidth / 100;
    else if (mWidthMode == Pixels)
        width = mWidth;

    if (mHeightMode == Percent)
        height = inputImage.height() * mHeight / 100;
    else if (mHeightMode == Pixels)
        height = mHeight;

    if (mWidthMode == KeepAspectRatio)
        width = inputImage.width() * height / inputImage.height();

    if (mHeightMode == KeepAspectRatio)
        height = inputImage.height() * width / inputImage.width();

    if (width < 1)
        width = 1;
    if (height < 1)
        height = 1;

    cv::resize(srcM, dstM, cv::Size(width, height), 0, 0, mResamplingMode == NearestNeighbor ? cv::INTER_NEAREST :
                                                          mResamplingMode == Bilinear ? cv::INTER_LINEAR :
                                                          mResamplingMode == Bicubic ? cv::INTER_CUBIC :
                                                                                       cv::INTER_LANCZOS4);

    QImage i(width, height, QImage::Format_ARGB32);

    for (int y = 0; y < height; y++)
        memcpy(i.scanLine(y), dstM.row(y).data, i.bytesPerLine());

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    int width, height;
    QString widthModeStr, heightModeStr, resamplingModeStr;
    SizeMode widthMode, heightMode;
    ResamplingMode resamplingMode;
    bool ok;

    width = s.value("width", 100).toUInt(&ok);
    if (!ok)
        return false;

    height = s.value("height", 100).toUInt(&ok);
    if (!ok)
        return false;

    widthModeStr = s.value("widthmode", "percent").toString();
    if (widthModeStr == "percent")
        widthMode = Percent;
    else if (widthModeStr == "pixels")
        widthMode = Pixels;
    else if (widthModeStr == "keepaspectratio")
        widthMode = KeepAspectRatio;
    else
        return false;

    heightModeStr = s.value("heightmode", "percent").toString();
    if (heightModeStr == "percent")
        heightMode = Percent;
    else if (heightModeStr == "pixels")
        heightMode = Pixels;
    else if (heightModeStr == "keepaspectratio")
        heightMode = KeepAspectRatio;
    else
        return false;

    if (widthMode == KeepAspectRatio && heightMode == KeepAspectRatio)
        return false;

    resamplingModeStr = s.value("resamplingmode", "bicubic").toString();
    if (resamplingModeStr == "nearestneighbor")
        resamplingMode = NearestNeighbor;
    else if (resamplingModeStr == "bilinear")
        resamplingMode = Bilinear;
    else if (resamplingModeStr == "bicubic")
        resamplingMode = Bicubic;
    else if (resamplingModeStr == "lanczos")
        resamplingMode = Lanczos;
    else
        return false;

    setWidthMode(widthMode);
    setHeightMode(heightMode);
    setWidth(width);
    setHeight(height);
    setResamplingMode(resamplingMode);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("width", mWidth);
    s.setValue("height", mHeight);
    s.setValue("widthmode", mWidthMode == Percent ? "percent" : mWidthMode == Pixels ? "pixels" : "keepaspectratio");
    s.setValue("heightmode", mHeightMode == Percent ? "percent" : mHeightMode == Pixels ? "pixels" : "keepaspectratio");
    s.setValue("resamplingmode", mResamplingMode == NearestNeighbor ? "nearestneighbor" :
                                 mResamplingMode == Bilinear ? "bilinear" :
                                 mResamplingMode == Bicubic ? "bicubic" : "lanczos");

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setWidthMode(mWidthMode);
    fw->setHeightMode(mHeightMode);
    fw->setWidth(mWidth);
    fw->setHeight(mHeight);
    fw->setResamplingMode(mResamplingMode);

    connect(this, SIGNAL(widthChanged(int)), fw, SLOT(setWidth(int)));
    connect(this, SIGNAL(heightChanged(int)), fw, SLOT(setHeight(int)));
    connect(this, SIGNAL(widthModeChanged(Filter::SizeMode)), fw, SLOT(setWidthMode(Filter::SizeMode)));
    connect(this, SIGNAL(heightModeChanged(Filter::SizeMode)), fw, SLOT(setHeightMode(Filter::SizeMode)));
    connect(this, SIGNAL(resamplingModeChanged(Filter::ResamplingMode)),
            fw, SLOT(setResamplingMode(Filter::ResamplingMode)));

    connect(fw, SIGNAL(widthChanged(int)), this, SLOT(setWidth(int)));
    connect(fw, SIGNAL(heightChanged(int)), this, SLOT(setHeight(int)));
    connect(fw, SIGNAL(widthModeChanged(Filter::SizeMode)), this, SLOT(setWidthMode(Filter::SizeMode)));
    connect(fw, SIGNAL(heightModeChanged(Filter::SizeMode)), this, SLOT(setHeightMode(Filter::SizeMode)));
    connect(fw, SIGNAL(resamplingModeChanged(Filter::ResamplingMode)),
            this, SLOT(setResamplingMode(Filter::ResamplingMode)));

    return fw;
}

void Filter::setWidth(int w)
{
    if (w == mWidth)
        return;
    mWidth = w;
    emit widthChanged(w);
    emit parametersChanged();
}

void Filter::setHeight(int h)
{
    if (h == mHeight)
        return;
    mHeight = h;
    emit heightChanged(h);
    emit parametersChanged();
}

void Filter::setWidthMode(Filter::SizeMode wm)
{
    if (wm == mWidthMode)
        return;
    mWidthMode = wm;
    emit widthModeChanged(wm);
    emit parametersChanged();
}

void Filter::setHeightMode(Filter::SizeMode hm)
{
    if (hm == mHeightMode)
        return;
    mHeightMode = hm;
    emit heightModeChanged(hm);
    emit parametersChanged();
}

void Filter::setResamplingMode(Filter::ResamplingMode rm)
{
    if (rm == mResamplingMode)
        return;
    mResamplingMode = rm;
    emit resamplingModeChanged(rm);
    emit parametersChanged();
}
