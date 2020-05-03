//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <math.h>

#include "filterwidget.h"
#include "ui_filterwidget.h"
#include "../widgets/colorpicker.h"

using namespace ibp::widgets;

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
