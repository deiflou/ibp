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

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"
#include "../imgproc/colorconversion.h"
#include "../imgproc/lut.h"
#include "../misc/util.h"

Filter::Filter() :
    mColorize(false),
    mRelHue(0),
    mRelSaturation(0),
    mRelLightness(0),
    mAbsHue(30),
    mAbsSaturation(25)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mColorize = mColorize;
    f->mRelHue = mRelHue;
    f->mRelSaturation = mRelSaturation;
    f->mRelLightness = mRelLightness;
    f->mAbsHue = mAbsHue;
    f->mAbsSaturation = mAbsSaturation;
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
    HSL * inputHSLImage, * bits;
    register int totalSize;
    register int h2, s2, l2;

    if (mRelLightness != 0)
    {
        BGRA * bits2 = (BGRA *)i.bits();
        totalSize = inputImage.width() * inputImage.height();
        l2 = mRelLightness * 255 / 100;

        if (mRelLightness < 0)
        {
            l2 += 255;
            while (totalSize--)
            {
                bits2->r = lut01[bits2->r][l2];
                bits2->g = lut01[bits2->g][l2];
                bits2->b = lut01[bits2->b][l2];

                bits2++;
            }
        }
        else
        {
            while (totalSize--)
            {
                bits2->r += lut01[255 - bits2->r][l2];
                bits2->g += lut01[255 - bits2->g][l2];
                bits2->b += lut01[255 - bits2->b][l2];

                bits2++;
            }
        }

    }

    if (!mColorize)
    {
        if (mRelHue != 0 || mRelSaturation != 0)
        {
            totalSize = inputImage.width() * inputImage.height();
            inputHSLImage = (HSL *)malloc(totalSize * sizeof(HSL));
            convertBGRToHSL((unsigned char *)i.bits(), (unsigned char *)inputHSLImage, totalSize);
            bits = inputHSLImage;

            h2 = mRelHue * 127 / 180;
            s2 = mRelSaturation * 255 / 100;
            while (totalSize--)
            {
                bits->h = (bits->h + h2 + 256) % 256;
                if (bits->s > 0)
                    bits->s = s2 < 0 ?
                              lut01[bits->s][s2 + 255] :
                              s2 == 255 ? 255 : AT_minimum(lut02[bits->s][255 - s2], 255);

                bits++;
            }

            convertHSLToBGR((unsigned char *)inputHSLImage, (unsigned char *)i.bits(),
                            inputImage.width() * inputImage.height());
            free(inputHSLImage);
        }
    }
    else
    {
        totalSize = inputImage.width() * inputImage.height();
        inputHSLImage = (HSL *)malloc(totalSize * sizeof(HSL));
        convertBGRToHSL((unsigned char *)i.bits(), (unsigned char *)inputHSLImage, totalSize);
        bits = inputHSLImage;

        h2 = mAbsHue * 255 / 360;
        s2 = mAbsSaturation * 255 / 100;
        while (totalSize--)
        {
            bits->h = h2;
            bits->s = s2;

            bits++;
        }

        convertHSLToBGR((unsigned char *)inputHSLImage, (unsigned char *)i.bits(),
                        inputImage.width() * inputImage.height());
        free(inputHSLImage);
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    int relHue, relSaturation, relLightness, absHue, absSaturation;
    bool colorize;
    bool ok;
    relHue = s.value("relhue", 0).toInt(&ok);
    if (!ok)
        return false;
    relSaturation = s.value("relsaturation", 0).toInt(&ok);
    if (!ok)
        return false;
    relLightness = s.value("rellightness", 0).toInt(&ok);
    if (!ok)
        return false;
    absHue = s.value("abshue", 30).toInt(&ok);
    if (!ok)
        return false;
    absSaturation = s.value("abssaturation", 25).toInt(&ok);
    if (!ok)
        return false;
    colorize = s.value("colorize", false).toBool();

    setRelHue(relHue);
    setRelSaturation(relSaturation);
    setRelLightness(relLightness);
    setAbsHue(absHue);
    setAbsSaturation(absSaturation);
    setColorize(colorize);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("relhue", mRelHue);
    s.setValue("relsaturation", mRelSaturation);
    s.setValue("rellightness", mRelLightness);
    s.setValue("abshue", mAbsHue);
    s.setValue("abssaturation", mAbsSaturation);
    s.setValue("colorize", mColorize);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setRelHue(mRelHue);
    fw->setRelSaturation(mRelSaturation);
    fw->setRelLightness(mRelLightness);
    fw->setAbsHue(mAbsHue);
    fw->setAbsSaturation(mAbsSaturation);
    fw->setColorize(mColorize);

    connect(this, SIGNAL(relHueChanged(int)), fw, SLOT(setRelHue(int)));
    connect(this, SIGNAL(relSaturationChanged(int)), fw, SLOT(setRelSaturation(int)));
    connect(this, SIGNAL(relLightnessChanged(int)), fw, SLOT(setRelLightness(int)));
    connect(this, SIGNAL(absHueChanged(int)), fw, SLOT(setAbsHue(int)));
    connect(this, SIGNAL(absSaturationChanged(int)), fw, SLOT(setAbsSaturation(int)));
    connect(this, SIGNAL(colorizeChanged(bool)), fw, SLOT(setColorize(bool)));

    connect(fw, SIGNAL(relHueChanged(int)), this, SLOT(setRelHue(int)));
    connect(fw, SIGNAL(relSaturationChanged(int)), this, SLOT(setRelSaturation(int)));
    connect(fw, SIGNAL(relLightnessChanged(int)), this, SLOT(setRelLightness(int)));
    connect(fw, SIGNAL(absHueChanged(int)), this, SLOT(setAbsHue(int)));
    connect(fw, SIGNAL(absSaturationChanged(int)), this, SLOT(setAbsSaturation(int)));
    connect(fw, SIGNAL(colorizeChanged(bool)), this, SLOT(setColorize(bool)));

    return fw;
}

void Filter::setRelHue(int v)
{
    if (v == mRelHue)
        return;
    mRelHue = v;
    emit relHueChanged(v);
    emit parametersChanged();
}

void Filter::setRelSaturation(int v)
{
    if (v == mRelSaturation)
        return;
    mRelSaturation = v;
    emit relSaturationChanged(v);
    emit parametersChanged();
}

void Filter::setRelLightness(int v)
{
    if (v == mRelLightness)
        return;
    mRelLightness = v;
    emit relLightnessChanged(v);
    emit parametersChanged();
}

void Filter::setAbsHue(int v)
{
    if (v == mAbsHue)
        return;
    mAbsHue = v;
    emit absHueChanged(v);
    emit parametersChanged();
}

void Filter::setAbsSaturation(int v)
{
    if (v == mAbsSaturation)
        return;
    mAbsSaturation = v;
    emit absSaturationChanged(v);
    emit parametersChanged();
}

void Filter::setColorize(bool v)
{
    if (v == mColorize)
        return;
    mColorize = v;
    emit colorizeChanged(v);
    emit parametersChanged();
}
