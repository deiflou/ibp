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
