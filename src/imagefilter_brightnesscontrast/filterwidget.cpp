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
#include <QDebug>

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

void FilterWidget::setWorkingChannel(Filter::WorkingChannel s)
{
    if ((s == Filter::RGB && ui->mButtonWorkingChannelRGB->isChecked()) ||
        (s == Filter::Red && ui->mButtonWorkingChannelRed->isChecked()) ||
        (s == Filter::Green && ui->mButtonWorkingChannelGreen->isChecked()) ||
        (s == Filter::Blue && ui->mButtonWorkingChannelBlue->isChecked()) ||
        (s == Filter::Alpha && ui->mButtonWorkingChannelAlpha->isChecked()))
        return;
    mEmitSignals = false;
    if (s == Filter::RGB)
        ui->mButtonWorkingChannelRGB->setChecked(true);
    else if (s == Filter::Red)
        ui->mButtonWorkingChannelRed->setChecked(true);
    else if (s == Filter::Green)
        ui->mButtonWorkingChannelGreen->setChecked(true);
    else if (s == Filter::Blue)
        ui->mButtonWorkingChannelBlue->setChecked(true);
    else if (s == Filter::Alpha)
        ui->mButtonWorkingChannelAlpha->setChecked(true);
    mEmitSignals = true;
    emit workingChannelChanged(s);
}

void FilterWidget::setBrightness(int v)
{
    if (v == ui->mSpinBrightness->value())
        return;
    mEmitSignals = false;
    ui->mSpinBrightness->setValue(v);
    mEmitSignals = true;
    emit brightnessChanged(v);
}

void FilterWidget::setContrast(int v)
{
    if (v == ui->mSpinContrast->value())
        return;
    mEmitSignals = false;
    ui->mSpinContrast->setValue(v);
    mEmitSignals = true;
    emit contrastChanged(v);
}

void FilterWidget::setUseSoftMode(bool v)
{
    if (v == ui->mButtonOptionsUseSoftMode->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonOptionsUseSoftMode->setChecked(v);
    mEmitSignals = true;
    emit useSoftModeChanged(v);
}

void FilterWidget::on_mButtonWorkingChannelRGB_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::RGB);
}

void FilterWidget::on_mButtonWorkingChannelRed_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::Red);
}

void FilterWidget::on_mButtonWorkingChannelGreen_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::Green);
}

void FilterWidget::on_mButtonWorkingChannelBlue_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::Blue);
}

void FilterWidget::on_mButtonWorkingChannelAlpha_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::Alpha);
}

void FilterWidget::on_mSliderBrightness_valueChanged(int v)
{
    ui->mSpinBrightness->setValue(v);
}

void FilterWidget::on_mSpinBrightness_valueChanged(int v)
{
    ui->mSliderBrightness->setValue(v);
    if (mEmitSignals)
        emit brightnessChanged(v);
}

void FilterWidget::on_mSliderContrast_valueChanged(int v)
{
    ui->mSpinContrast->setValue(v);
}

void FilterWidget::on_mSpinContrast_valueChanged(int v)
{
    ui->mSliderContrast->setValue(v);
    if (mEmitSignals)
        emit contrastChanged(v);
}

void FilterWidget::on_mButtonOptionsUseSoftMode_toggled(bool c)
{
    if (mEmitSignals)
        emit useSoftModeChanged(c);
}
