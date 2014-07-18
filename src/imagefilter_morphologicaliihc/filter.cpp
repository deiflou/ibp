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
#include <QDebug>

#include "filter.h"
#include "../imgproc/lut.h"
#include "../imgproc/types.h"
#include "../imgproc/colorconversion.h"
#include "../misc/util.h"

#define MAX_IMAGE_SIZE 512

Filter::Filter()
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    return new Filter();
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

    register int x, y, w = inputImage.width(), h = inputImage.height();
    HSL * bitsLab = (HSL *)malloc(w * h * sizeof(HSL)), * bitsLabsl;
    cv::Mat mlchannel(h, w, CV_8UC1);
    unsigned char * mlchannelsl;

    convertBGRToHSL(inputImage.bits(), (unsigned char *)bitsLab, w * h);

    for (y = 0; y < h; y++)
    {
        bitsLabsl = bitsLab + y * w;
        mlchannelsl = mlchannel.ptr(y);
        for (x = 0; x < w; x++)
        {
            *mlchannelsl = bitsLabsl->l;
            bitsLabsl++;
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
        cv::Mat mresized(sh, sw, CV_8UC1);
        cv::Mat mresized2(sh, sw, CV_8UC1);
        cv::resize(mlchannel, mresized, cv::Size(sw, sh));
        cv::medianBlur(mresized, mresized2, 5);
        cv::morphologyEx(mresized2, mresized, cv::MORPH_CLOSE,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(30, 30)));
        cv::resize(mresized, mlchannel, cv::Size(w, h));
    }
    else
    {
        cv::Mat mlchannel2(h, w, CV_8UC1);
        cv::medianBlur(mlchannel, mlchannel2, 5);
        cv::morphologyEx(mlchannel2, mlchannel, cv::MORPH_CLOSE,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(30, 30)));
    }

    for (y = 0; y < h; y++)
    {
        bitsLabsl = bitsLab + y * w;
        mlchannelsl = mlchannel.ptr(y);
        for (x = 0; x < w; x++)
        {
            bitsLabsl->l = AT_minimum(lut02[bitsLabsl->l][*mlchannelsl], 255);;
            bitsLabsl++;
            mlchannelsl++;
        }
    }

    QImage i = inputImage.copy();
    convertHSLToBGR((const unsigned char *)bitsLab, i.bits(), w * h);
    free(bitsLab);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    Q_UNUSED(parent)
    return(0);
}
