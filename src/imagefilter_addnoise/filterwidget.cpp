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

void FilterWidget::setAmount(double v)
{
    if (qFuzzyCompare(ui->mSpinAmount->value(), v))
        return;
    mEmitSignals = false;
    ui->mSpinAmount->setValue(v);
    mEmitSignals = true;
    emit amountChanged(v);
}

void FilterWidget::setDistribution(Filter::Distribution v)
{
    if ((v == Filter::Uniform && ui->mButtonDistributionUniform->isChecked()) ||
        (v == Filter::Gaussian && ui->mButtonDistributionGaussian->isChecked()))
        return;
    mEmitSignals = false;
    if (v == Filter::Uniform)
        ui->mButtonDistributionUniform->setChecked(true);
    else
        ui->mButtonDistributionGaussian->setChecked(true);
    mEmitSignals = true;
    emit distributionChanged(v);
}

void FilterWidget::setColorMode(Filter::ColorMode v)
{
    if ((v == Filter::Monochromatic && ui->mButtonColorModeMonochromatic->isChecked()) ||
        (v == Filter::Color && ui->mButtonColorModeColor->isChecked()))
        return;
    mEmitSignals = false;
    if (v == Filter::Monochromatic)
        ui->mButtonColorModeMonochromatic->setChecked(true);
    else
        ui->mButtonColorModeColor->setChecked(true);
    mEmitSignals = true;
    emit colorModeChanged(v);
}

void FilterWidget::on_mSliderAmount_valueChanged(int v)
{
    ui->mSpinAmount->setValue(v / 100.0);
    if (mEmitSignals)
        emit amountChanged(v / 100.0);
}

void FilterWidget::on_mSpinAmount_valueChanged(double v)
{
    ui->mSliderAmount->setValue((int)round(v * 100.0));
    if (mEmitSignals)
        emit amountChanged(v);
}

void FilterWidget::on_mButtonDistributionUniform_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit distributionChanged(Filter::Uniform);
}

void FilterWidget::on_mButtonDistributionGaussian_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit distributionChanged(Filter::Gaussian);
}

void FilterWidget::on_mButtonColorModeMonochromatic_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit colorModeChanged(Filter::Monochromatic);
}

void FilterWidget::on_mButtonColorModeColor_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit colorModeChanged(Filter::Color);
}
