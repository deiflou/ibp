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
#include <math.h>

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"
#include "../imgproc/lut.h"
#include "../misc/nearestneighborsplineinterpolator.h"
#include "../misc/linearsplineinterpolator.h"
#include "../misc/cubicsplineinterpolator.h"
#include "../misc/util.h"

Filter::Filter() :
    mSplineInterpolator(0),
    mInterpolationMode(Smooth),
    mIsInverted(false),
    mOutputMode(KeyedImage)
{
    mSplineInterpolator = new CubicSplineInterpolator();
    mSplineInterpolator->addKnot(0.0, 0.0);
    mSplineInterpolator->addKnot(1.0, 1.0);
    makeLUT();
}

Filter::~Filter()
{
    if (mSplineInterpolator)
        delete mSplineInterpolator;
}

ImageFilter * Filter::clone()
{
    Filter * f = new Filter();

    if (f->mSplineInterpolator)
        delete f->mSplineInterpolator;
    f->mSplineInterpolator = mSplineInterpolator->clone();
    f->mInterpolationMode = mInterpolationMode;
    f->mIsInverted = mIsInverted;
    f->mOutputMode = mOutputMode;
    f->makeLUT();

    return f;
}

extern "C" QHash<QString, QString> getAnitoolsPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getAnitoolsPluginInfo();
}

QImage Filter::process(const QImage & inputImage)
{
    if (inputImage.isNull() || inputImage.format() != QImage::Format_ARGB32)
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);

    register BGRA * bits = (BGRA*)inputImage.bits();
    register BGRA * bits2 = (BGRA*)i.bits();
    register int totalPixels = i.width() * i.height();
    register unsigned int gR = .2126 * 0x10000;
    register unsigned int gG = .7152 * 0x10000;
    register unsigned int gB = .0722 * 0x10000;

    if (mOutputMode == KeyedImage)
        while (totalPixels--)
        {
            bits2->r = bits->r;
            bits2->g = bits->g;
            bits2->b = bits->b;
            bits2->a = lut01[bits->a][mLut[(bits->r * gR >> 16) + (bits->g * gG >> 16) + (bits->b * gB >> 16)]];
            bits++;
            bits2++;
        }
    else
        while (totalPixels--)
        {
            bits2->r = bits2->g = bits2->b =
                    lut01[bits->a][mLut[(bits->r * gR >> 16) + (bits->g * gG >> 16) + (bits->b * gB >> 16)]];
            bits2->a = 255;
            bits++;
            bits2++;
        }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString interpolationModeStr;
    InterpolationMode interpolationMode;
    QString knotsStr;
    QStringList knotsList;
    QStringList knotList;
    double x, y;
    SplineInterpolatorKnots knots;
    bool isInverted;
    QString outputModeStr;
    OutputMode outputMode;
    bool ok;

    interpolationModeStr = s.value("interpolationmode", "smooth").toString();
    if (interpolationModeStr == "flat")
        interpolationMode = Flat;
    else if (interpolationModeStr == "linear")
        interpolationMode = Linear;
    else if (interpolationModeStr == "smooth")
        interpolationMode = Smooth;
    else
        return false;

    knotsStr = s.value("knots", "0.0 0.0, 1.0 1.0").toString();
    knotsList = knotsStr.split(QRegularExpression("\\s*,\\s*"), QString::SkipEmptyParts);
    if (knotsList.size() < 2)
        return false;
    for (int i = 0; i < knotsList.size(); i++)
    {
        knotList = knotsList.at(i).split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        if (knotList.size() != 2)
            return false;
        x = knotList.at(0).toDouble(&ok);
        if (!ok || x < 0. || x > 1.)
            return false;
        y = knotList.at(1).toDouble(&ok);
        if (!ok || y < 0. || y > 1.)
            return false;
        knots.append(SplineInterpolatorKnot(x, y));
    }

    isInverted = s.value("isinverted", false).toBool();

    outputModeStr = s.value("outputmode", "keyedimage").toString();
    if (outputModeStr == "keyedimage")
        outputMode = KeyedImage;
    else if (outputModeStr == "matte")
        outputMode = Matte;
    else
        return false;

    setInterpolationMode(interpolationMode);
    setKnots(knots);
    setInverted(isInverted);
    setOutputMode(outputMode);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("interpolationmode", mInterpolationMode == Flat ? "flat" :
                                    mInterpolationMode == Linear ? "linear" : "smooth");

    SplineInterpolatorKnots knots = mSplineInterpolator->knots();
    QString knotsStr = "";
    for (int i = 0; i < knots.size(); i++)
    {
        knotsStr += QString::number(knots.at(i).x(), 'f', 2) + " " +
                    QString::number(knots.at(i).y(), 'f', 2);
        if (i < knots.size() - 1)
            knotsStr += ", ";
    }
    s.setValue("knots", knotsStr);

    s.setValue("isinverted", mIsInverted);

    s.setValue("outputmode", mOutputMode == KeyedImage ? "keyedimage" : "matte");

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setInterpolationMode(mInterpolationMode);
    fw->setKnots(mSplineInterpolator->knots());
    fw->setInverted(mIsInverted);
    fw->setOutputMode(mOutputMode);

    connect(this, SIGNAL(interpolationModeChanged(Filter::InterpolationMode)),
            fw, SLOT(setInterpolationMode(Filter::InterpolationMode)));
    connect(this, SIGNAL(knotsChanged(SplineInterpolatorKnots)),
            fw, SLOT(setKnots(SplineInterpolatorKnots)));
    connect(this, SIGNAL(invertedChanged(bool)),
            fw, SLOT(setInverted(bool)));
    connect(this, SIGNAL(outputModeChanged(Filter::OutputMode)),
            fw, SLOT(setOutputMode(Filter::OutputMode)));

    connect(fw, SIGNAL(interpolationModeChanged(Filter::InterpolationMode)),
            this, SLOT(setInterpolationMode(Filter::InterpolationMode)));
    connect(fw, SIGNAL(knotsChanged(SplineInterpolatorKnots)),
            this, SLOT(setKnots(SplineInterpolatorKnots)));
    connect(fw, SIGNAL(invertedChanged(bool)),
            this, SLOT(setInverted(bool)));
    connect(fw, SIGNAL(outputModeChanged(Filter::OutputMode)),
            this, SLOT(setOutputMode(Filter::OutputMode)));

    return fw;
}

