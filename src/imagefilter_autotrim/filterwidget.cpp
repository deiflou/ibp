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
    connect(ui->mSpinMarginsTop, SIGNAL(valueChanged(int)), this, SLOT(On_mSpinMargins_valueChanged(int)));
    connect(ui->mSpinMarginsLeft, SIGNAL(valueChanged(int)), this, SLOT(On_mSpinMargins_valueChanged(int)));
    connect(ui->mSpinMarginsBottom, SIGNAL(valueChanged(int)), this, SLOT(On_mSpinMargins_valueChanged(int)));
    connect(ui->mSpinMarginsRight, SIGNAL(valueChanged(int)), this, SLOT(On_mSpinMargins_valueChanged(int)));
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setReference(Filter::Reference v)
{
    if ((v == Filter::AlphaChannel && ui->mButtonReferenceAlphaChannel->isChecked()) ||
        (v == Filter::Luma && ui->mButtonReferenceLuma->isChecked()))
        return;
    mEmitSignals = false;
    if (v == Filter::AlphaChannel)
        ui->mButtonReferenceAlphaChannel->setChecked(true);
    else
        ui->mButtonReferenceLuma->setChecked(true);
    mEmitSignals = true;
    emit referenceChanged(v);
}

void FilterWidget::setThreshold(int v)
{
    if (ui->mSpinThreshold->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinThreshold->setValue(v);
    mEmitSignals = true;
    emit thresholdChanged(v);
}

void FilterWidget::setMargins(const QMargins &v)
{
    if (ui->mSpinMarginsTop->value() == v.top() && ui->mSpinMarginsLeft->value() == v.left() &&
        ui->mSpinMarginsBottom->value() == v.bottom() && ui->mSpinMarginsRight->value() == v.left())
        return;
    mEmitSignals = false;
    ui->mSpinMarginsTop->setValue(v.top());
    ui->mSpinMarginsLeft->setValue(v.left());
    ui->mSpinMarginsBottom->setValue(v.bottom());
    ui->mSpinMarginsRight->setValue(v.right());
    mEmitSignals = true;
    emit marginsChanged(v);
}

void FilterWidget::on_mButtonReferenceAlphaChannel_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit referenceChanged(Filter::AlphaChannel);
}

void FilterWidget::on_mButtonReferenceLuma_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit referenceChanged(Filter::Luma);
}

void FilterWidget::on_mSliderThreshold_valueChanged(int v)
{
    ui->mSpinThreshold->setValue(v);
    if (mEmitSignals)
        emit thresholdChanged(v);
}

void FilterWidget::on_mSpinThreshold_valueChanged(int v)
{
    ui->mSliderThreshold->setValue(v);
    if (mEmitSignals)
        emit thresholdChanged(v);
}

void FilterWidget::On_mSpinMargins_valueChanged(int v)
{
    Q_UNUSED(v)
    if (mEmitSignals)
        emit marginsChanged(QMargins(ui->mSpinMarginsLeft->value(), ui->mSpinMarginsTop->value(),
                                     ui->mSpinMarginsRight->value(), ui->mSpinMarginsBottom->value()));
}
