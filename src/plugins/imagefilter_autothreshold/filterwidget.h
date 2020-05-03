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

signals:
    void thresholdModeChanged(int m);
    void colorModeChanged(int m);
    void affectedChannelChanged(int c, bool a);

public slots:
    void setThresholdMode(int m);
    void setColorMode(int m);
    void setAffectedChannel(int c, bool a);

private slots:
    void on_mButtonThresholdModeGlobal_toggled(bool checked);
    void on_mButtonThresholdModeLocal_toggled(bool checked);
    void on_mButtonColorModeLuma_toggled(bool checked);
    void on_mButtonColorModeRGB_toggled(bool checked);
    void on_mButtonAffectedChannelsLuma_toggled(bool checked);
    void on_mButtonAffectedChannelsRed_toggled(bool checked);
    void on_mButtonAffectedChannelsGreen_toggled(bool checked);
    void on_mButtonAffectedChannelsBlue_toggled(bool checked);
    void on_mButtonAffectedChannelsAlpha_toggled(bool checked);
};

#endif // FILTERWIDGET_H
