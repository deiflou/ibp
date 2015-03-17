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

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/util.h"
#include "../imgproc/pixelblending.h"

Filter::Filter() :
    mColor(255, 0, 0),
    mPosition(Front),
    mColorCompositionMode(ColorCompositionMode_Normal),
    mOpacity(100)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mColor = mColor;
    f->mPosition = mPosition;
    f->mColorCompositionMode = mColorCompositionMode;
    f->mOpacity = mOpacity;
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

    QImage i(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);

    BGRA src;
    src.b = mColor.blue();
    src.g = mColor.green();
    src.r = mColor.red();
    src.a = qRound(mOpacity * 255 / 100.);
    register BGRA * dst = (BGRA *)inputImage.bits(), * blend = (BGRA *)i.bits();

    register int totalPixels = i.width() * i.height();

    if (mPosition == Front)
    {
        while (totalPixels--)
        {
            blendColors[mColorCompositionMode](src, *dst, *blend);
            dst++;
            blend++;
        }
    }
    else if (mPosition == Inside)
    {
        if (mColorCompositionMode == ColorCompositionMode_Normal)
        {
            while (totalPixels--)
            {
                alphaBlendColors[AlphaCompositionMode_SourceAtopDestination](src, *dst, *blend);
                dst++;
                blend++;
            }
        }
        else
        {
            while (totalPixels--)
            {
                blendColors[mColorCompositionMode](src, *dst, *blend);
                alphaBlendColors[AlphaCompositionMode_SourceAtopDestination](*blend, *dst, *blend);
                dst++;
                blend++;
            }
        }
    }
    else
    {
        while (totalPixels--)
        {
            alphaBlendColors[AlphaCompositionMode_DestinationOverSource](src, *dst, *blend);
            dst++;
            blend++;
        }
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString colorStr;
    QString positionStr;
    QString colorCompositionModeStr;
    QColor color(255, 0, 0);
    Position position;
    ColorCompositionMode colorCompositionMode;
    int opacity;
    bool ok;

    colorStr = s.value("color", "#FF0000").toString();
    color = QColor(colorStr);
    if (!color.isValid())
        return false;
    color.setAlpha(255);

    positionStr = s.value("position", "front").toString();
    if (positionStr == "front")
        position = Front;
    else if (positionStr == "behind")
        position = Behind;
    else if (positionStr == "inside")
        position = Inside;
    else
        return false;

    colorCompositionModeStr = s.value("colorcompositionmode", "normal").toString();
    colorCompositionMode = colorCompositionModeStringToEnum(colorCompositionModeStr);
    if (colorCompositionMode == ColorCompositionMode_Unsupported)
      return false;

    opacity = s.value("opacity", 100).toInt(&ok);
    if (!ok || opacity < 0 || opacity > 100)
        return false;

    setColor(color);
    setPosition(position);
    setColorCompositionMode(colorCompositionMode);
    setOpacity(opacity);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("color", mColor.name(QColor::HexRgb));
    s.setValue("position", mPosition == Front ? "front" : (mPosition == Behind ? "behind" : "inside"));
    s.setValue("colorcompositionmode", colorCompositionModeEnumToString(mColorCompositionMode));
    s.setValue("opacity", mOpacity);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setColor(mColor);
    fw->setPosition(mPosition);
    fw->setColorCompositionMode(mColorCompositionMode);
    fw->setOpacity(mOpacity);
    connect(this, SIGNAL(colorChanged(QColor)), fw, SLOT(setColor(QColor)));
    connect(this, SIGNAL(positionChanged(Filter::Position)), fw, SLOT(setPosition(Filter::Position)));
    connect(this, SIGNAL(colorCompositionModeChanged(ColorCompositionMode)),
            fw, SLOT(setColorCompositionMode(ColorCompositionMode)));
    connect(this, SIGNAL(opacityChanged(int)), fw, SLOT(setOpacity(int)));
    connect(fw, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
    connect(fw, SIGNAL(positionChanged(Filter::Position)), this, SLOT(setPosition(Filter::Position)));
    connect(fw, SIGNAL(colorCompositionModeChanged(ColorCompositionMode)),
            this, SLOT(setColorCompositionMode(ColorCompositionMode)));
    connect(fw, SIGNAL(opacityChanged(int)), this, SLOT(setOpacity(int)));
    return fw;
}

void Filter::setColor(const QColor & v)
{
    if (v == mColor)
        return;
    mColor = v;
    emit colorChanged(v);
    emit parametersChanged();
}

void Filter::setPosition(Filter::Position v)
{
    if (v == mPosition)
        return;
    mPosition = v;
    emit positionChanged(v);
    emit parametersChanged();
}

void Filter::setColorCompositionMode(ColorCompositionMode v)
{
    if (v == mColorCompositionMode)
        return;
    mColorCompositionMode = v;
    emit colorCompositionModeChanged(v);
    emit parametersChanged();
}

void Filter::setOpacity(int v)
{
    if (v == mOpacity)
        return;
    mOpacity = v;
    emit opacityChanged(v);
    emit parametersChanged();
}
