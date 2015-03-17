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
#include "../imgproc/lut.h"
#include "../imgproc/types.h"
#include "../imgproc/colorconversion.h"
#include "../misc/util.h"

Filter::Filter() :
    mImage(),
    mOutputMode(CorrectedImageMode1)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mImage = mImage;
    f->mOutputMode = mOutputMode;
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

    if (mImage.isNull())
        return inputImage;

    QImage bg = mImage.scaled(inputImage.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    register int w = inputImage.width(), h = inputImage.height(), totalPixels = w * h;
    register HSL * bitsHSL = 0, * bitsHSLsl, * bitsHSLbg = 0, * bitsHSLbgsl;

    bitsHSL = (HSL *)malloc(w * h * sizeof(HSL));
    if (!bitsHSL)
        return inputImage;
    bitsHSLbg = (HSL *)malloc(w * h * sizeof(HSL));
    if (!bitsHSLbg)
    {
        free(bitsHSL);
        return inputImage;
    }
    convertBGRToHSL(inputImage.bits(), (unsigned char *)bitsHSL, w * h);
    convertBGRToHSL(bg.bits(), (unsigned char *)bitsHSLbg, w * h);

    bitsHSLbgsl = bitsHSLbg;
    bitsHSLsl = bitsHSL;
    totalPixels = w * h;
    if (mOutputMode == CorrectedImageMode1)
    {
        register int mean = 0;
        bitsHSLbgsl = bitsHSLbg;
        totalPixels = w * h;
        while (totalPixels--)
        {
            mean += bitsHSLbgsl->l;
            bitsHSLbgsl++;
        }
        mean /= w * h;

        bitsHSLbgsl = bitsHSLbg;
        totalPixels = w * h;
        while (totalPixels--)
        {
            bitsHSLsl->l = AT_clamp(0, lut02[bitsHSLsl->l][AT_clamp(1, bitsHSLbgsl->l, 255)] * mean / 255, 255);
            bitsHSLsl++;
            bitsHSLbgsl++;
        }
    }
    else if (mOutputMode == CorrectedImageMode2)
    {
        while (totalPixels--)
        {
            bitsHSLsl->l = AT_clamp(0, lut02[bitsHSLsl->l][AT_clamp(1, bitsHSLbgsl->l, 255)], 255);
            bitsHSLsl++;
            bitsHSLbgsl++;
        }
    }
    else
    {
        while (totalPixels--)
        {
            bitsHSLsl->h = bitsHSLsl->s = 0;
            bitsHSLsl->l = bitsHSLbgsl->l;
            bitsHSLsl++;
            bitsHSLbgsl++;
        }
    }

    QImage i = inputImage.copy();
    convertHSLToBGR((unsigned char *)bitsHSL, i.bits(), w * h);
    free(bitsHSL);
    free(bitsHSLbg);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QVariant v;
    QString outputModeStr;
    OutputMode outputMode;

    v = s.value("image", QImage());
    if (!v.isValid() || !v.canConvert<QImage>())
        return false;

    outputModeStr = s.value("outputmode", "correctedimagemode1").toString();
    if (outputModeStr == "correctedimagemode1")
        outputMode = CorrectedImageMode1;
    else if (outputModeStr == "correctedimagemode2")
        outputMode = CorrectedImageMode2;
    else if (outputModeStr == "iihcorrectionmodel")
        outputMode = IIHCorrectionModel;
    else
        return false;

    setImage(v.value<QImage>());
    setOutputMode(outputMode);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("image", mImage);
    s.setValue("outputmode", mOutputMode == CorrectedImageMode1 ? "correctedimagemode1" :
                             mOutputMode == CorrectedImageMode2 ? "correctedimagemode2" : "iihcorrectionmodel");
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setImage(mImage);
    fw->setOutputMode(mOutputMode);
    connect(this, SIGNAL(imageChanged(QImage)), fw, SLOT(setImage(QImage)));
    connect(this, SIGNAL(outputModeChanged(Filter::OutputMode)), fw, SLOT(setOutputMode(Filter::OutputMode)));
    connect(fw, SIGNAL(imageChanged(QImage)), this, SLOT(setImage(QImage)));
    connect(fw, SIGNAL(outputModeChanged(Filter::OutputMode)), this, SLOT(setOutputMode(Filter::OutputMode)));
    return fw;
}

void Filter::setImage(const QImage &i)
{
    if (i == mImage)
        return;
    if (i.format() != QImage::Format_ARGB32)
        mImage = i.convertToFormat(QImage::Format_ARGB32);
    else
        mImage = i;
    emit imageChanged(i);
    emit parametersChanged();
}

void Filter::setOutputMode(Filter::OutputMode om)
{
    if (om == mOutputMode)
        return;
    mOutputMode = om;
    emit outputModeChanged(om);
    emit parametersChanged();
}
