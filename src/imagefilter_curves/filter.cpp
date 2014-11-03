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
#include "../misc/nearestneighborsplineinterpolator1D.h"
#include "../misc/linearsplineinterpolator1D.h"
#include "../misc/cubicsplineinterpolator1D.h"

Filter::Filter() :
    mWorkingChannel(RGB)
{
    for (int i = 0; i < 5; i++)
    {
        mInterpolationMode[i] = Smooth;
        mSplineInterpolator[i] = new CubicSplineInterpolator1D();
        mSplineInterpolator[i]->addKnot(0.0, 0.0);
        mSplineInterpolator[i]->addKnot(1.0, 1.0);
        makeLUT((WorkingChannel)i);
    }
}

Filter::~Filter()
{
    for (int i = 0; i < 5; i++)
        if (mSplineInterpolator[i])
            delete mSplineInterpolator[i];
}

ImageFilter * Filter::clone()
{
    Filter * f = new Filter();

    for (int i = 0; i < 5; i++)
    {
        if (f->mSplineInterpolator[i])
            delete f->mSplineInterpolator[i];
        f->mSplineInterpolator[i] = mSplineInterpolator[i]->clone();

        f->mInterpolationMode[i] = mInterpolationMode[i];

        f->makeLUT((WorkingChannel)i);
    }
    f->mWorkingChannel = mWorkingChannel;

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
    QString interpolationModeStr[5];
    InterpolationMode interpolationMode[5];
    QString knotsStr[5];
    QStringList knotsList;
    QStringList knotList;
    double x, y;
    Interpolator1DKnots knots[5];

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

    interpolationModeStr[RGB] = s.value("rgbinterpolationmode", "smooth").toString();
    interpolationModeStr[Red] = s.value("redinterpolationmode", "smooth").toString();
    interpolationModeStr[Green] = s.value("greeninterpolationmode", "smooth").toString();
    interpolationModeStr[Blue] = s.value("blueinterpolationmode", "smooth").toString();
    interpolationModeStr[Alpha] = s.value("alphainterpolationmode", "smooth").toString();
    for (int i = 0; i < 5; i++)
    {
        if (interpolationModeStr[i] == "flat")
            interpolationMode[i] = Flat;
        else if (interpolationModeStr[i] == "linear")
            interpolationMode[i] = Linear;
        else if (interpolationModeStr[i] == "smooth")
            interpolationMode[i] = Smooth;
        else
            return false;
    }

    knotsStr[RGB] = s.value("rgbknots", "0.0 0.0, 1.0 1.0").toString();
    knotsStr[Red] = s.value("redknots", "0.0 0.0, 1.0 1.0").toString();
    knotsStr[Green] = s.value("greenknots", "0.0 0.0, 1.0 1.0").toString();
    knotsStr[Blue] = s.value("blueknots", "0.0 0.0, 1.0 1.0").toString();
    knotsStr[Alpha] = s.value("alphaknots", "0.0 0.0, 1.0 1.0").toString();
    for (int i = 0; i < 5; i++)
    {
        knotsList = knotsStr[i].split(QRegularExpression("\\s*,\\s*"), QString::SkipEmptyParts);
        if (knotsList.size() < 2)
            return false;
        for (int j = 0; j < knotsList.size(); j++)
        {
            knotList = knotsList.at(j).split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
            if (knotList.size() != 2)
                return false;
            x = knotList.at(0).toDouble(&ok);
            if (!ok || x < 0. || x > 1.)
                return false;
            y = knotList.at(1).toDouble(&ok);
            if (!ok || y < 0. || y > 1.)
                return false;
            knots[i].append(Interpolator1DKnot(x, y));
        }
    }

    mWorkingChannel = workingChannel;
    for(int i = 0; i < 5; i++)
    {
        mInterpolationMode[i] = interpolationMode[i];
        delete mSplineInterpolator[i];
        if (mInterpolationMode[i] == Flat)
            mSplineInterpolator[i] = new NearestNeighborSplineInterpolator1D();
        else if (mInterpolationMode[i] == Linear)
            mSplineInterpolator[i] = new LinearSplineInterpolator1D();
        else
            mSplineInterpolator[i] = new CubicSplineInterpolator1D();
        mSplineInterpolator[i]->setKnots(knots[i]);

        makeLUT((WorkingChannel)i);
    }

    emit workingChannelChanged(mWorkingChannel);
    emit interpolationModeChanged(mInterpolationMode[mWorkingChannel]);
    emit knotsChanged(mSplineInterpolator[mWorkingChannel]->knots());

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("workingchannel", mWorkingChannel == RGB ? "rgb" :
                                 mWorkingChannel == Red ? "red" :
                                 mWorkingChannel == Green ? "green" :
                                 mWorkingChannel == Blue ? "blue" :
                                 "alpha");
    QString interpolationModeNameStr[5] = { "rgbinterpolationmode", "redinterpolationmode", "greeninterpolationmode",
                                            "blueinterpolationmode", "alphainterpolationmode" };
    QString knotsNameStr[5] = { "rgbknots", "redknots", "greenknots", "blueknots", "alphaknots" };
    QString knotsStr;
    for(int i = 0; i < 5; i++)
    {
        s.setValue(interpolationModeNameStr[i], mInterpolationMode[i] == Flat ? "flat" :
                                                mInterpolationMode[i] == Linear ? "linear" : "smooth");

        Interpolator1DKnots knots = mSplineInterpolator[i]->knots();
        knotsStr = "";
        for (int j = 0; j < knots.size(); j++)
        {
            knotsStr += QString::number(knots.at(j).x(), 'f', 2) + " " +
                        QString::number(knots.at(j).y(), 'f', 2);
            if (j < knots.size() - 1)
                knotsStr += ", ";
        }
        s.setValue(knotsNameStr[i], knotsStr);
    }

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setWorkingChannel(mWorkingChannel);
    fw->setInterpolationMode(mInterpolationMode[mWorkingChannel]);
    fw->setKnots(mSplineInterpolator[mWorkingChannel]->knots());

    connect(this, SIGNAL(workingChannelChanged(Filter::WorkingChannel)),
            fw, SLOT(setWorkingChannel(Filter::WorkingChannel)));
    connect(this, SIGNAL(interpolationModeChanged(Filter::InterpolationMode)),
            fw, SLOT(setInterpolationMode(Filter::InterpolationMode)));
    connect(this, SIGNAL(knotsChanged(Interpolator1DKnots)),
            fw, SLOT(setKnots(Interpolator1DKnots)));

    connect(fw, SIGNAL(workingChannelChanged(Filter::WorkingChannel)),
            this, SLOT(setWorkingChannel(Filter::WorkingChannel)));
    connect(fw, SIGNAL(interpolationModeChanged(Filter::InterpolationMode)),
            this, SLOT(setInterpolationMode(Filter::InterpolationMode)));
    connect(fw, SIGNAL(knotsChanged(Interpolator1DKnots)),
            this, SLOT(setKnots(Interpolator1DKnots)));

    return fw;
}

