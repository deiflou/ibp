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
    void workingChannelChanged(Filter::WorkingChannel s);
    void inputBlackPointChanged(double v);
    void inputGammaChanged(double v);
    void inputWhitePointChanged(double v);
    void outputBlackPointChanged(double v);
    void outputWhitePointChanged(double v);

public slots:
    void setWorkingChannel(Filter::WorkingChannel s);
    void setInputBlackPoint(double v);
    void setInputGamma(double v);
    void setInputWhitePoint(double v);
    void setOutputBlackPoint(double v);
    void setOutputWhitePoint(double v);

private slots:
    void on_mButtonWorkingChannelLuma_toggled(bool c);
    void on_mButtonWorkingChannelRed_toggled(bool c);
    void on_mButtonWorkingChannelGreen_toggled(bool c);
    void on_mButtonWorkingChannelBlue_toggled(bool c);
    void on_mButtonWorkingChannelAlpha_toggled(bool c);

    void on_mWidgetInputLevels_blackPointChanged(double v);
    void on_mWidgetInputLevels_whitePointChanged(double v);
    void on_mWidgetInputLevels_gammaCorrectionChanged(double v);

    void on_mWidgetOutputLevels_blackPointChanged(double v);
    void on_mWidgetOutputLevels_whitePointChanged(double v);
};

#endif // FILTERWIDGET_H
