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

void FilterWidget::setAmount(double v)
{
    if (qFuzzyCompare(ui->mSpinAmount->value(), v))
        return;
    mEmitSignals = false;
    ui->mSpinAmount->setValue(v);
    mEmitSignals = true;
    emit amountChanged(v);
}

void FilterWidget::setDistribution(Filter::Distribution v)
{
    if ((v == Filter::Uniform && ui->mButtonDistributionUniform->isChecked()) ||
        (v == Filter::Gaussian && ui->mButtonDistributionGaussian->isChecked()))
        return;
    mEmitSignals = false;
    if (v == Filter::Uniform)
        ui->mButtonDistributionUniform->setChecked(true);
    else
        ui->mButtonDistributionGaussian->setChecked(true);
    mEmitSignals = true;
    emit distributionChanged(v);
}

void FilterWidget::setColorMode(Filter::ColorMode v)
{
    if ((v == Filter::Monochromatic && ui->mButtonColorModeMonochromatic->isChecked()) ||
        (v == Filter::Color && ui->mButtonColorModeColor->isChecked()))
        return;
    mEmitSignals = false;
    if (v == Filter::Monochromatic)
        ui->mButtonColorModeMonochromatic->setChecked(true);
    else
        ui->mButtonColorModeColor->setChecked(true);
    mEmitSignals = true;
    emit colorModeChanged(v);
}

void FilterWidget::on_mSliderAmount_valueChanged(int v)
{
    ui->mSpinAmount->setValue(v / 100.0);
    if (mEmitSignals)
        emit amountChanged(v / 100.0);
}

void FilterWidget::on_mSpinAmount_valueChanged(double v)
{
    ui->mSliderAmount->setValue((int)round(v * 100.0));
    if (mEmitSignals)
        emit amountChanged(v);
}

void FilterWidget::on_mButtonDistributionUniform_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit distributionChanged(Filter::Uniform);
}

void FilterWidget::on_mButtonDistributionGaussian_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit distributionChanged(Filter::Gaussian);
}

void FilterWidget::on_mButtonColorModeMonochromatic_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit colorModeChanged(Filter::Monochromatic);
}

void FilterWidget::on_mButtonColorModeColor_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit colorModeChanged(Filter::Color);
}
