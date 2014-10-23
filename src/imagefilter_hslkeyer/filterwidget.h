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

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

#include "filter.h"
#include "../widgets/hslkeyingcurvespaintdelegate.h"

using namespace anitools::widgets;

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

private slots:
    void on_mButtonHueInterpolationModeFlat_toggled(bool v);
    void on_mButtonHueInterpolationModeLinear_toggled(bool v);
    void on_mButtonHueInterpolationModeSmooth_toggled(bool v);
    void on_mWidgetHueCurve_knotsChanged(const SplineInterpolatorKnots & k);
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
    void on_mWidgetSaturationCurve_knotsChanged(const SplineInterpolatorKnots & k);
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
    void on_mWidgetLightnessCurve_knotsChanged(const SplineInterpolatorKnots & k);
    void on_mWidgetLightnessCurve_selectedKnotChanged(int i);
    void on_mSpinLightnessInValue_valueChanged(double v);
    void on_mSpinLightnessOutValue_valueChanged(double v);
    void on_mButtonLightnessInverted_toggled(bool v);
    void on_mButtonLightnessZoomIn_clicked();
    void on_mButtonLightnessZoomOut_clicked();
    void on_mButtonLightnessZoom100_clicked();

    void on_mButtonOutputModeKeyedImage_toggled(bool c);
    void on_mButtonOutputModeMatte_toggled(bool c);
};

#endif // FILTERWIDGET_H
