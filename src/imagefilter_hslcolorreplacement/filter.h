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

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>

#include "../imgproc/imagefilter.h"
#include "../imgproc/types.h"
#include "../misc/splineinterpolator.h"

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
        CorrectedImage,
        Mask
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
    SplineInterpolator * mSplineInterpolatorHue, * mSplineInterpolatorSaturation, * mSplineInterpolatorLightness;
    InterpolationMode mInterpolationModeHue, mInterpolationModeSaturation, mInterpolationModeLightness;
    bool mIsInvertedHue, mIsInvertedSaturation, mIsInvertedLightness;
    OutputMode mOutputMode;
    double mPreblurRadius;
    bool mColorize;
    int mRelHue, mRelSaturation, mRelLightness, mAbsHue, mAbsSaturation;
    unsigned char mLutHue[256], mLutSaturation[256], mLutLightness[256];

    void makeLUT(ColorChannel c);

signals:
    void hueKnotsChanged(const SplineInterpolatorKnots & k);
    void hueInterpolationModeChanged(Filter::InterpolationMode im);
    void hueInvertedChanged(bool i);
    void saturationKnotsChanged(const SplineInterpolatorKnots & k);
    void saturationInterpolationModeChanged(Filter::InterpolationMode im);
    void saturationInvertedChanged(bool i);
    void lightnessKnotsChanged(const SplineInterpolatorKnots & k);
    void lightnessInterpolationModeChanged(Filter::InterpolationMode im);
    void lightnessInvertedChanged(bool i);
    void outputModeChanged(Filter::OutputMode om);
    void preblurRadiusChanged(double pbr);
    void colorizeChanged(bool v);
    void relHueChanged(int v);
    void relSaturationChanged(int v);
    void relLightnessChanged(int v);
    void absHueChanged(int v);
    void absSaturationChanged(int v);

public slots:
    void setHueKnots(const SplineInterpolatorKnots & k);
    void setHueInterpolationMode(Filter::InterpolationMode im);
    void setHueInverted(bool i);
    void setSaturationKnots(const SplineInterpolatorKnots & k);
    void setSaturationInterpolationMode(Filter::InterpolationMode im);
    void setSaturationInverted(bool i);
    void setLightnessKnots(const SplineInterpolatorKnots & k);
    void setLightnessInterpolationMode(Filter::InterpolationMode im);
    void setLightnessInverted(bool i);
    void setOutputMode(Filter::OutputMode om);
    void setPreblurRadius(double pbr);
    void setColorize(bool v);
    void setRelHue(int v);
    void setRelSaturation(int v);
    void setRelLightness(int v);
    void setAbsHue(int v);
    void setAbsSaturation(int v);
};

#endif // FILTER_H
