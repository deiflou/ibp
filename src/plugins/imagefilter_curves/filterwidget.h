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
#include "../widgets/levelscurvespaintdelegate.h"

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
    LevelsCurvesPaintDelegate * mCurvesPaintDelegate;

signals:
    void workingChannelChanged(Filter::WorkingChannel s);
    void knotsChanged(const Interpolator1DKnots & k);
    void interpolationModeChanged(Filter::InterpolationMode im);

public slots:
    void setWorkingChannel(Filter::WorkingChannel s);
    void setKnots(const Interpolator1DKnots & k);
    void setInterpolationMode(Filter::InterpolationMode im);

private slots:
    void on_mButtonWorkingChannelLuma_toggled(bool v);
    void on_mButtonWorkingChannelRed_toggled(bool v);
    void on_mButtonWorkingChannelGreen_toggled(bool v);
    void on_mButtonWorkingChannelBlue_toggled(bool v);
    void on_mButtonWorkingChannelAlpha_toggled(bool v);
    void on_mButtonInterpolationModeFlat_toggled(bool v);
    void on_mButtonInterpolationModeLinear_toggled(bool v);
    void on_mButtonInterpolationModeSmooth_toggled(bool v);
    void on_mWidgetCurves_knotsChanged(const Interpolator1DKnots & k);
    void on_mWidgetCurves_selectedKnotChanged(int i);
    void on_mSpinInputValue_valueChanged(double v);
    void on_mSpinOutputValue_valueChanged(double v);
};

#endif // FILTERWIDGET_H
