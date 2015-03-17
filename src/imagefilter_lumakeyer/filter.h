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

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>

#include "../imgproc/imagefilter.h"
#include "../misc/interpolator1D.h"

using namespace anitools::imgproc;
using namespace anitools::misc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    enum InterpolationMode
    {
        Flat,
        Linear,
        Smooth
    };

    enum OutputMode
    {
        KeyedImage,
        Matte
    };

    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    Interpolator1D * mSplineInterpolator;
    InterpolationMode mInterpolationMode;
    bool mIsInverted;
    OutputMode mOutputMode;
    double mPreblurRadius;
    unsigned char mLut[256];

    void makeLUT();

signals:
    void knotsChanged(const Interpolator1DKnots & k);
    void interpolationModeChanged(Filter::InterpolationMode im);
    void invertedChanged(bool i);
    void outputModeChanged(Filter::OutputMode om);
    void preblurRadiusChanged(double pbr);

public slots:
    void setKnots(const Interpolator1DKnots & k);
    void setInterpolationMode(Filter::InterpolationMode im);
    void setInverted(bool i);
    void setOutputMode(Filter::OutputMode om);
    void setPreblurRadius(double pbr);

};

#endif // FILTER_H
