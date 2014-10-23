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
#include "../imgproc/colorconversion.h"
#include "../misc/nearestneighborsplineinterpolator.h"
#include "../misc/linearsplineinterpolator.h"
#include "../misc/cubicsplineinterpolator.h"
#include "../misc/util.h"

Filter::Filter() :
    mSplineInterpolatorHue(0),
    mSplineInterpolatorSaturation(0),
    mSplineInterpolatorLightness(0),
    mInterpolationModeHue(Smooth),
    mInterpolationModeSaturation(Smooth),
    mInterpolationModeLightness(Smooth),
    mIsInvertedHue(false),
    mIsInvertedSaturation(false),
    mIsInvertedLightness(false),
    mOutputMode(KeyedImage)
{
    mSplineInterpolatorHue = new CubicSplineInterpolator();
    mSplineInterpolatorHue->addKnot(.0, 0.);
    mSplineInterpolatorHue->addKnot(.33, 1.);
    mSplineInterpolatorHue->addKnot(.66, 0.);
    makeLUT(ColorChannel_Hue);
    mSplineInterpolatorSaturation = new CubicSplineInterpolator();
    mSplineInterpolatorSaturation->addKnot(0., 0.);
    mSplineInterpolatorSaturation->addKnot(1., 1.);
    makeLUT(ColorChannel_Saturation);
    mSplineInterpolatorLightness = new CubicSplineInterpolator();
    mSplineInterpolatorLightness->addKnot(0., 0.);
    mSplineInterpolatorLightness->addKnot(.5, 1.);
    mSplineInterpolatorLightness->addKnot(1., 0.);
    makeLUT(ColorChannel_Lightness);
}

Filter::~Filter()
{
    if (mSplineInterpolatorHue)
        delete mSplineInterpolatorHue;
    if (mSplineInterpolatorSaturation)
        delete mSplineInterpolatorSaturation;
    if (mSplineInterpolatorLightness)
        delete mSplineInterpolatorLightness;
}

