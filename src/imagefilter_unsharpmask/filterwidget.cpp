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

void FilterWidget::setRadius(double v)
{
    if (ui->mSpinRadius->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinRadius->setValue(v);
    mEmitSignals = true;
    emit radiusChanged(v);
}

void FilterWidget::setAmount(unsigned int v)
{
    if (ui->mSpinAmount->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinAmount->setValue(v);
    mEmitSignals = true;
    emit amountChanged(v);
}

void FilterWidget::setThreshold(unsigned int v)
{
    if (ui->mSpinThreshold->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinThreshold->setValue(v);
    mEmitSignals = true;
    emit thresholdChanged(v);
}

void FilterWidget::on_mSliderRadius_valueChanged(int v)
{
    ui->mSpinRadius->setValue(v / 100.0);
    if (mEmitSignals)
        emit radiusChanged(v / 100.0);
}

void FilterWidget::on_mSpinRadius_valueChanged(double v)
{
    ui->mSliderRadius->setValue((int)round(v * 100.0));
    if (mEmitSignals)
        emit radiusChanged(v);
}

void FilterWidget::on_mSliderAmount_valueChanged(int v)
{
    ui->mSpinAmount->setValue(v);
    if (mEmitSignals)
        emit amountChanged(v);
}

void FilterWidget::on_mSpinAmount_valueChanged(int v)
{
    ui->mSliderAmount->setValue(v);
    if (mEmitSignals)
        emit amountChanged(v);
}

void FilterWidget::on_mSliderThreshold_valueChanged(int v)
{
    ui->mSpinThreshold->setValue(v);
    if (mEmitSignals)
        emit thresholdChanged(v);
}

void FilterWidget::on_mSpinThreshold_valueChanged(int v)
{
    ui->mSliderThreshold->setValue(v);
    if (mEmitSignals)
        emit thresholdChanged(v);
}
