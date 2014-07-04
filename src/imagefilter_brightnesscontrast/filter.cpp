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

#include <QRegularExpression>
#include <QDebug>
#include <math.h>

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"

Filter::Filter() :
    mWorkingChannel(RGB)
{
    for (int i = 0; i < 5; i++)
    {
        mBrightnessContrast[i][Brightness] = 0;
        mBrightnessContrast[i][Contrast] = 0;
        makeLUT((WorkingChannel)i);
    }
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mWorkingChannel = mWorkingChannel;
    for (int i = 0; i < 5; i++)
    {
        f->mBrightnessContrast[i][Brightness] = mBrightnessContrast[i][Brightness];
        f->mBrightnessContrast[i][Contrast] = mBrightnessContrast[i][Contrast];
        f->makeLUT((WorkingChannel)i);
    }
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

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);

    register BGRA * bits = (BGRA*)inputImage.bits();
    register BGRA * bits2 = (BGRA*)i.bits();
    register int totalPixels = i.width() * i.height();

    while (totalPixels--)
    {
        bits2->r = mLuts[RGB][mLuts[Red][bits->r]];
        bits2->g = mLuts[RGB][mLuts[Green][bits->g]];
        bits2->b = mLuts[RGB][mLuts[Blue][bits->b]];
        bits2->a = mLuts[Alpha][bits->a];
        bits++;
        bits2++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString workingChannelStr;
    WorkingChannel workingChannel;
    QString brightnessContrastStr[5];
    QStringList brightnessContrastList;
    int brightnessContrast[5][2];
    bool ok;

    workingChannelStr = s.value("workingchannel", 0).toString();
    if (workingChannelStr == "rgb")
        workingChannel = RGB;
    else if (workingChannelStr == "red")
        workingChannel = Red;
    else if (workingChannelStr == "green")
        workingChannel = Green;
    else if (workingChannelStr == "blue")
        workingChannel = Blue;
    else if (workingChannelStr == "alpha")
        workingChannel = Alpha;
    else
        return false;

    brightnessContrastStr[RGB] = s.value("rgbbrightnesscontrast", 0).toString();
    brightnessContrastStr[Red] = s.value("redbrightnesscontrast", 0).toString();
    brightnessContrastStr[Green] = s.value("greenbrightnesscontrast", 0).toString();
    brightnessContrastStr[Blue] = s.value("bluebrightnesscontrast", 0).toString();
    brightnessContrastStr[Alpha] = s.value("alphabrightnesscontrast", 0).toString();
    for (int i = 0; i < 5; i++)
    {
        brightnessContrastList = brightnessContrastStr[i].split(QRegularExpression("\\s+"), QString::SkipEmptyParts);

        brightnessContrast[i][Brightness] = brightnessContrastList.at(0).toInt(&ok);
        if (!ok)
            return false;
        brightnessContrast[i][Contrast] = brightnessContrastList.at(1).toInt(&ok);
        if (!ok)
            return false;

        if (brightnessContrast[i][Brightness] < -100 || brightnessContrast[i][Brightness] > 100 ||
            brightnessContrast[i][Contrast] < -100 || brightnessContrast[i][Contrast] > 100)
            return false;
    }

    mWorkingChannel = workingChannel;
    for(int i = 0; i < 5; i++)
    {
        mBrightnessContrast[i][Brightness] = brightnessContrast[i][Brightness];
        mBrightnessContrast[i][Contrast] = brightnessContrast[i][Contrast];
        makeLUT((WorkingChannel)i);
    }
    emit workingChannelChanged(mWorkingChannel);
    emit brightnessChanged(mBrightnessContrast[mWorkingChannel][Brightness]);
    emit contrastChanged(mBrightnessContrast[mWorkingChannel][Contrast]);
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("workingchannel", mWorkingChannel == RGB ? "rgb" :
                                 mWorkingChannel == Red ? "red" :
                                 mWorkingChannel == Green ? "green" :
                                 mWorkingChannel == Blue ? "blue" :
                                 "alpha");
    QString brightnessContrastNameStr[5] = { "rgbbrightnesscontrast", "redbrightnesscontrast",
                                             "greenbrightnesscontrast", "bluebrightnesscontrast",
                                             "alphabrightnesscontrast" };
    QString brightnessContrastStr;
    for(int i = 0; i < 5; i++)
    {
        brightnessContrastStr = QString::number(mBrightnessContrast[i][Brightness]) + " " +
                                QString::number(mBrightnessContrast[i][Contrast]);
        s.setValue(brightnessContrastNameStr[i], brightnessContrastStr);
    }

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setWorkingChannel(mWorkingChannel);
    fw->setBrightness(mBrightnessContrast[mWorkingChannel][Brightness]);
    fw->setContrast(mBrightnessContrast[mWorkingChannel][Contrast]);

    connect(this, SIGNAL(workingChannelChanged(Filter::WorkingChannel)),
            fw, SLOT(setWorkingChannel(Filter::WorkingChannel)));
    connect(this, SIGNAL(brightnessChanged(int)),
            fw, SLOT(setBrightness(int)));
    connect(this, SIGNAL(contrastChanged(int)),
            fw, SLOT(setContrast(int)));

    connect(fw, SIGNAL(workingChannelChanged(Filter::WorkingChannel)),
            this, SLOT(setWorkingChannel(Filter::WorkingChannel)));
    connect(fw, SIGNAL(brightnessChanged(int)),
            this, SLOT(setBrightness(int)));
    connect(fw, SIGNAL(contrastChanged(int)),
            this, SLOT(setContrast(int)));

    return fw;
}

void Filter::setWorkingChannel(Filter::WorkingChannel s)
{
    if (s == mWorkingChannel)
        return;
    mWorkingChannel = s;
    emit workingChannelChanged(s);
    emit brightnessChanged(mBrightnessContrast[mWorkingChannel][Brightness]);
    emit contrastChanged(mBrightnessContrast[mWorkingChannel][Contrast]);
    emit parametersChanged();
}

void Filter::setBrightness(int v)
{
    if (v == mBrightnessContrast[mWorkingChannel][Brightness])
        return;
    mBrightnessContrast[mWorkingChannel][Brightness] = v;
    makeLUT(mWorkingChannel);
    emit brightnessChanged(v);
    emit parametersChanged();
}

void Filter::setContrast(int v)
{
    if (v == mBrightnessContrast[mWorkingChannel][Contrast])
        return;
    mBrightnessContrast[mWorkingChannel][Contrast] = v;
    makeLUT(mWorkingChannel);
    emit contrastChanged(v);
    emit parametersChanged();
}

void Filter::makeLUT(WorkingChannel c)
{
    double brightness = mBrightnessContrast[c][Brightness];
    double contrast = mBrightnessContrast[c][Contrast] * 127 / 100.0;
    double m, b;
    int value;
    unsigned char * lut = mLuts[c];

    if (contrast < 0.0)
    {
        m = (255.0 - fabs(contrast) * 2.0) / 255.0;
        b = contrast;
    }
    else
    {
        m = 255.0 / (255.0 - contrast * 2.0);
        b = contrast * m;
    }
    for (int i = 0; i < 256; i++)
    {

        if (contrast < 0.0)
        {
            // Contrast
            value = round(m * i) - b;
            // Brightness
            value = value + brightness;
        }
        else
        {
            // Brightness
            value = i + brightness;
            // Contrast
            value = round(m * value) - b;
        }
        lut[i] = qBound(0, value, 255);
    }
}

