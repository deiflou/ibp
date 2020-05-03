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
    void enhanceChannelsSeparatelyChanged(bool v);
    void adjustMidtonesChanged(bool v);
    void targetColorShadowsChanged(const QColor & v);
    void targetColorMidtonesChanged(const QColor & v);
    void targetColorHighlightsChanged(const QColor & v);
    void clippingShadowsChanged(double v);
    void clippingHighlightsChanged(double v);

public slots:
    void setEnhanceChannelsSeparately(bool v);
    void setAdjustMidtones(bool v);
    void setTargetColorShadows(const QColor & v);
    void setTargetColorMidtones(const QColor & v);
    void setTargetColorHighlights(const QColor & v);
    void setClippingShadows(double v);
    void setClippingHighlights(double v);

private slots:
    void on_mButtonHistogramOptionsEnhanceChannelsSeparately_toggled(bool v);
    void on_mButtonHistogramOptionsAdjustMidtones_toggled(bool v);
    void on_mButtonTargetColorsShadows_colorChanged(const QColor & v);
    void on_mButtonTargetColorsMidtones_colorChanged(const QColor & v);
    void on_mButtonTargetColorsHighlights_colorChanged(const QColor & v);
    void on_mSpinClippingShadows_valueChanged(double v);
    void on_mSpinClippingHighlights_valueChanged(double v);
};

#endif // FILTERWIDGET_H
