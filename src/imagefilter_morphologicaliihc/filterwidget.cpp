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

void FilterWidget::setFeatureSize(int fs)
{
    if (ui->mSpinFeatureSize->value() == fs)
        return;
    mEmitSignals = false;
    ui->mSpinFeatureSize->setValue(fs);
    mEmitSignals = true;
    emit featureSizeChanged(fs);
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
    if (om == Filter::CorrectedImageMode2)
        ui->mButtonOutputModeCorrectedImageMode2->setChecked(true);
    else
        ui->mButtonOutputModeIIHCorrectionModel->setChecked(true);
    mEmitSignals = true;
    emit outputModeChanged(om);
}

void FilterWidget::on_mSliderFeatureSize_valueChanged(int value)
{
    ui->mSpinFeatureSize->setValue(value);
    if (mEmitSignals)
        emit featureSizeChanged(value);
}

void FilterWidget::on_mSpinFeatureSize_valueChanged(int arg1)
{
    ui->mSliderFeatureSize->setValue(arg1);
    if (mEmitSignals)
        emit featureSizeChanged(arg1);
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