ImageFilter * Filter::clone()
{
    Filter * f = new Filter();

    if (f->mSplineInterpolatorHue)
        delete f->mSplineInterpolatorHue;
    f->mSplineInterpolatorHue = mSplineInterpolatorHue->clone();
    f->mInterpolationModeHue = mInterpolationModeHue;
    f->mIsInvertedHue = mIsInvertedHue;
    f->makeLUT(ColorChannel_Hue);

    if (f->mSplineInterpolatorSaturation)
        delete f->mSplineInterpolatorSaturation;
    f->mSplineInterpolatorSaturation = mSplineInterpolatorSaturation->clone();
    f->mInterpolationModeSaturation = mInterpolationModeSaturation;
    f->mIsInvertedSaturation = mIsInvertedSaturation;
    f->makeLUT(ColorChannel_Saturation);

    if (f->mSplineInterpolatorLightness)
        delete f->mSplineInterpolatorLightness;
    f->mSplineInterpolatorLightness = mSplineInterpolatorLightness->clone();
    f->mInterpolationModeLightness = mInterpolationModeLightness;
    f->mIsInvertedLightness = mIsInvertedLightness;
    f->makeLUT(ColorChannel_Lightness);

    f->mOutputMode = mOutputMode;

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
    register int totalPixels = i.width() * i.height();
    HSL * hslImage = (HSL *)malloc(totalPixels * sizeof(HSL));
    register BGRA * bits = (BGRA*)inputImage.bits();
    register BGRA * bits2 = (BGRA*)i.bits();
    register HSL * bitsHSL = hslImage;

    convertBGRToHSL(inputImage.bits(), (unsigned char *)hslImage, totalPixels);

    if (mOutputMode == KeyedImage)
        while (totalPixels--)
        {
            bits2->r = bits->r;
            bits2->g = bits->g;
            bits2->b = bits->b;
            bits2->a = lut01[bits->a][255 -
                       lut01[mLutHue[bitsHSL->h]][lut01[mLutSaturation[bitsHSL->s]][mLutLightness[bitsHSL->l]]]];;
            bits++;
            bits2++;
            bitsHSL++;
        }
    else
        while (totalPixels--)
        {
            bits2->r = bits2->g = bits2->b =
                    lut01[bits->a][255 -
                    lut01[mLutHue[bitsHSL->h]][
                    lut01[mLutSaturation[bitsHSL->s]][
                    mLutLightness[bitsHSL->l]]]];
            bits2->a = 255;
            bits++;
            bits2++;
            bitsHSL++;
        }

    free(hslImage);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString interpolationModeStr;
    InterpolationMode interpolationModeHue, interpolationModeSaturation, interpolationModeLightness;
    QString knotsStr;
    QStringList knotsList;
    QStringList knotList;
    double x, y;
    SplineInterpolatorKnots knotsHue, knotsSaturation, knotsLightness;
    bool isInvertedHue, isInvertedSaturation, isInvertedLightness;
    QString outputModeStr;
    OutputMode outputMode;
    bool ok;

    interpolationModeStr = s.value("hueinterpolationmode", "smooth").toString();
    if (interpolationModeStr == "flat")
        interpolationModeHue = Flat;
    else if (interpolationModeStr == "linear")
        interpolationModeHue = Linear;
    else if (interpolationModeStr == "smooth")
        interpolationModeHue = Smooth;
    else
        return false;
    interpolationModeStr = s.value("saturationinterpolationmode", "smooth").toString();
    if (interpolationModeStr == "flat")
        interpolationModeSaturation = Flat;
    else if (interpolationModeStr == "linear")
        interpolationModeSaturation = Linear;
    else if (interpolationModeStr == "smooth")
        interpolationModeSaturation = Smooth;
    else
        return false;
    interpolationModeStr = s.value("lightnessinterpolationmode", "smooth").toString();
    if (interpolationModeStr == "flat")
        interpolationModeLightness = Flat;
    else if (interpolationModeStr == "linear")
        interpolationModeLightness = Linear;
    else if (interpolationModeStr == "smooth")
        interpolationModeLightness = Smooth;
    else
        return false;

    knotsStr = s.value("hueknots", "0.0 0.0, 1.0 1.0").toString();
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
        knotsHue.append(SplineInterpolatorKnot(x, y));
    }
    // append last knot to make it periodic
    knotsHue.append(SplineInterpolatorKnot(AT_maximum(1.001, knotsHue.first().x() + 1.), knotsHue.first().y()));
    knotsStr = s.value("saturationknots", "0.0 0.0, 1.0 1.0").toString();
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
        knotsSaturation.append(SplineInterpolatorKnot(x, y));
    }
    knotsStr = s.value("lightnessknots", "0.0 0.0, 1.0 1.0").toString();
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
        knotsLightness.append(SplineInterpolatorKnot(x, y));
    }

    isInvertedHue = s.value("hueisinverted", false).toBool();
    isInvertedSaturation = s.value("saturationisinverted", false).toBool();
    isInvertedLightness = s.value("lightnessisinverted", false).toBool();

    outputModeStr = s.value("outputmode", "keyedimage").toString();
    if (outputModeStr == "keyedimage")
        outputMode = KeyedImage;
    else if (outputModeStr == "matte")
        outputMode = Matte;
    else
        return false;

    setHueInterpolationMode(interpolationModeHue);
    setHueKnots(knotsHue);
    setHueInverted(isInvertedHue);
    setSaturationInterpolationMode(interpolationModeSaturation);
    setSaturationKnots(knotsSaturation);
    setSaturationInverted(isInvertedSaturation);
    setLightnessInterpolationMode(interpolationModeLightness);
    setLightnessKnots(knotsLightness);
    setLightnessInverted(isInvertedLightness);
    setOutputMode(outputMode);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("hueinterpolationmode", mInterpolationModeHue == Flat ? "flat" :
                                       mInterpolationModeHue == Linear ? "linear" : "smooth");
    s.setValue("saturationinterpolationmode", mInterpolationModeSaturation == Flat ? "flat" :
                                              mInterpolationModeSaturation == Linear ? "linear" : "smooth");
    s.setValue("lightnessinterpolationmode", mInterpolationModeLightness == Flat ? "flat" :
                                             mInterpolationModeLightness == Linear ? "linear" : "smooth");

    SplineInterpolatorKnots knots = mSplineInterpolatorHue->knots();
    QString knotsStr = "";
    for (int i = 0; i < knots.size() - 1; i++)
    {
        knotsStr += QString::number(knots.at(i).x(), 'f', 2) + " " +
                    QString::number(knots.at(i).y(), 'f', 2);
        if (i < knots.size() - 2)
            knotsStr += ", ";
    }
    s.setValue("hueknots", knotsStr);
    knots = mSplineInterpolatorSaturation->knots();
    knotsStr = "";
    for (int i = 0; i < knots.size(); i++)
    {
        knotsStr += QString::number(knots.at(i).x(), 'f', 2) + " " +
                    QString::number(knots.at(i).y(), 'f', 2);
        if (i < knots.size() - 1)
            knotsStr += ", ";
    }
    s.setValue("saturationknots", knotsStr);
    knots = mSplineInterpolatorLightness->knots();
    knotsStr = "";
    for (int i = 0; i < knots.size(); i++)
    {
        knotsStr += QString::number(knots.at(i).x(), 'f', 2) + " " +
                    QString::number(knots.at(i).y(), 'f', 2);
        if (i < knots.size() - 1)
            knotsStr += ", ";
    }
    s.setValue("lightnessknots", knotsStr);

    s.setValue("hueisinverted", mIsInvertedHue);
    s.setValue("saturationisinverted", mIsInvertedSaturation);
    s.setValue("lightnessisinverted", mIsInvertedLightness);

    s.setValue("outputmode", mOutputMode == KeyedImage ? "keyedimage" : "matte");

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setHueInterpolationMode(mInterpolationModeHue);
    fw->setHueKnots(mSplineInterpolatorHue->knots());
    fw->setHueInverted(mIsInvertedHue);
    fw->setSaturationInterpolationMode(mInterpolationModeSaturation);
    fw->setSaturationKnots(mSplineInterpolatorSaturation->knots());
    fw->setSaturationInverted(mIsInvertedSaturation);
    fw->setLightnessInterpolationMode(mInterpolationModeLightness);
    fw->setLightnessKnots(mSplineInterpolatorLightness->knots());
    fw->setLightnessInverted(mIsInvertedLightness);
    fw->setOutputMode(mOutputMode);

    connect(this, SIGNAL(hueInterpolationModeChanged(Filter::InterpolationMode)),
            fw, SLOT(setHueInterpolationMode(Filter::InterpolationMode)));
    connect(this, SIGNAL(hueKnotsChanged(SplineInterpolatorKnots)),
            fw, SLOT(setHueKnots(SplineInterpolatorKnots)));
    connect(this, SIGNAL(hueInvertedChanged(bool)),
            fw, SLOT(setHueInverted(bool)));
    connect(this, SIGNAL(saturationInterpolationModeChanged(Filter::InterpolationMode)),
            fw, SLOT(setSaturationInterpolationMode(Filter::InterpolationMode)));
    connect(this, SIGNAL(saturationKnotsChanged(SplineInterpolatorKnots)),
            fw, SLOT(setSaturationKnots(SplineInterpolatorKnots)));
    connect(this, SIGNAL(saturationInvertedChanged(bool)),
            fw, SLOT(setSaturationInverted(bool)));
    connect(this, SIGNAL(lightnessInterpolationModeChanged(Filter::InterpolationMode)),
            fw, SLOT(setLightnessInterpolationMode(Filter::InterpolationMode)));
    connect(this, SIGNAL(lightnessKnotsChanged(SplineInterpolatorKnots)),
            fw, SLOT(setLightnessKnots(SplineInterpolatorKnots)));
    connect(this, SIGNAL(lightnessInvertedChanged(bool)),
            fw, SLOT(setLightnessInverted(bool)));
    connect(this, SIGNAL(outputModeChanged(Filter::OutputMode)),
            fw, SLOT(setOutputMode(Filter::OutputMode)));

    connect(fw, SIGNAL(hueInterpolationModeChanged(Filter::InterpolationMode)),
            this, SLOT(setHueInterpolationMode(Filter::InterpolationMode)));
    connect(fw, SIGNAL(hueKnotsChanged(SplineInterpolatorKnots)),
            this, SLOT(setHueKnots(SplineInterpolatorKnots)));
    connect(fw, SIGNAL(hueInvertedChanged(bool)),
            this, SLOT(setHueInverted(bool)));
    connect(fw, SIGNAL(saturationInterpolationModeChanged(Filter::InterpolationMode)),
            this, SLOT(setSaturationInterpolationMode(Filter::InterpolationMode)));
    connect(fw, SIGNAL(saturationKnotsChanged(SplineInterpolatorKnots)),
            this, SLOT(setSaturationKnots(SplineInterpolatorKnots)));
    connect(fw, SIGNAL(saturationInvertedChanged(bool)),
            this, SLOT(setSaturationInverted(bool)));
    connect(fw, SIGNAL(lightnessInterpolationModeChanged(Filter::InterpolationMode)),
            this, SLOT(setLightnessInterpolationMode(Filter::InterpolationMode)));
    connect(fw, SIGNAL(lightnessKnotsChanged(SplineInterpolatorKnots)),
            this, SLOT(setLightnessKnots(SplineInterpolatorKnots)));
    connect(fw, SIGNAL(lightnessInvertedChanged(bool)),
            this, SLOT(setLightnessInverted(bool)));
    connect(fw, SIGNAL(outputModeChanged(Filter::OutputMode)),
            this, SLOT(setOutputMode(Filter::OutputMode)));

    return fw;
}

