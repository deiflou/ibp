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

void FilterWidget::setRefinement(int v)
{
    if (ui->mSpinRefinement->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinRefinement->setValue(v);
    mEmitSignals = true;
    emit refinementChanged(v);
}

void FilterWidget::setSmoothness(int v)
{
    if (ui->mSpinSmoothness->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinSmoothness->setValue(v);
    mEmitSignals = true;
    emit smoothnessChanged(v);
}

void FilterWidget::setOutputMode(Filter::OutputMode om)
{
    if ((om == Filter::CorrectedImageMode1 && ui->mButtonOutputModeCorrectedImageMode1->isChecked()) ||
        (om == Filter::CorrectedImageMode2 && ui->mButtonOutputModeCorrectedImageMode2->isChecked()) ||
        (om == Filter::IIHCorrectionModel && ui->mButtonOutputModeIIHCorrectionModel->isChecked()))
        return;
    mEmitSignals = false;
    if (om == Filter::CorrectedImageMode1)
        ui->mButtonOutputModeCorrectedImageMode1->setChecked(true);
    else if (om == Filter::CorrectedImageMode2)
        ui->mButtonOutputModeCorrectedImageMode2->setChecked(true);
    else
        ui->mButtonOutputModeIIHCorrectionModel->setChecked(true);
    mEmitSignals = true;
    emit outputModeChanged(om);
}

void FilterWidget::on_mSliderRefinement_valueChanged(int value)
{
    ui->mSpinRefinement->setValue(value);
    if (mEmitSignals)
        emit refinementChanged(value);
}

void FilterWidget::on_mSpinRefinement_valueChanged(int arg1)
{
    ui->mSliderRefinement->setValue(arg1);
    if (mEmitSignals)
        emit refinementChanged(arg1);
}

void FilterWidget::on_mSliderSmoothness_valueChanged(int value)
{
    ui->mSpinSmoothness->setValue(value);
    if (mEmitSignals)
        emit smoothnessChanged(value);
}

void FilterWidget::on_mSpinSmoothness_valueChanged(int arg1)
{
    ui->mSliderSmoothness->setValue(arg1);
    if (mEmitSignals)
        emit smoothnessChanged(arg1);
}

void FilterWidget::on_mButtonOutputModeCorrectedImageMode1_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::CorrectedImageMode1);
}

void FilterWidget::on_mButtonOutputModeCorrectedImageMode2_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::CorrectedImageMode2);
}

void FilterWidget::on_mButtonOutputModeIIHCorrectionModel_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::IIHCorrectionModel);
}
