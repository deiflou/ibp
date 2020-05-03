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
    ui->mButtonColor->setColorPickerFlags(ColorPicker::HideAlpha);
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setColor(const QColor & v)
{
    if (ui->mButtonColor->color() == v)
        return;
    mEmitSignals = false;
    ui->mButtonColor->setColor(v);
    mEmitSignals = true;
    emit colorChanged(v);
}

void FilterWidget::setPosition(Filter::Position v)
{
    Filter::Position p = ui->mButtonPositionFront->isChecked() ? Filter::Front :
                         ui->mButtonPositionBehind->isChecked() ? Filter::Behind :
                         Filter::Inside;
    if (p == v)
        return;
    mEmitSignals = false;
    if (v == Filter::Front)
        ui->mButtonPositionFront->setChecked(true);
    else if (v == Filter::Behind)
        ui->mButtonPositionBehind->setChecked(true);
    else
        ui->mButtonPositionInside->setChecked(true);
    mEmitSignals = true;
    emit positionChanged(v);
}

void FilterWidget::setColorCompositionMode(ColorCompositionMode v)
{
    if (ui->mComboColorCompositionMode->colorCompositionMode() == v)
        return;
    mEmitSignals = false;
    ui->mComboColorCompositionMode->setColorCompositionMode(v);
    mEmitSignals = true;
    emit colorCompositionModeChanged(v);
}

void FilterWidget::setOpacity(int v)
{
    if (ui->mSpinOpacity->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinOpacity->setValue(v);
    mEmitSignals = true;
    emit opacityChanged(v);
}

void FilterWidget::on_mButtonColor_colorChanged(const QColor & c)
{
    if (mEmitSignals)
        emit colorChanged(c);
}

void FilterWidget::on_mButtonPositionFront_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit positionChanged(Filter::Front);
}

void FilterWidget::on_mButtonPositionBehind_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit positionChanged(Filter::Behind);
}

void FilterWidget::on_mButtonPositionInside_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit positionChanged(Filter::Inside);
}

void FilterWidget::on_mComboColorCompositionMode_colorCompositionModeChanged(ColorCompositionMode m)
{
    if (mEmitSignals)
        emit colorCompositionModeChanged(m);
}

void FilterWidget::on_mSliderOpacity_valueChanged(int value)
{
    ui->mSpinOpacity->setValue(value);
    if (mEmitSignals)
        emit opacityChanged(value);
}

void FilterWidget::on_mSpinOpacity_valueChanged(int arg1)
{
    ui->mSliderOpacity->setValue(arg1);
    if (mEmitSignals)
        emit opacityChanged(arg1);
}
