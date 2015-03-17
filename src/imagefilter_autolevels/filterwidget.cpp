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
#include "../widgets/colorpicker.h"

using namespace anitools::widgets;

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new ::Ui::FilterWidget),
    mEmitSignals(true)
{
    ui->setupUi(this);
    ui->mButtonTargetColorsShadows->setColorPickerFlags(ColorPicker::HideAlpha);
    ui->mButtonTargetColorsMidtones->setColorPickerFlags(ColorPicker::HideAlpha);
    ui->mButtonTargetColorsHighlights->setColorPickerFlags(ColorPicker::HideAlpha);
    ui->mLabelTargetColorsMidtones->hide();
    ui->mButtonTargetColorsMidtones->hide();
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setEnhanceChannelsSeparately(bool v)
{
    if (ui->mButtonHistogramOptionsEnhanceChannelsSeparately->isChecked() == v)
        return;
    mEmitSignals = false;
    ui->mButtonHistogramOptionsEnhanceChannelsSeparately->setChecked(v);
    mEmitSignals = true;
    emit enhanceChannelsSeparatelyChanged(v);
}

void FilterWidget::setAdjustMidtones(bool v)
{
    if (ui->mButtonHistogramOptionsAdjustMidtones->isChecked() == v)
        return;
    mEmitSignals = false;
    ui->mButtonHistogramOptionsAdjustMidtones->setChecked(v);
    mEmitSignals = true;
    emit adjustMidtonesChanged(v);
}

void FilterWidget::setTargetColorShadows(const QColor & v)
{
    if (ui->mButtonTargetColorsShadows->color() == v)
        return;
    mEmitSignals = false;
    ui->mButtonTargetColorsShadows->setColor(v);
    mEmitSignals = true;
    emit targetColorShadowsChanged(v);
}

void FilterWidget::setTargetColorMidtones(const QColor & v)
{
    if (ui->mButtonTargetColorsMidtones->color() == v)
        return;
    mEmitSignals = false;
    ui->mButtonTargetColorsMidtones->setColor(v);
    mEmitSignals = true;
    emit targetColorMidtonesChanged(v);
}

void FilterWidget::setTargetColorHighlights(const QColor & v)
{
    if (ui->mButtonTargetColorsHighlights->color() == v)
        return;
    mEmitSignals = false;
    ui->mButtonTargetColorsHighlights->setColor(v);
    mEmitSignals = true;
    emit targetColorHighlightsChanged(v);
}

void FilterWidget::setClippingShadows(double v)
{
    if (ui->mSpinClippingShadows->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinClippingShadows->setValue(v);
    mEmitSignals = true;
    emit clippingShadowsChanged(v);
}

void FilterWidget::setClippingHighlights(double v)
{
    if (ui->mSpinClippingHighlights->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinClippingHighlights->setValue(v);
    mEmitSignals = true;
    emit clippingHighlightsChanged(v);
}

void FilterWidget::on_mButtonHistogramOptionsEnhanceChannelsSeparately_toggled(bool v)
{
    if (mEmitSignals)
        emit enhanceChannelsSeparatelyChanged(v);
}

void FilterWidget::on_mButtonHistogramOptionsAdjustMidtones_toggled(bool v)
{
    ui->mLabelTargetColorsMidtones->setVisible(v);
    ui->mButtonTargetColorsMidtones->setVisible(v);
    if (mEmitSignals)
        emit adjustMidtonesChanged(v);
}

void FilterWidget::on_mButtonTargetColorsShadows_colorChanged(const QColor & v)
{
    if (mEmitSignals)
        emit targetColorShadowsChanged(v);
}

void FilterWidget::on_mButtonTargetColorsMidtones_colorChanged(const QColor & v)
{
    if (mEmitSignals)
        emit targetColorMidtonesChanged(v);
}

void FilterWidget::on_mButtonTargetColorsHighlights_colorChanged(const QColor & v)
{
    if (mEmitSignals)
        emit targetColorHighlightsChanged(v);
}

void FilterWidget::on_mSpinClippingShadows_valueChanged(double v)
{
    if (mEmitSignals)
        emit clippingShadowsChanged(v);
}

void FilterWidget::on_mSpinClippingHighlights_valueChanged(double v)
{
    if (mEmitSignals)
        emit clippingHighlightsChanged(v);
}
