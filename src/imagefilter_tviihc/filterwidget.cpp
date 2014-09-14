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

void FilterWidget::setGridSize(int gs)
{
    if (ui->mSpinGridSize->value() == gs)
        return;
    mEmitSignals = false;
    ui->mSpinGridSize->setValue(gs);
    mEmitSignals = true;
    emit gridSizeChanged(gs);
}

void FilterWidget::setOutputMode(Filter::OutputMode om)
{
    if ((om == Filter::CorrectedImage && ui->mButtonOutputModeCorrectedImage->isChecked()) ||
        (om == Filter::IIHCorrectionModel && ui->mButtonOutputModeIIHCorrectionModel->isChecked()))
        return;
    mEmitSignals = false;
    if (om == Filter::CorrectedImage)
        ui->mButtonOutputModeCorrectedImage->setChecked(true);
    else
        ui->mButtonOutputModeIIHCorrectionModel->setChecked(true);
    mEmitSignals = true;
    emit outputModeChanged(om);
}

void FilterWidget::on_mSliderGridSize_valueChanged(int value)
{
    ui->mSpinGridSize->setValue(value);
    if (mEmitSignals)
        emit gridSizeChanged(value);
}

void FilterWidget::on_mSpinGridSize_valueChanged(int arg1)
{
    ui->mSliderGridSize->setValue(arg1);
    if (mEmitSignals)
        emit gridSizeChanged(arg1);
}

void FilterWidget::on_mButtonOutputModeCorrectedImage_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::CorrectedImage);
}

void FilterWidget::on_mButtonOutputModeIIHCorrectionModel_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::IIHCorrectionModel);
}
