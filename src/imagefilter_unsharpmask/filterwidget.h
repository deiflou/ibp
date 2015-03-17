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
    void radiusChanged(double v);
    void amountChanged(int v);
    void thresholdChanged(int v);

public slots:
    void setRadius(double v);
    void setAmount(int v);
    void setThreshold(int v);

private slots:
    void on_mSliderRadius_valueChanged(int v);
    void on_mSpinRadius_valueChanged(double v);
    void on_mSliderAmount_valueChanged(int v);
    void on_mSpinAmount_valueChanged(int v);
    void on_mSliderThreshold_valueChanged(int v);
    void on_mSpinThreshold_valueChanged(int v);
};

#endif // FILTERWIDGET_H
