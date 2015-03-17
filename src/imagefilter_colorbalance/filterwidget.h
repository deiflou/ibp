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
