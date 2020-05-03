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

void FilterWidget::setRadius(double v)
{
    if (ui->mSpinRadius->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinRadius->setValue(v);
    mEmitSignals = true;
    emit radiusChanged(v);
}

void FilterWidget::setAmount(int v)
{
    if (ui->mSpinAmount->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinAmount->setValue(v);
    mEmitSignals = true;
    emit amountChanged(v);
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

void FilterWidget::on_mSliderRadius_valueChanged(int v)
{
    ui->mSpinRadius->setValue(v / 100.0);
    if (mEmitSignals)
        emit radiusChanged(v / 100.0);
}

void FilterWidget::on_mSpinRadius_valueChanged(double v)
{
    ui->mSliderRadius->setValue((int)round(v * 100.0));
    if (mEmitSignals)
        emit radiusChanged(v);
}

void FilterWidget::on_mSliderAmount_valueChanged(int v)
{
    ui->mSpinAmount->setValue(v);
    if (mEmitSignals)
        emit amountChanged(v);
}

void FilterWidget::on_mSpinAmount_valueChanged(int v)
{
    ui->mSliderAmount->setValue(v);
    if (mEmitSignals)
        emit amountChanged(v);
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