void Filter::setWorkingChannel(Filter::WorkingChannel s)
{
    if (s == mWorkingChannel)
        return;
    mWorkingChannel = s;
    emit workingChannelChanged(s);
    emit interpolationModeChanged(mInterpolationMode[mWorkingChannel]);
    emit knotsChanged(mSplineInterpolator[mWorkingChannel]->knots());
    emit parametersChanged();
}

void Filter::setKnots(const Interpolator1DKnots &k)
{
    if (mSplineInterpolator[mWorkingChannel]->knots() == k)
        return;
    mSplineInterpolator[mWorkingChannel]->setKnots(k);
    emit knotsChanged(k);
    emit parametersChanged();
}

void Filter::setInterpolationMode(Filter::InterpolationMode im)
{
    if (mInterpolationMode[mWorkingChannel] == im)
        return;

    mInterpolationMode[mWorkingChannel] = im;

    Interpolator1DKnots k = mSplineInterpolator[mWorkingChannel]->knots();

    delete mSplineInterpolator[mWorkingChannel];

    if (im == 0)
        mSplineInterpolator[mWorkingChannel] = new NearestNeighborSplineInterpolator1D();
    else if (im == 1)
        mSplineInterpolator[mWorkingChannel] = new LinearSplineInterpolator1D();
    else
        mSplineInterpolator[mWorkingChannel] = new CubicSplineInterpolator1D();

    mSplineInterpolator[mWorkingChannel]->setKnots(k);

    makeLUT(mWorkingChannel);

    emit interpolationModeChanged(im);
    emit parametersChanged();
}

void Filter::makeLUT(WorkingChannel c)
{
    double x, y;
    const double inc = 1.0 / 255.0;

    for (x = 0.0; x <= 1.0; x += inc)
    {
        y = mSplineInterpolator[c]->f(x);
        mLuts[c][qBound<int>(0, round(x * 255.0), 255)] = qBound<int>(0, round(y * 255.0), 255);
    }
}
