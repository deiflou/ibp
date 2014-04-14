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
#include "filterwidget.h"
#include "../imgproc/lut.h"
#include "../imgproc/types.h"

#define MAX_IMAGE_SIZE 512

Filter::Filter() :
    mMode(0),
    mImage()
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mMode = mMode;
    f->mImage = mImage;
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

    QImage i, bg;
    cv::Mat msrc, mdst;
    register BGRA * bits1, * bits2, *bits3;
    register int totalPixels;

    switch (mMode)
    {
    case 0:
        int h, w;
        if (inputImage.width() > MAX_IMAGE_SIZE || inputImage.height() > MAX_IMAGE_SIZE)
        {
            if (inputImage.width() > inputImage.height())
            {
                w = MAX_IMAGE_SIZE;
                h = inputImage.height() * MAX_IMAGE_SIZE / inputImage.width();
            }
            else
            {
                h = MAX_IMAGE_SIZE;
                w = inputImage.width() * MAX_IMAGE_SIZE / inputImage.height();
            }
            i = inputImage.scaled(w, h);
        }
        else
        {
            i = inputImage;
            w = inputImage.width();
            h = inputImage.height();
        }
        bg = QImage(w, h, QImage::Format_ARGB32);
        msrc = cv::Mat(h, w, CV_8UC4, i.bits());
        mdst = cv::Mat(h, w, CV_8UC4, bg.bits());
        cv::medianBlur(msrc, mdst, 5);
        cv::morphologyEx(mdst, msrc, cv::MORPH_CLOSE,
                         cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(30, 30)));
        bg = i.scaled(inputImage.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        break;
    case 1:
        if (mImage.isNull())
            return inputImage;
        bg = mImage.scaled(inputImage.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        break;
    default:
        return inputImage;
    }

    i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    totalPixels = bg.width() * bg.height();
    bits1 = (BGRA*)bg.bits();
    bits2 = (BGRA*)inputImage.bits();
    bits3 = (BGRA*)i.bits();
    while (totalPixels--)
    {
        bits3->r = qMin<int>(lut02[bits2->r][bits1->r], 255);
        bits3->g = qMin<int>(lut02[bits2->g][bits1->g], 255);
        bits3->b = qMin<int>(lut02[bits2->b][bits1->b], 255);
        bits3->a = bits2->a;
        bits1++;
        bits2++;
        bits3++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString m;
    int mode;
    QVariant v;

    m = s.value("mode", "auto").toString();
    if (m == "auto")
        mode = 0;
    else if (m == "fromimage")
        mode = 1;
    else
        return false;

    if (mode == 1)
    {
        v = s.value("image", QImage());
        if (!v.isValid() || !v.canConvert<QImage>())
            return false;
    }

    setMode(mode);
    if (mode == 1)
        setImage(v.value<QImage>());

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("mode", mMode == 0 ? "auto" : "fromimage");
    if (mMode == 1)
        s.setValue("image", mImage);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);

    fw->setMode(mMode);
    fw->setImage(mImage);
    connect(this, SIGNAL(modeChanged(int)), fw, SLOT(setMode(int)));
    connect(this, SIGNAL(imageChanged(QImage)), fw, SLOT(setImage(QImage)));
    connect(fw, SIGNAL(modeChanged(int)), this, SLOT(setMode(int)));
    connect(fw, SIGNAL(imageChanged(QImage)), this, SLOT(setImage(QImage)));
    return fw;
}

void Filter::setMode(int m)
{
    if (m == mMode)
        return;
    mMode = m;
    emit modeChanged(m);
    emit parametersChanged();
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