void Filter::setKnots(const SplineInterpolatorKnots &k)
{
    if (mSplineInterpolator->knots() == k)
        return;
    mSplineInterpolator->setKnots(k);
    makeLUT();
    emit knotsChanged(k);
    emit parametersChanged();
}

void Filter::setInterpolationMode(Filter::InterpolationMode im)
{
    if (mInterpolationMode == im)
        return;

    mInterpolationMode = im;

    SplineInterpolator * tmp = mSplineInterpolator;

    if (im == Flat)
        mSplineInterpolator = new NearestNeighborSplineInterpolator();
    else if (im == Linear)
        mSplineInterpolator = new LinearSplineInterpolator();
    else
        mSplineInterpolator = new CubicSplineInterpolator();

    mSplineInterpolator->setKnots(tmp->knots());

    delete tmp;

    makeLUT();

    emit interpolationModeChanged(im);
    emit parametersChanged();
}

void Filter::setInverted(bool i)
{
    if (mIsInverted == i)
        return;

    mIsInverted = i;

    makeLUT();

    emit invertedChanged(i);
    emit parametersChanged();
}

void Filter::setOutputMode(Filter::OutputMode om)
{
    if (mOutputMode == om)
        return;

    mOutputMode = om;

    emit outputModeChanged(om);
    emit parametersChanged();
}


void Filter::makeLUT()
{
    if (mIsInverted)
        for (int i = 0; i < 256; i++)
            mLut[i] = AT_clamp(0, round(mSplineInterpolator->f(i / 255.) * 255.), 255);
    else
        for (int i = 0; i < 256; i++)
            mLut[i] = 255 - AT_clamp(0, round(mSplineInterpolator->f(i / 255.) * 255.), 255);
}
