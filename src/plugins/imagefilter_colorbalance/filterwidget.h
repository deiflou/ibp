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
    void shadowsRedChanged(int v);
    void shadowsGreenChanged(int v);
    void shadowsBlueChanged(int v);
    void midtonesRedChanged(int v);
    void midtonesGreenChanged(int v);
    void midtonesBlueChanged(int v);
    void highlightsRedChanged(int v);
    void highlightsGreenChanged(int v);
    void highlightsBlueChanged(int v);
    void preserveLuminosityChanged(bool v);

public slots:
    void setShadowsRed(int v);
    void setShadowsGreen(int v);
    void setShadowsBlue(int v);
    void setMidtonesRed(int v);
    void setMidtonesGreen(int v);
    void setMidtonesBlue(int v);
    void setHighlightsRed(int v);
    void setHighlightsGreen(int v);
    void setHighlightsBlue(int v);
    void setPreserveLuminosity(bool v);

private slots:
    void on_mSliderShadowsRed_valueChanged(int v);
    void on_mSpinShadowsRed_valueChanged(int v);
    void on_mSliderShadowsGreen_valueChanged(int v);
    void on_mSpinShadowsGreen_valueChanged(int v);
    void on_mSliderShadowsBlue_valueChanged(int v);
    void on_mSpinShadowsBlue_valueChanged(int v);
    void on_mSliderMidtonesRed_valueChanged(int v);
    void on_mSpinMidtonesRed_valueChanged(int v);
    void on_mSliderMidtonesGreen_valueChanged(int v);
    void on_mSpinMidtonesGreen_valueChanged(int v);
    void on_mSliderMidtonesBlue_valueChanged(int v);
    void on_mSpinMidtonesBlue_valueChanged(int v);
    void on_mSliderHighlightsRed_valueChanged(int v);
    void on_mSpinHighlightsRed_valueChanged(int v);
    void on_mSliderHighlightsGreen_valueChanged(int v);
    void on_mSpinHighlightsGreen_valueChanged(int v);
    void on_mSliderHighlightsBlue_valueChanged(int v);
    void on_mSpinHighlightsBlue_valueChanged(int v);

    void on_mButtonTonesShadows_toggled(bool v);
    void on_mButtonTonesMidtones_toggled(bool v);
    void on_mButtonTonesHighlights_toggled(bool v);

    void on_mButtonPreserveLuminosity_toggled(bool v);
};

#endif // FILTERWIDGET_H
