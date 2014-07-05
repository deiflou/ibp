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

void FilterWidget::setRedChannel(bool v)
{
    if (v == ui->mButtonRedChannel->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonRedChannel->setChecked(v);
    mEmitSignals = true;
    emit redChannelToggled(v);
}

void FilterWidget::setGreenChannel(bool v)
{
    if (v == ui->mButtonGreenChannel->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonGreenChannel->setChecked(v);
    mEmitSignals = true;
    emit greenChannelToggled(v);
}

void FilterWidget::setBlueChannel(bool v)
{
    if (v == ui->mButtonBlueChannel->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonBlueChannel->setChecked(v);
    mEmitSignals = true;
    emit blueChannelToggled(v);
}

void FilterWidget::setAlphaChannel(bool v)
{
    if (v == ui->mButtonAlphaChannel->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonAlphaChannel->setChecked(v);
    mEmitSignals = true;
    emit alphaChannelToggled(v);
}

void FilterWidget::on_mButtonRedChannel_toggled(bool c)
{
    if (mEmitSignals)
        emit redChannelToggled(c);
}

void FilterWidget::on_mButtonGreenChannel_toggled(bool c)
{
    if (mEmitSignals)
        emit greenChannelToggled(c);
}

void FilterWidget::on_mButtonBlueChannel_toggled(bool c)
{
    if (mEmitSignals)
        emit blueChannelToggled(c);
}

void FilterWidget::on_mButtonAlphaChannel_toggled(bool c)
{
    if (mEmitSignals)
        emit alphaChannelToggled(c);
}
