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
#include <QToolButton>
#include <QSlider>
#include <QSpinBox>

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

    QToolButton * mButtonAffectedChannel[5];
    QSpinBox * mSpinThreshold[5];

signals:
    void colorModeChanged(int m);
    void affectedChannelChanged(int c, bool a);
    void thresholdChanged(int c, int t);

public slots:
    void setColorMode(int m);
    void setAffectedChannel(int c, bool a);
    void setThreshold(int c, int t);

private slots:
    void on_mButtonColorModeLuma_toggled(bool checked);
    void on_mButtonColorModeRGB_toggled(bool checked);
    void on_mButtonAffectedChannelsLuma_toggled(bool checked);
    void on_mButtonAffectedChannelsRed_toggled(bool checked);
    void on_mButtonAffectedChannelsGreen_toggled(bool checked);
    void on_mButtonAffectedChannelsBlue_toggled(bool checked);
    void on_mButtonAffectedChannelsAlpha_toggled(bool checked);
    void on_mSliderLuma_valueChanged(int value);
    void on_mSpinLuma_valueChanged(int arg1);
    void on_mSliderRed_valueChanged(int value);
    void on_mSpinRed_valueChanged(int arg1);
    void on_mSliderGreen_valueChanged(int value);
    void on_mSpinGreen_valueChanged(int arg1);
    void on_mSliderBlue_valueChanged(int value);
    void on_mSpinBlue_valueChanged(int arg1);
    void on_mSliderAlpha_valueChanged(int value);
    void on_mSpinAlpha_valueChanged(int arg1);
};

#endif // FILTERWIDGET_H
