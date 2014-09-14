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

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/lut.h"
#include "../imgproc/types.h"
#include "../imgproc/colorconversion.h"
#include "../misc/util.h"

#define MAX_IMAGE_SIZE 512

Filter::Filter() :
    mFeatureSize(10),
    mOutputMode(CorrectedImage)
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mFeatureSize = mFeatureSize;
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

    register int x, y, w = inputImage.width(), h = inputImage.height(), mean = 0;
    register HSL * bitsHSL = (HSL *)malloc(w * h * sizeof(HSL)), * bitsHSLsl;
    cv::Mat mlchannel(h, w, CV_8UC1);
    register unsigned char * mlchannelsl;
    int size = mFeatureSize == 0 ? 1 : mFeatureSize * 4;

    convertBGRToHSL(inputImage.bits(), (unsigned char *)bitsHSL, w * h);

    for (y = 0; y < h; y++)
    {
        bitsHSLsl = bitsHSL + y * w;
        mlchannelsl = mlchannel.ptr(y);
        for (x = 0; x < w; x++)
        {
            *mlchannelsl = bitsHSLsl->l;
            bitsHSLsl++;
            mlchannelsl++;
        }
    }

    if (w > MAX_IMAGE_SIZE || h > MAX_IMAGE_SIZE)
    {
        int sw, sh;
        if (w > h)
        {
            sw = MAX_IMAGE_SIZE;
            sh = h * MAX_IMAGE_SIZE / w;
        }
        else
        {
            sh = MAX_IMAGE_SIZE;
            sw = w * MAX_IMAGE_SIZE / h;
        }

        double ratio = (double)sw / (double)w;
        size = ceil(size * ratio);

        cv::Mat mresized(sh, sw, CV_8UC1);
        cv::Mat mresized2(sh, sw, CV_8UC1);
        cv::resize(mlchannel, mresized, cv::Size(sw, sh));
        cv::medianBlur(mresized, mresized2, 5);
        cv::morphologyEx(mresized2, mresized, cv::MORPH_CLOSE,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
        cv::resize(mresized, mlchannel, cv::Size(w, h));
    }
    else
    {
        cv::Mat mlchannel2(h, w, CV_8UC1);
        cv::medianBlur(mlchannel, mlchannel2, 5);
        cv::morphologyEx(mlchannel2, mlchannel, cv::MORPH_CLOSE,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
    }

    for (y = 0; y < h; y++)
    {
        mlchannelsl = mlchannel.ptr(y);
        for (x = 0; x < w; x++)
        {
            mean += *mlchannelsl;
            mlchannelsl++;
        }
    }
    mean /= w * h;

    if (mOutputMode == CorrectedImage)
    {
        for (y = 0; y < h; y++)
        {
            bitsHSLsl = bitsHSL + y * w;
            mlchannelsl = mlchannel.ptr(y);
            for (x = 0; x < w; x++)
            {
                bitsHSLsl->l = AT_clamp(0, lut02[bitsHSLsl->l][AT_clamp(1, *mlchannelsl, 255)] * mean / 255, 255);
                bitsHSLsl++;
                mlchannelsl++;
            }
        }
    }
    else
    {
        for (y = 0; y < h; y++)
        {
            bitsHSLsl = bitsHSL + y * w;
            mlchannelsl = mlchannel.ptr(y);
            for (x = 0; x < w; x++)
            {
                bitsHSLsl->h = bitsHSLsl->s = 0;
                bitsHSLsl->l = *mlchannelsl;
                bitsHSLsl++;
                mlchannelsl++;
            }
        }
    }

    QImage i = inputImage.copy();
    convertHSLToBGR((const unsigned char *)bitsHSL, i.bits(), w * h);
    free(bitsHSL);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    int featureSize;
    QString outputModeStr;
    OutputMode outputMode;
    bool ok;

    featureSize = s.value("featuresize", 20).toUInt(&ok);
    if (!ok || featureSize > 200)
        return false;

    outputModeStr = s.value("outputmode", "correctedimage").toString();
    if (outputModeStr == "correctedimage")
        outputMode = CorrectedImage;
    else if (outputModeStr == "iihcorrectionmodel")
        outputMode = IIHCorrectionModel;
    else
        return false;

    setFeatureSize(featureSize);
    setOutputMode(outputMode);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("featuresize", mFeatureSize);
    s.setValue("outputmode", mOutputMode == CorrectedImage ? "correctedimage" : "iihcorrectionmodel");
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setFeatureSize(mFeatureSize);
    fw->setOutputMode(mOutputMode);
    connect(this, SIGNAL(featureSizeChanged(int)), fw, SLOT(setFeatureSize(int)));
    connect(this, SIGNAL(outputModeChanged(Filter::OutputMode)), fw, SLOT(setOutputMode(Filter::OutputMode)));
    connect(fw, SIGNAL(featureSizeChanged(int)), this, SLOT(setFeatureSize(int)));
    connect(fw, SIGNAL(outputModeChanged(Filter::OutputMode)), this, SLOT(setOutputMode(Filter::OutputMode)));
    return fw;
}

void Filter::setFeatureSize(int fs)
{
    if (fs == mFeatureSize)
        return;
    mFeatureSize = fs;
    emit featureSizeChanged(fs);
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
