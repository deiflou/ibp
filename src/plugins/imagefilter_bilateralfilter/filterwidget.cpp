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

void FilterWidget::setRadius(double s)
{
    if (ui->mSpinRadius->value() == s)
        return;
    mEmitSignals = false;
    ui->mSpinRadius->setValue(s);
    mEmitSignals = true;
    emit radiusChanged(s);
}

void FilterWidget::setEdgePreservation(int v)
{
    if (ui->mSpinEdgePreservation->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinEdgePreservation->setValue(v);
    mEmitSignals = true;
    emit edgePreservationChanged(v);
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

void FilterWidget::on_mSliderEdgePreservation_valueChanged(int value)
{
    ui->mSpinEdgePreservation->setValue(value);
    if (mEmitSignals)
        emit edgePreservationChanged(value);
}

void FilterWidget::on_mSpinEdgePreservation_valueChanged(int arg1)
{
    ui->mSliderEdgePreservation->setValue(arg1);
    if (mEmitSignals)
        emit edgePreservationChanged(arg1);
}
