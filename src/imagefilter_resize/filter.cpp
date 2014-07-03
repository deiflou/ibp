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

#include <QPainter>

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"

Filter::Filter() :
    mWidth(100),
    mHeight(100),
    mWidthMode(Percent),
    mHeightMode(Percent),
    mResizeMode(Absolute),
    mAnchorPosition(anitools::misc::CenterCenter),
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

extern "C" QHash<QString, QString> getAnitoolsPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getAnitoolsPluginInfo();
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

    if (mAnchorPosition == anitools::misc::TopLeft ||
        mAnchorPosition == anitools::misc::CenterLeft ||
        mAnchorPosition == anitools::misc::BottomLeft)
        offsetX = 0;
    else if (mAnchorPosition == anitools::misc::TopCenter ||
             mAnchorPosition == anitools::misc::CenterCenter ||
             mAnchorPosition == anitools::misc::BottomCenter)
        offsetX = (width - inputImage.width()) >> 1;
    else
        offsetX = width - inputImage.width();

    if (mAnchorPosition == anitools::misc::TopLeft ||
        mAnchorPosition == anitools::misc::TopCenter ||
        mAnchorPosition == anitools::misc::TopRight)
        offsetY = 0;
    else if (mAnchorPosition == anitools::misc::CenterLeft ||
             mAnchorPosition == anitools::misc::CenterCenter ||
             mAnchorPosition == anitools::misc::CenterRight)
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
    anitools::misc::Alignment anchorPosition;
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
        anchorPosition = anitools::misc::TopLeft;
    else if (anchorPositionStr == "topcenter")
        anchorPosition = anitools::misc::TopCenter;
    else if (anchorPositionStr == "topright")
        anchorPosition = anitools::misc::TopRight;
    else if (anchorPositionStr == "centerleft")
        anchorPosition = anitools::misc::CenterLeft;
    else if (anchorPositionStr == "centercenter")
        anchorPosition = anitools::misc::CenterCenter;
    else if (anchorPositionStr == "centerright")
        anchorPosition = anitools::misc::CenterRight;
    else if (anchorPositionStr == "bottomleft")
        anchorPosition = anitools::misc::BottomLeft;
    else if (anchorPositionStr == "bottomcenter")
        anchorPosition = anitools::misc::BottomCenter;
    else if (anchorPositionStr == "bottomright")
        anchorPosition = anitools::misc::BottomRight;
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
    s.setValue("anchorPosition", mAnchorPosition == anitools::misc::TopLeft ? "topleft" :
                                 mAnchorPosition == anitools::misc::TopCenter ? "topcenter" :
                                 mAnchorPosition == anitools::misc::TopRight ? "topright" :
                                 mAnchorPosition == anitools::misc::CenterLeft ? "centerleft" :
                                 mAnchorPosition == anitools::misc::CenterCenter ? "centercenter" :
                                 mAnchorPosition == anitools::misc::CenterRight ? "centerright" :
                                 mAnchorPosition == anitools::misc::BottomLeft ? "bottomleft" :
                                 mAnchorPosition == anitools::misc::BottomCenter ? "bottomcenter" : "bottomright");
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
    connect(this, SIGNAL(anchorPositionChanged(anitools::misc::Alignment)),
            fw, SLOT(setAnchorPosition(anitools::misc::Alignment)));
    connect(this, SIGNAL(backgroundColorChanged(QColor)), fw, SLOT(setBackgroundColor(QColor)));

    connect(fw, SIGNAL(widthChanged(int)), this, SLOT(setWidth(int)));
    connect(fw, SIGNAL(heightChanged(int)), this, SLOT(setHeight(int)));
    connect(fw, SIGNAL(widthModeChanged(Filter::SizeMode)), this, SLOT(setWidthMode(Filter::SizeMode)));
    connect(fw, SIGNAL(heightModeChanged(Filter::SizeMode)), this, SLOT(setHeightMode(Filter::SizeMode)));
    connect(fw, SIGNAL(resizeModeChanged(Filter::ResizeMode)),
            this, SLOT(setResizeMode(Filter::ResizeMode)));
    connect(fw, SIGNAL(anchorPositionChanged(anitools::misc::Alignment)),
            this, SLOT(setAnchorPosition(anitools::misc::Alignment)));
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

void Filter::setAnchorPosition(anitools::misc::Alignment ap)
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
