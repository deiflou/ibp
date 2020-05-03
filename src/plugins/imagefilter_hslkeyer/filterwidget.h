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

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

#include "filter.h"
#include "../widgets/hslkeyingcurvespaintdelegate.h"

using namespace ibp::widgets;

namespace Ui {
class FilterWidget;
}

class FilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget *parent = 0);
    ~FilterWidget();

private:
    Ui::FilterWidget *ui;
    bool mEmitSignals;
    HSLKeyingCurvesPaintDelegate * mHueCurvesPaintDelegate;
    HSLKeyingCurvesPaintDelegate * mSaturationCurvesPaintDelegate;
    HSLKeyingCurvesPaintDelegate * mLightnessCurvesPaintDelegate;

    QVector<double> mZoomFactors;
    int mHueZoomFactorIndex, mSaturationZoomFactorIndex, mLightnessZoomFactorIndex;

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

private slots:
    void on_mButtonHueInterpolationModeFlat_toggled(bool v);
    void on_mButtonHueInterpolationModeLinear_toggled(bool v);
    void on_mButtonHueInterpolationModeSmooth_toggled(bool v);
    void on_mWidgetHueCurve_knotsChanged(const Interpolator1DKnots & k);
    void on_mWidgetHueCurve_selectedKnotChanged(int i);
    void on_mSpinHueInValue_valueChanged(double v);
    void on_mSpinHueOutValue_valueChanged(double v);
    void on_mButtonHueInverted_toggled(bool v);
    void on_mButtonHueZoomIn_clicked();
    void on_mButtonHueZoomOut_clicked();
    void on_mButtonHueZoom100_clicked();

    void on_mButtonSaturationInterpolationModeFlat_toggled(bool v);
    void on_mButtonSaturationInterpolationModeLinear_toggled(bool v);
    void on_mButtonSaturationInterpolationModeSmooth_toggled(bool v);
    void on_mWidgetSaturationCurve_knotsChanged(const Interpolator1DKnots & k);
    void on_mWidgetSaturationCurve_selectedKnotChanged(int i);
    void on_mSpinSaturationInValue_valueChanged(double v);
    void on_mSpinSaturationOutValue_valueChanged(double v);
    void on_mButtonSaturationInverted_toggled(bool v);
    void on_mButtonSaturationZoomIn_clicked();
    void on_mButtonSaturationZoomOut_clicked();
    void on_mButtonSaturationZoom100_clicked();

    void on_mButtonLightnessInterpolationModeFlat_toggled(bool v);
    void on_mButtonLightnessInterpolationModeLinear_toggled(bool v);
    void on_mButtonLightnessInterpolationModeSmooth_toggled(bool v);
    void on_mWidgetLightnessCurve_knotsChanged(const Interpolator1DKnots & k);
    void on_mWidgetLightnessCurve_selectedKnotChanged(int i);
    void on_mSpinLightnessInValue_valueChanged(double v);
    void on_mSpinLightnessOutValue_valueChanged(double v);
    void on_mButtonLightnessInverted_toggled(bool v);
    void on_mButtonLightnessZoomIn_clicked();
    void on_mButtonLightnessZoomOut_clicked();
    void on_mButtonLightnessZoom100_clicked();

    void on_mButtonOutputModeKeyedImage_toggled(bool c);
    void on_mButtonOutputModeMatte_toggled(bool c);

    void on_mSliderPreblurRadius_valueChanged(int value);
    void on_mSpinPreblurRadius_valueChanged(double arg1);
};

#endif // FILTERWIDGET_H
