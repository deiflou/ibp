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

void FilterWidget::setNoiseReduction(double v)
{
    if (ui->mSpinNoiseReduction->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinNoiseReduction->setValue(v);
    mEmitSignals = true;
    emit noiseReductionChanged(v);
}

void FilterWidget::setMaskExpansion(int v)
{
    if (ui->mSpinMaskExpansion->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinMaskExpansion->setValue(v);
    mEmitSignals = true;
    emit maskExpansionChanged(v);
}

void FilterWidget::setOutputMode(Filter::OutputMode om)
{
    if ((om == Filter::CorrectedImageMode1 && ui->mButtonOutputModeCorrectedImageMode1->isChecked()) ||
        (om == Filter::CorrectedImageMode2 && ui->mButtonOutputModeCorrectedImageMode2->isChecked()) ||
        (om == Filter::Mask && ui->mButtonOutputModeMask->isChecked()) ||
        (om == Filter::IIHCorrectionModel && ui->mButtonOutputModeIIHCorrectionModel->isChecked()))
        return;
    mEmitSignals = false;
    if (om == Filter::CorrectedImageMode1)
        ui->mButtonOutputModeCorrectedImageMode1->setChecked(true);
    else if (om == Filter::CorrectedImageMode2)
        ui->mButtonOutputModeCorrectedImageMode2->setChecked(true);
    else if (om == Filter::Mask)
        ui->mButtonOutputModeMask->setChecked(true);
    else
        ui->mButtonOutputModeIIHCorrectionModel->setChecked(true);
    mEmitSignals = true;
    emit outputModeChanged(om);
}

void FilterWidget::on_mSliderNoiseReduction_valueChanged(int v)
{
    ui->mSpinNoiseReduction->setValue(v / 100.);
    if (mEmitSignals)
        emit noiseReductionChanged(v / 100.);
}

void FilterWidget::on_mSpinNoiseReduction_valueChanged(double v)
{
    ui->mSliderNoiseReduction->setValue(round(v * 100.));
    if (mEmitSignals)
        emit noiseReductionChanged(v);
}

void FilterWidget::on_mSliderMaskExpansion_valueChanged(int v)
{
    ui->mSpinMaskExpansion->setValue(v);
    if (mEmitSignals)
        emit maskExpansionChanged(v);
}

void FilterWidget::on_mSpinMaskExpansion_valueChanged(int v)
{
    ui->mSliderMaskExpansion->setValue(v);
    if (mEmitSignals)
        emit maskExpansionChanged(v);
}

void FilterWidget::on_mButtonOutputModeCorrectedImageMode1_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::CorrectedImageMode1);
}

void FilterWidget::on_mButtonOutputModeCorrectedImageMode2_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::CorrectedImageMode2);
}

void FilterWidget::on_mButtonOutputModeMask_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::Mask);
}

void FilterWidget::on_mButtonOutputModeIIHCorrectionModel_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::IIHCorrectionModel);
}
