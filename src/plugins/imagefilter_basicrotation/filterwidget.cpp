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

void FilterWidget::setAngle(Filter::Angle a)
{
    if ((a == Filter::_90Clockwise && ui->mButtonAngle90Clockwise->isChecked()) ||
        (a == Filter::_90CounterClockwise && ui->mButtonAngle90CounterClockwise->isChecked()) ||
        (a == Filter::_180 && ui->mButtonAngle180->isChecked()))
        return;

    mEmitSignals = false;
    if (a == Filter::_90Clockwise)
        ui->mButtonAngle90Clockwise->setChecked(true);
    else if (a == Filter::_90CounterClockwise)
        ui->mButtonAngle90CounterClockwise->setChecked(true);
    else
        ui->mButtonAngle180->setChecked(true);
    mEmitSignals = true;

    emit angleChanged(a);
}

void FilterWidget::on_mButtonAngle90Clockwise_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit angleChanged(Filter::_90Clockwise);
}

void FilterWidget::on_mButtonAngle90CounterClockwise_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit angleChanged(Filter::_90CounterClockwise);
}

void FilterWidget::on_mButtonAngle180_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit angleChanged(Filter::_180);
}
