//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>

#include <imgproc/imagefilter.h>
#include <imgproc/types.h>
#include "../misc/interpolator1D.h"

using namespace ibp::imgproc;
using namespace ibp::misc;

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
    Interpolator1D * mSplineInterpolatorHue, * mSplineInterpolatorSaturation, * mSplineInterpolatorLightness;
    InterpolationMode mInterpolationModeHue, mInterpolationModeSaturation, mInterpolationModeLightness;
    bool mIsInvertedHue, mIsInvertedSaturation, mIsInvertedLightness;
    OutputMode mOutputMode;
    double mPreblurRadius;
    bool mColorize;
    int mRelHue, mRelSaturation, mRelLightness, mAbsHue, mAbsSaturation;
    unsigned char mLutHue[256], mLutSaturation[256], mLutLightness[256];

    void makeLUT(ColorChannel c);

signals:
    void hueKnotsChanged(const Interpolator1DKnots & k);
    void hueInterpolationModeChanged(Filter::InterpolationMode im);
    void hueInvertedChanged(bool i);
    void saturationKnotsChanged(const Interpolator1DKnots & k);
    void saturationInterpolationModeChanged(Filter::InterpolationMode im);
    void saturationInvertedChanged(bool i);
    void lightnessKnotsChanged(const Interpolator1DKnots & k);
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
    void setHueKnots(const Interpolator1DKnots & k);
    void setHueInterpolationMode(Filter::InterpolationMode im);
    void setHueInverted(bool i);
    void setSaturationKnots(const Interpolator1DKnots & k);
    void setSaturationInterpolationMode(Filter::InterpolationMode im);
    void setSaturationInverted(bool i);
    void setLightnessKnots(const Interpolator1DKnots & k);
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
