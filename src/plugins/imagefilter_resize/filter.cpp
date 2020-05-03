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

#include <QPainter>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/types.h>

Filter::Filter() :
    mWidth(100),
    mHeight(100),
    mWidthMode(Percent),
    mHeightMode(Percent),
    mResizeMode(Absolute),
    mAnchorPosition(ibp::misc::CenterCenter),
    mBackgroundColor(0, 0, 0, 0)
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
    f->mResizeMode = mResizeMode;
    f->mAnchorPosition = mAnchorPosition;
    f->mBackgroundColor = mBackgroundColor;
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

    int width = 0, height = 0;

    if (mResizeMode == Absolute)
    {
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
    }
    else
    {
        if (mWidthMode == Percent)
            width = inputImage.width() + inputImage.width() * mWidth / 100;
        else if (mWidthMode == Pixels)
            width = inputImage.width() + mWidth;

        if (mHeightMode == Percent)
            height = inputImage.height() + inputImage.height() * mHeight / 100;
        else if (mHeightMode == Pixels)
            height = inputImage.height() + mHeight;

        if (mWidthMode == KeepAspectRatio)
            width = inputImage.width() * height / inputImage.height();

        if (mHeightMode == KeepAspectRatio)
            height = inputImage.height() * width / inputImage.width();
    }
    if (width < 1)
        width = 1;
    if (height < 1)
        height = 1;


    QImage i(width, height, QImage::Format_ARGB32);
    int offsetX, offsetY;

    if (mAnchorPosition == ibp::misc::TopLeft ||
        mAnchorPosition == ibp::misc::CenterLeft ||
        mAnchorPosition == ibp::misc::BottomLeft)
        offsetX = 0;
    else if (mAnchorPosition == ibp::misc::TopCenter ||
             mAnchorPosition == ibp::misc::CenterCenter ||
             mAnchorPosition == ibp::misc::BottomCenter)
        offsetX = (width - inputImage.width()) >> 1;
    else
        offsetX = width - inputImage.width();

    if (mAnchorPosition == ibp::misc::TopLeft ||
        mAnchorPosition == ibp::misc::TopCenter ||
        mAnchorPosition == ibp::misc::TopRight)
        offsetY = 0;
    else if (mAnchorPosition == ibp::misc::CenterLeft ||
             mAnchorPosition == ibp::misc::CenterCenter ||
             mAnchorPosition == ibp::misc::CenterRight)
        offsetY = (height - inputImage.height()) >> 1;
    else
        offsetY = height - inputImage.height();

    i.fill(mBackgroundColor);
    QPainter p(&i);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawImage(offsetX, offsetY, inputImage);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    int width, height;
    QString widthModeStr, heightModeStr, resizeModeStr, anchorPositionStr, backgroundColorStr;
    SizeMode widthMode, heightMode;
    ResizeMode resizeMode;
    ibp::misc::Alignment anchorPosition;
    QColor backgroundColor;
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

    resizeModeStr = s.value("resizemode", "absolute").toString();
    if (resizeModeStr == "absolute")
        resizeMode = Absolute;
    else if (resizeModeStr == "relative")
        resizeMode = Relative;
    else
        return false;

    anchorPositionStr = s.value("anchorposition", "centercenter").toString();
    if (anchorPositionStr == "topleft")
        anchorPosition = ibp::misc::TopLeft;
    else if (anchorPositionStr == "topcenter")
        anchorPosition = ibp::misc::TopCenter;
    else if (anchorPositionStr == "topright")
        anchorPosition = ibp::misc::TopRight;
    else if (anchorPositionStr == "centerleft")
        anchorPosition = ibp::misc::CenterLeft;
    else if (anchorPositionStr == "centercenter")
        anchorPosition = ibp::misc::CenterCenter;
    else if (anchorPositionStr == "centerright")
        anchorPosition = ibp::misc::CenterRight;
    else if (anchorPositionStr == "bottomleft")
        anchorPosition = ibp::misc::BottomLeft;
    else if (anchorPositionStr == "bottomcenter")
        anchorPosition = ibp::misc::BottomCenter;
    else if (anchorPositionStr == "bottomright")
        anchorPosition = ibp::misc::BottomRight;
    else
        return false;

    backgroundColorStr = s.value("backgroundcolor", "#00000000").toString();
    backgroundColor = QColor(backgroundColorStr);
    if (!backgroundColor.isValid())
        return false;

    setWidth(width);
    setHeight(height);
    setWidthMode(widthMode);
    setHeightMode(heightMode);
    setResizeMode(resizeMode);
    setAnchorPosition(anchorPosition);
    setBackgroundColor(backgroundColor);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("width", mWidth);
    s.setValue("height", mHeight);
    s.setValue("widthmode", mWidthMode == Percent ? "percent" : mWidthMode == Pixels ? "pixels" : "keepaspectratio");
    s.setValue("heightmode", mHeightMode == Percent ? "percent" : mHeightMode == Pixels ? "pixels" : "keepaspectratio");
    s.setValue("resizemode", mResizeMode == Absolute ? "absolute" : "relative");
    s.setValue("anchorPosition", mAnchorPosition == ibp::misc::TopLeft ? "topleft" :
                                 mAnchorPosition == ibp::misc::TopCenter ? "topcenter" :
                                 mAnchorPosition == ibp::misc::TopRight ? "topright" :
                                 mAnchorPosition == ibp::misc::CenterLeft ? "centerleft" :
                                 mAnchorPosition == ibp::misc::CenterCenter ? "centercenter" :
                                 mAnchorPosition == ibp::misc::CenterRight ? "centerright" :
                                 mAnchorPosition == ibp::misc::BottomLeft ? "bottomleft" :
                                 mAnchorPosition == ibp::misc::BottomCenter ? "bottomcenter" : "bottomright");
    s.setValue("backgroundcolor", mBackgroundColor.name(QColor::HexArgb));

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setWidth(mWidth);
    fw->setHeight(mHeight);
    fw->setWidthMode(mWidthMode);
    fw->setHeightMode(mHeightMode);
    fw->setResizeMode(mResizeMode);
    fw->setAnchorPosition(mAnchorPosition);
    fw->setBackgroundColor(mBackgroundColor);

    connect(this, SIGNAL(widthChanged(int)), fw, SLOT(setWidth(int)));
    connect(this, SIGNAL(heightChanged(int)), fw, SLOT(setHeight(int)));
    connect(this, SIGNAL(widthModeChanged(Filter::SizeMode)), fw, SLOT(setWidthMode(Filter::SizeMode)));
    connect(this, SIGNAL(heightModeChanged(Filter::SizeMode)), fw, SLOT(setHeightMode(Filter::SizeMode)));
    connect(this, SIGNAL(resizeModeChanged(Filter::ResizeMode)),
            fw, SLOT(setResizeMode(Filter::ResizeMode)));
    connect(this, SIGNAL(anchorPositionChanged(ibp::misc::Alignment)),
            fw, SLOT(setAnchorPosition(ibp::misc::Alignment)));
    connect(this, SIGNAL(backgroundColorChanged(QColor)), fw, SLOT(setBackgroundColor(QColor)));

    connect(fw, SIGNAL(widthChanged(int)), this, SLOT(setWidth(int)));
    connect(fw, SIGNAL(heightChanged(int)), this, SLOT(setHeight(int)));
    connect(fw, SIGNAL(widthModeChanged(Filter::SizeMode)), this, SLOT(setWidthMode(Filter::SizeMode)));
    connect(fw, SIGNAL(heightModeChanged(Filter::SizeMode)), this, SLOT(setHeightMode(Filter::SizeMode)));
    connect(fw, SIGNAL(resizeModeChanged(Filter::ResizeMode)),
            this, SLOT(setResizeMode(Filter::ResizeMode)));
    connect(fw, SIGNAL(anchorPositionChanged(ibp::misc::Alignment)),
            this, SLOT(setAnchorPosition(ibp::misc::Alignment)));
    connect(fw, SIGNAL(backgroundColorChanged(QColor)), this, SLOT(setBackgroundColor(QColor)));

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

void Filter::setResizeMode(Filter::ResizeMode rm)
{
    if (rm == mResizeMode)
        return;
    mResizeMode = rm;
    emit resizeModeChanged(rm);
    emit parametersChanged();
}

void Filter::setAnchorPosition(ibp::misc::Alignment ap)
{
    if (ap == mAnchorPosition)
        return;
    mAnchorPosition = ap;
    emit anchorPositionChanged(ap);
    emit parametersChanged();
}

void Filter::setBackgroundColor(const QColor &c)
{
    if (c == mBackgroundColor)
        return;
    mBackgroundColor = c;
    emit backgroundColorChanged(c);
    emit parametersChanged();
}
