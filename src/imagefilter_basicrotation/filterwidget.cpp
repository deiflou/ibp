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

void FilterWidget::setAngle(Filter::Angle a)
{
    if ((a == Filter::_90Clockwise && ui->mButtonAngle90Clockwise->isChecked()) ||
        (a == Filter::_90CounterClockwise && ui->mButtonAngle90CounterClockwise->isChecked()) ||
        (a == Filter::_180 && ui->mButtonAngle180->isChecked()))
        return;

    mEmitSignals = false;
    if (a == Filter::_90Clockwise)
        ui->mButtonAngle90Clockwise->setChecked(true);
    else if (a == Filter::_90CounterClockwise)
        ui->mButtonAngle90CounterClockwise->setChecked(true);
    else
        ui->mButtonAngle180->setChecked(true);
    mEmitSignals = true;

    emit angleChanged(a);
}

void FilterWidget::on_mButtonAngle90Clockwise_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit angleChanged(Filter::_90Clockwise);
}

void FilterWidget::on_mButtonAngle90CounterClockwise_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit angleChanged(Filter::_90CounterClockwise);
}

void FilterWidget::on_mButtonAngle180_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit angleChanged(Filter::_180);
}