void Filter::setHueKnots(const SplineInterpolatorKnots &k)
{
    if (mSplineInterpolatorHue->knots() == k)
        return;
    mSplineInterpolatorHue->setKnots(k);
    makeLUT(ColorChannel_Hue);
    emit hueKnotsChanged(k);
    emit parametersChanged();
}

void Filter::setHueInterpolationMode(Filter::InterpolationMode im)
{
    if (mInterpolationModeHue == im)
        return;

    mInterpolationModeHue = im;

    SplineInterpolator * tmp = mSplineInterpolatorHue;

    if (im == Flat)
        mSplineInterpolatorHue = new NearestNeighborSplineInterpolator();
    else if (im == Linear)
        mSplineInterpolatorHue = new LinearSplineInterpolator();
    else
        mSplineInterpolatorHue = new CubicSplineInterpolator();

    mSplineInterpolatorHue->setKnots(tmp->knots());

    delete tmp;

    makeLUT(ColorChannel_Hue);

    emit hueInterpolationModeChanged(im);
    emit parametersChanged();
}

void Filter::setHueInverted(bool i)
{
    if (mIsInvertedHue == i)
        return;

    mIsInvertedHue = i;

    makeLUT(ColorChannel_Hue);

    emit hueInvertedChanged(i);
    emit parametersChanged();
}

