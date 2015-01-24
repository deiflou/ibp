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
    void on_mButtonOutputModeCorrectedImage_toggled(bool v);
    void on_mButtonOutputModeMask_toggled(bool v);
    void on_mButtonOutputModeIIHCorrectionModel_toggled(bool v);
};

#endif // FILTERWIDGET_H
