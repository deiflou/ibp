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
    void colorizeChanged(bool v);
    void relHueChanged(int v);
    void relSaturationChanged(int v);
    void relLightnessChanged(int v);
    void absHueChanged(int v);
    void absSaturationChanged(int v);

public slots:
    void setColorize(bool v);
    void setRelHue(int v);
    void setRelSaturation(int v);
    void setRelLightness(int v);
    void setAbsHue(int v);
    void setAbsSaturation(int v);

private slots:
    void on_mSliderRelHue_valueChanged(int v);
    void on_mSpinRelHue_valueChanged(int v);
    void on_mSliderRelSaturation_valueChanged(int v);
    void on_mSpinRelSaturation_valueChanged(int v);
    void on_mSliderAbsHue_valueChanged(int v);
    void on_mSpinAbsHue_valueChanged(int v);
    void on_mSliderAbsSaturation_valueChanged(int v);
    void on_mSpinAbsSaturation_valueChanged(int v);
    void on_mSliderRelLightness_valueChanged(int v);
    void on_mSpinRelLightness_valueChanged(int v);
    void on_mButtonColorize_toggled(bool v);
};

#endif // FILTERWIDGET_H