void Filter::setSaturationKnots(const SplineInterpolatorKnots &k)
{
    if (mSplineInterpolatorSaturation->knots() == k)
        return;
    mSplineInterpolatorSaturation->setKnots(k);
    makeLUT(ColorChannel_Saturation);
    emit saturationKnotsChanged(k);
    emit parametersChanged();
}

void Filter::setSaturationInterpolationMode(Filter::InterpolationMode im)
{
    if (mInterpolationModeSaturation == im)
        return;

    mInterpolationModeSaturation = im;

    SplineInterpolator * tmp = mSplineInterpolatorSaturation;

    if (im == Flat)
        mSplineInterpolatorSaturation = new NearestNeighborSplineInterpolator();
    else if (im == Linear)
        mSplineInterpolatorSaturation = new LinearSplineInterpolator();
    else
        mSplineInterpolatorSaturation = new CubicSplineInterpolator();

    mSplineInterpolatorSaturation->setKnots(tmp->knots());

    delete tmp;

    makeLUT(ColorChannel_Saturation);

    emit saturationInterpolationModeChanged(im);
    emit parametersChanged();
}

void Filter::setSaturationInverted(bool i)
{
    if (mIsInvertedSaturation == i)
        return;

    mIsInvertedSaturation = i;

    makeLUT(ColorChannel_Saturation);

    emit saturationInvertedChanged(i);
    emit parametersChanged();
}

void Filter::setLightnessKnots(const SplineInterpolatorKnots &k)
{
    if (mSplineInterpolatorLightness->knots() == k)
        return;
    mSplineInterpolatorLightness->setKnots(k);
    makeLUT(ColorChannel_Lightness);
    emit lightnessKnotsChanged(k);
    emit parametersChanged();
}

void Filter::setLightnessInterpolationMode(Filter::InterpolationMode im)
{
    if (mInterpolationModeLightness == im)
        return;

    mInterpolationModeLightness = im;

    SplineInterpolator * tmp = mSplineInterpolatorLightness;

    if (im == Flat)
        mSplineInterpolatorLightness = new NearestNeighborSplineInterpolator();
    else if (im == Linear)
        mSplineInterpolatorLightness = new LinearSplineInterpolator();
    else
        mSplineInterpolatorLightness = new CubicSplineInterpolator();

    mSplineInterpolatorLightness->setKnots(tmp->knots());

    delete tmp;

    makeLUT(ColorChannel_Lightness);

    emit lightnessInterpolationModeChanged(im);
    emit parametersChanged();
}

void Filter::setLightnessInverted(bool i)
{
    if (mIsInvertedLightness == i)
        return;

    mIsInvertedLightness = i;

    makeLUT(ColorChannel_Lightness);

    emit lightnessInvertedChanged(i);
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

void Filter::makeLUT(ColorChannel c)
{
    unsigned char * lut = c == ColorChannel_Hue ? mLutHue :
                          c == ColorChannel_Saturation ? mLutSaturation : mLutLightness;
    SplineInterpolator * splineInterpolator = c == ColorChannel_Hue ? mSplineInterpolatorHue :
                                              c == ColorChannel_Saturation ? mSplineInterpolatorSaturation :
                                              mSplineInterpolatorLightness;
    bool inverted = c == ColorChannel_Hue ? mIsInvertedHue :
                    c == ColorChannel_Saturation ? mIsInvertedSaturation : mIsInvertedLightness;

    if (inverted)
        for (int i = 0; i < 256; i++)
            lut[i] = 255 - AT_clamp(0, round(splineInterpolator->f(i / 255.) * 255.), 255);
    else
        for (int i = 0; i < 256; i++)
            lut[i] = AT_clamp(0, round(splineInterpolator->f(i / 255.) * 255.), 255);
}
