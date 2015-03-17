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

void FilterWidget::setRadius(double s)
{
    if (ui->mSpinRadius->value() == s)
        return;
    mEmitSignals = false;
    ui->mSpinRadius->setValue(s);
    mEmitSignals = true;
    emit radiusChanged(s);
}

void FilterWidget::setBlurRGB(bool v)
{
    if (ui->mButtonAffectedChannelsRGB->isChecked() == v)
        return;
    mEmitSignals = false;
    ui->mButtonAffectedChannelsRGB->setChecked(v);
    mEmitSignals = true;
    emit blurRGBChanged(v);
}

void FilterWidget::setBlurAlpha(bool v)
{
    if (ui->mButtonAffectedChannelsAlpha->isChecked() == v)
        return;
    mEmitSignals = false;
    ui->mButtonAffectedChannelsAlpha->setChecked(v);
    mEmitSignals = true;
    emit blurAlphaChanged(v);
}

void FilterWidget::on_mSliderRadius_valueChanged(int value)
{
    ui->mSpinRadius->setValue(value / 100.0);
    if (mEmitSignals)
        emit radiusChanged(value / 100.0);
}

void FilterWidget::on_mSpinRadius_valueChanged(double arg1)
{
    ui->mSliderRadius->setValue((int)round(arg1 * 100.0));
    if (mEmitSignals)
        emit radiusChanged(arg1);
}

void FilterWidget::on_mButtonAffectedChannelsRGB_toggled(bool c)
{
    if (mEmitSignals)
        emit blurRGBChanged(c);
}

void FilterWidget::on_mButtonAffectedChannelsAlpha_toggled(bool c)
{
    if (mEmitSignals)
        emit blurAlphaChanged(c);
}
