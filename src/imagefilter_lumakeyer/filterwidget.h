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
#include "../widgets/lumakeyingcurvespaintdelegate.h"

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
    LumaKeyingCurvesPaintDelegate * mLumaKeyingCurvesPaintDelegate;

    QVector<double> mZoomFactors;
    int mZoomFactorIndex;

signals:
    void knotsChanged(const SplineInterpolatorKnots & k);
    void interpolationModeChanged(Filter::InterpolationMode im);
    void invertedChanged(bool i);
    void outputModeChanged(Filter::OutputMode om);
    void preblurRadiusChanged(double pbr);

public slots:
    void setKnots(const SplineInterpolatorKnots & k);
    void setInterpolationMode(Filter::InterpolationMode im);
    void setInverted(bool i);
    void setOutputMode(Filter::OutputMode om);
    void setPreblurRadius(double pbr);

private slots:
    void on_mButtonInterpolationModeFlat_toggled(bool v);
    void on_mButtonInterpolationModeLinear_toggled(bool v);
    void on_mButtonInterpolationModeSmooth_toggled(bool v);
    void on_mWidgetCurves_knotsChanged(const SplineInterpolatorKnots & k);
    void on_mWidgetCurves_selectedKnotChanged(int i);
    void on_mSpinLumaValue_valueChanged(double v);
    void on_mSpinOpacityValue_valueChanged(double v);
    void on_mButtonInverted_toggled(bool v);
    void on_mButtonZoomIn_clicked();
    void on_mButtonZoomOut_clicked();
    void on_mButtonZoom100_clicked();
    void on_mButtonOutputModeKeyedImage_toggled(bool c);
    void on_mButtonOutputModeMatte_toggled(bool c);
    void on_mSliderPreblurRadius_valueChanged(int value);
    void on_mSpinPreblurRadius_valueChanged(double arg1);
};

#endif // FILTERWIDGET_H
