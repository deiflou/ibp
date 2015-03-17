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

    ui->mSliderAbsHue->setVisible(false);
    ui->mSpinAbsHue->setVisible(false);
    ui->mSliderAbsSaturation->setVisible(false);
    ui->mSpinAbsSaturation->setVisible(false);
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setRelHue(int v)
{
    if (ui->mSpinRelHue->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinRelHue->setValue(v);
    mEmitSignals = true;
    emit relHueChanged(v);
}

void FilterWidget::setRelSaturation(int v)
{
    if (ui->mSpinRelSaturation->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinRelSaturation->setValue(v);
    mEmitSignals = true;
    emit relSaturationChanged(v);
}

void FilterWidget::setRelLightness(int v)
{
    if (ui->mSpinRelLightness->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinRelLightness->setValue(v);
    mEmitSignals = true;
    emit relLightnessChanged(v);
}

void FilterWidget::setAbsHue(int v)
{
    if (ui->mSpinAbsHue->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinAbsHue->setValue(v);
    mEmitSignals = true;
    emit absHueChanged(v);
}

void FilterWidget::setAbsSaturation(int v)
{
    if (ui->mSpinAbsSaturation->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinAbsSaturation->setValue(v);
    mEmitSignals = true;
    emit absSaturationChanged(v);
}

void FilterWidget::setColorize(bool v)
{
    if (ui->mButtonColorize->isChecked() == v)
        return;
    mEmitSignals = false;
    ui->mButtonColorize->setChecked(v);
    mEmitSignals = true;
    emit relHueChanged(v);
}

void FilterWidget::on_mSliderRelHue_valueChanged(int v)
{
    ui->mSpinRelHue->setValue(v);
    if (mEmitSignals)
        emit relHueChanged(v);
}

void FilterWidget::on_mSpinRelHue_valueChanged(int v)
{
    ui->mSliderRelHue->setValue(v);
    if (mEmitSignals)
        emit relHueChanged(v);
}

void FilterWidget::on_mSliderRelSaturation_valueChanged(int v)
{
    ui->mSpinRelSaturation->setValue(v);
    if (mEmitSignals)
        emit relSaturationChanged(v);
}

void FilterWidget::on_mSpinRelSaturation_valueChanged(int v)
{
    ui->mSliderRelSaturation->setValue(v);
    if (mEmitSignals)
        emit relSaturationChanged(v);
}

void FilterWidget::on_mSliderRelLightness_valueChanged(int v)
{
    ui->mSpinRelLightness->setValue(v);
    if (mEmitSignals)
        emit relLightnessChanged(v);
}

void FilterWidget::on_mSpinRelLightness_valueChanged(int v)
{
    ui->mSliderRelLightness->setValue(v);
    if (mEmitSignals)
        emit relLightnessChanged(v);
}

void FilterWidget::on_mSliderAbsHue_valueChanged(int v)
{
    ui->mSpinAbsHue->setValue(v);
    if (mEmitSignals)
        emit absHueChanged(v);
}

void FilterWidget::on_mSpinAbsHue_valueChanged(int v)
{
    ui->mSliderAbsHue->setValue(v);
    if (mEmitSignals)
        emit absHueChanged(v);
}

void FilterWidget::on_mSliderAbsSaturation_valueChanged(int v)
{
    ui->mSpinAbsSaturation->setValue(v);
    if (mEmitSignals)
        emit absSaturationChanged(v);
}

void FilterWidget::on_mSpinAbsSaturation_valueChanged(int v)
{
    ui->mSliderAbsSaturation->setValue(v);
    if (mEmitSignals)
        emit absSaturationChanged(v);
}

void FilterWidget::on_mButtonColorize_toggled(bool v)
{
    ui->mSliderRelHue->setVisible(!v);
    ui->mSpinRelHue->setVisible(!v);
    ui->mSliderRelSaturation->setVisible(!v);
    ui->mSpinRelSaturation->setVisible(!v);

    ui->mSliderAbsHue->setVisible(v);
    ui->mSpinAbsHue->setVisible(v);
    ui->mSliderAbsSaturation->setVisible(v);
    ui->mSpinAbsSaturation->setVisible(v);

    if (mEmitSignals)
        emit colorizeChanged(v);
}
