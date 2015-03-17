/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
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
