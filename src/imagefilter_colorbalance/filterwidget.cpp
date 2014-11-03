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

    ui->mSliderShadowsRed->setVisible(false);
    ui->mSliderShadowsGreen->setVisible(false);
    ui->mSliderShadowsBlue->setVisible(false);
    ui->mSliderHighlightsRed->setVisible(false);
    ui->mSliderHighlightsGreen->setVisible(false);
    ui->mSliderHighlightsBlue->setVisible(false);
    ui->mSpinShadowsRed->setVisible(false);
    ui->mSpinShadowsGreen->setVisible(false);
    ui->mSpinShadowsBlue->setVisible(false);
    ui->mSpinHighlightsRed->setVisible(false);
    ui->mSpinHighlightsGreen->setVisible(false);
    ui->mSpinHighlightsBlue->setVisible(false);
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setShadowsRed(int v)
{
    if (ui->mSpinShadowsRed->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinShadowsRed->setValue(v);
    mEmitSignals = true;
    emit shadowsRedChanged(v);
}

void FilterWidget::setShadowsGreen(int v)
{
    if (ui->mSpinShadowsGreen->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinShadowsGreen->setValue(v);
    mEmitSignals = true;
    emit shadowsGreenChanged(v);
}

void FilterWidget::setShadowsBlue(int v)
{
    if (ui->mSpinShadowsBlue->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinShadowsBlue->setValue(v);
    mEmitSignals = true;
    emit shadowsBlueChanged(v);
}

void FilterWidget::setMidtonesRed(int v)
{
    if (ui->mSpinMidtonesRed->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinMidtonesRed->setValue(v);
    mEmitSignals = true;
    emit midtonesRedChanged(v);
}

void FilterWidget::setMidtonesGreen(int v)
{
    if (ui->mSpinMidtonesGreen->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinMidtonesGreen->setValue(v);
    mEmitSignals = true;
    emit midtonesGreenChanged(v);
}

void FilterWidget::setMidtonesBlue(int v)
{
    if (ui->mSpinMidtonesBlue->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinMidtonesBlue->setValue(v);
    mEmitSignals = true;
    emit midtonesBlueChanged(v);
}

void FilterWidget::setHighlightsRed(int v)
{
    if (ui->mSpinHighlightsRed->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinHighlightsRed->setValue(v);
    mEmitSignals = true;
    emit highlightsRedChanged(v);
}

void FilterWidget::setHighlightsGreen(int v)
{
    if (ui->mSpinHighlightsGreen->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinHighlightsGreen->setValue(v);
    mEmitSignals = true;
    emit highlightsGreenChanged(v);
}

void FilterWidget::setHighlightsBlue(int v)
{
    if (ui->mSpinHighlightsBlue->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinHighlightsBlue->setValue(v);
    mEmitSignals = true;
    emit highlightsBlueChanged(v);
}

void FilterWidget::setPreserveLuminosity(bool v)
{
    if (ui->mButtonPreserveLuminosity->isChecked() == v)
        return;
    mEmitSignals = false;
    ui->mButtonPreserveLuminosity->setChecked(v);
    mEmitSignals = true;
    emit preserveLuminosityChanged(v);
}

void FilterWidget::on_mSliderShadowsRed_valueChanged(int v)
{
    ui->mSpinShadowsRed->setValue(v);
    if (mEmitSignals)
        emit shadowsRedChanged(v);
}
void FilterWidget::on_mSpinShadowsRed_valueChanged(int v)
{
    ui->mSliderShadowsRed->setValue(v);
    if (mEmitSignals)
        emit shadowsRedChanged(v);
}

void FilterWidget::on_mSliderShadowsGreen_valueChanged(int v)
{
    ui->mSpinShadowsGreen->setValue(v);
    if (mEmitSignals)
        emit shadowsGreenChanged(v);
}
void FilterWidget::on_mSpinShadowsGreen_valueChanged(int v)
{
    ui->mSliderShadowsGreen->setValue(v);
    if (mEmitSignals)
        emit shadowsGreenChanged(v);
}

void FilterWidget::on_mSliderShadowsBlue_valueChanged(int v)
{
    ui->mSpinShadowsBlue->setValue(v);
    if (mEmitSignals)
        emit shadowsBlueChanged(v);
}
void FilterWidget::on_mSpinShadowsBlue_valueChanged(int v)
{
    ui->mSliderShadowsBlue->setValue(v);
    if (mEmitSignals)
        emit shadowsBlueChanged(v);
}

void FilterWidget::on_mSliderMidtonesRed_valueChanged(int v)
{
    ui->mSpinMidtonesRed->setValue(v);
    if (mEmitSignals)
        emit midtonesRedChanged(v);
}
void FilterWidget::on_mSpinMidtonesRed_valueChanged(int v)
{
    ui->mSliderMidtonesRed->setValue(v);
    if (mEmitSignals)
        emit midtonesRedChanged(v);
}

void FilterWidget::on_mSliderMidtonesGreen_valueChanged(int v)
{
    ui->mSpinMidtonesGreen->setValue(v);
    if (mEmitSignals)
        emit midtonesGreenChanged(v);
}
void FilterWidget::on_mSpinMidtonesGreen_valueChanged(int v)
{
    ui->mSliderMidtonesGreen->setValue(v);
    if (mEmitSignals)
        emit midtonesGreenChanged(v);
}

void FilterWidget::on_mSliderMidtonesBlue_valueChanged(int v)
{
    ui->mSpinMidtonesBlue->setValue(v);
    if (mEmitSignals)
        emit midtonesBlueChanged(v);
}
void FilterWidget::on_mSpinMidtonesBlue_valueChanged(int v)
{
    ui->mSliderMidtonesBlue->setValue(v);
    if (mEmitSignals)
        emit midtonesBlueChanged(v);
}

void FilterWidget::on_mSliderHighlightsRed_valueChanged(int v)
{
    ui->mSpinHighlightsRed->setValue(v);
    if (mEmitSignals)
        emit highlightsRedChanged(v);
}
void FilterWidget::on_mSpinHighlightsRed_valueChanged(int v)
{
    ui->mSliderHighlightsRed->setValue(v);
    if (mEmitSignals)
        emit highlightsRedChanged(v);
}

void FilterWidget::on_mSliderHighlightsGreen_valueChanged(int v)
{
    ui->mSpinHighlightsGreen->setValue(v);
    if (mEmitSignals)
        emit highlightsGreenChanged(v);
}
void FilterWidget::on_mSpinHighlightsGreen_valueChanged(int v)
{
    ui->mSliderHighlightsGreen->setValue(v);
    if (mEmitSignals)
        emit highlightsGreenChanged(v);
}

void FilterWidget::on_mSliderHighlightsBlue_valueChanged(int v)
{
    ui->mSpinHighlightsBlue->setValue(v);
    if (mEmitSignals)
        emit highlightsBlueChanged(v);
}
void FilterWidget::on_mSpinHighlightsBlue_valueChanged(int v)
{
    ui->mSliderHighlightsBlue->setValue(v);
    if (mEmitSignals)
        emit highlightsBlueChanged(v);
}

void FilterWidget::on_mButtonTonesShadows_toggled(bool v)
{
    if (!v)
        return;
    ui->mSliderShadowsRed->setVisible(true);
    ui->mSliderShadowsGreen->setVisible(true);
    ui->mSliderShadowsBlue->setVisible(true);
    ui->mSpinShadowsRed->setVisible(true);
    ui->mSpinShadowsGreen->setVisible(true);
    ui->mSpinShadowsBlue->setVisible(true);
    ui->mSliderMidtonesRed->setVisible(false);
    ui->mSliderMidtonesGreen->setVisible(false);
    ui->mSliderMidtonesBlue->setVisible(false);
    ui->mSpinMidtonesRed->setVisible(false);
    ui->mSpinMidtonesGreen->setVisible(false);
    ui->mSpinMidtonesBlue->setVisible(false);
    ui->mSliderHighlightsRed->setVisible(false);
    ui->mSliderHighlightsGreen->setVisible(false);
    ui->mSliderHighlightsBlue->setVisible(false);
    ui->mSpinHighlightsRed->setVisible(false);
    ui->mSpinHighlightsGreen->setVisible(false);
    ui->mSpinHighlightsBlue->setVisible(false);
}
void FilterWidget::on_mButtonTonesMidtones_toggled(bool v)
{
    if (!v)
        return;
    ui->mSliderShadowsRed->setVisible(false);
    ui->mSliderShadowsGreen->setVisible(false);
    ui->mSliderShadowsBlue->setVisible(false);
    ui->mSpinShadowsRed->setVisible(false);
    ui->mSpinShadowsGreen->setVisible(false);
    ui->mSpinShadowsBlue->setVisible(false);
    ui->mSliderMidtonesRed->setVisible(true);
    ui->mSliderMidtonesGreen->setVisible(true);
    ui->mSliderMidtonesBlue->setVisible(true);
    ui->mSpinMidtonesRed->setVisible(true);
    ui->mSpinMidtonesGreen->setVisible(true);
    ui->mSpinMidtonesBlue->setVisible(true);
    ui->mSliderHighlightsRed->setVisible(false);
    ui->mSliderHighlightsGreen->setVisible(false);
    ui->mSliderHighlightsBlue->setVisible(false);
    ui->mSpinHighlightsRed->setVisible(false);
    ui->mSpinHighlightsGreen->setVisible(false);
    ui->mSpinHighlightsBlue->setVisible(false);
}
void FilterWidget::on_mButtonTonesHighlights_toggled(bool v)
{
    if (!v)
        return;
    ui->mSliderShadowsRed->setVisible(false);
    ui->mSliderShadowsGreen->setVisible(false);
    ui->mSliderShadowsBlue->setVisible(false);
    ui->mSpinShadowsRed->setVisible(false);
    ui->mSpinShadowsGreen->setVisible(false);
    ui->mSpinShadowsBlue->setVisible(false);
    ui->mSliderMidtonesRed->setVisible(false);
    ui->mSliderMidtonesGreen->setVisible(false);
    ui->mSliderMidtonesBlue->setVisible(false);
    ui->mSpinMidtonesRed->setVisible(false);
    ui->mSpinMidtonesGreen->setVisible(false);
    ui->mSpinMidtonesBlue->setVisible(false);
    ui->mSliderHighlightsRed->setVisible(true);
    ui->mSliderHighlightsGreen->setVisible(true);
    ui->mSliderHighlightsBlue->setVisible(true);
    ui->mSpinHighlightsRed->setVisible(true);
    ui->mSpinHighlightsGreen->setVisible(true);
    ui->mSpinHighlightsBlue->setVisible(true);
}

void FilterWidget::on_mButtonPreserveLuminosity_toggled(bool v)
{
    if (mEmitSignals)
        emit preserveLuminosityChanged(v);
}
