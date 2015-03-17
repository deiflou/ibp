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

void FilterWidget::setDirection(Filter::Direction d)
{
    if ((d == Filter::Horizontal && ui->mButtonDirectionHorizontal->isChecked()) ||
        (d == Filter::Vertical && ui->mButtonDirectionVertical->isChecked()) ||
        (d == Filter::Both && ui->mButtonDirectionBoth->isChecked()))
        return;

    mEmitSignals = false;
    if (d == Filter::Horizontal)
        ui->mButtonDirectionHorizontal->setChecked(true);
    else if (d == Filter::Vertical)
        ui->mButtonDirectionVertical->setChecked(true);
    else
        ui->mButtonDirectionBoth->setChecked(true);
    mEmitSignals = true;

    emit directionChanged(d);
}

void FilterWidget::on_mButtonDirectionHorizontal_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit directionChanged(Filter::Horizontal);
}

void FilterWidget::on_mButtonDirectionVertical_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit directionChanged(Filter::Vertical);
}

void FilterWidget::on_mButtonDirectionBoth_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit directionChanged(Filter::Both);
}
