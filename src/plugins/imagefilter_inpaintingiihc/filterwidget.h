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

signals:
    void noiseReductionChanged(double v);
    void maskExpansionChanged(int v);
    void outputModeChanged(Filter::OutputMode v);

public slots:
    void setNoiseReduction(double v);
    void setMaskExpansion(int v);
    void setOutputMode(Filter::OutputMode v);

private slots:
    void on_mSliderNoiseReduction_valueChanged(int v);
    void on_mSpinNoiseReduction_valueChanged(double v);
    void on_mSliderMaskExpansion_valueChanged(int v);
    void on_mSpinMaskExpansion_valueChanged(int v);
    void on_mButtonOutputModeCorrectedImageMode1_toggled(bool v);
    void on_mButtonOutputModeCorrectedImageMode2_toggled(bool v);
    void on_mButtonOutputModeMask_toggled(bool v);
    void on_mButtonOutputModeIIHCorrectionModel_toggled(bool v);
};

#endif // FILTERWIDGET_H
