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

void FilterWidget::setInputBlackPoint(double v)
{
    if (v == ui->mWidgetInputLevels->blackPoint())
        return;
    mEmitSignals = false;
    ui->mWidgetInputLevels->setBlackPoint(v);
    mEmitSignals = true;
    emit inputBlackPointChanged(v);
}

void FilterWidget::setInputGamma(double v)
{
    if (v == ui->mWidgetInputLevels->gammaCorrection())
        return;
    mEmitSignals = false;
    ui->mWidgetInputLevels->setGammaCorrection(v);
    mEmitSignals = true;
    emit inputGammaChanged(v);
}

void FilterWidget::setInputWhitePoint(double v)
{
    if (v == ui->mWidgetInputLevels->whitePoint())
        return;
    mEmitSignals = false;
    ui->mWidgetInputLevels->setWhitePoint(v);
    mEmitSignals = true;
    emit inputWhitePointChanged(v);
}

void FilterWidget::setOutputBlackPoint(double v)
{
    if (v == ui->mWidgetOutputLevels->blackPoint())
        return;
    mEmitSignals = false;
    ui->mWidgetOutputLevels->setBlackPoint(v);
    mEmitSignals = true;
    emit outputBlackPointChanged(v);
}

void FilterWidget::setOutputWhitePoint(double v)
{
    if (v == ui->mWidgetOutputLevels->whitePoint())
        return;
    mEmitSignals = false;
    ui->mWidgetOutputLevels->setWhitePoint(v);
    mEmitSignals = true;
    emit outputWhitePointChanged(v);
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

void FilterWidget::on_mWidgetInputLevels_blackPointChanged(double v)
{
    if (mEmitSignals)
        emit inputBlackPointChanged(v);
}

void FilterWidget::on_mWidgetInputLevels_whitePointChanged(double v)
{
    if (mEmitSignals)
        emit inputWhitePointChanged(v);
}

void FilterWidget::on_mWidgetInputLevels_gammaCorrectionChanged(double v)
{
    if (mEmitSignals)
        emit inputGammaChanged(v);
}

void FilterWidget::on_mWidgetOutputLevels_blackPointChanged(double v)
{
    if (mEmitSignals)
        emit outputBlackPointChanged(v);
}

void FilterWidget::on_mWidgetOutputLevels_whitePointChanged(double v)
{
    if (mEmitSignals)
        emit outputWhitePointChanged(v);
}

