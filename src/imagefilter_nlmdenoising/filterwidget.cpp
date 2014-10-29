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

#include <math.h>

#include "filterwidget.h"
#include "ui_filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true)
{
    ui->setupUi(this);
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setStrength(double s)
{
    if (ui->mSpinStrength->value() == s)
        return;
    mEmitSignals = false;
    ui->mSpinStrength->setValue(s);
    mEmitSignals = true;
    emit strengthChanged(s);
}

void FilterWidget::on_mSliderStrength_valueChanged(int value)
{
    ui->mSpinStrength->setValue(value / 100.);
    if (mEmitSignals)
        emit strengthChanged(value / 100.);
}

void FilterWidget::on_mSpinStrength_valueChanged(double arg1)
{
    ui->mSliderStrength->setValue((int)round(arg1 * 100.));
    if (mEmitSignals)
        emit strengthChanged(arg1);
}
