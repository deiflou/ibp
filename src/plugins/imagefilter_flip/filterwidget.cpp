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

void FilterWidget::setDirection(Filter::Direction d)
{
    if ((d == Filter::Horizontal && ui->mButtonDirectionHorizontal->isChecked()) ||
        (d == Filter::Vertical && ui->mButtonDirectionVertical->isChecked()) ||
        (d == Filter::Both && ui->mButtonDirectionBoth->isChecked()))
        return;

    mEmitSignals = false;
    if (d == Filter::Horizontal)
        ui->mButtonDirectionHorizontal->setChecked(true);
    else if (d == Filter::Vertical)
        ui->mButtonDirectionVertical->setChecked(true);
    else
        ui->mButtonDirectionBoth->setChecked(true);
    mEmitSignals = true;

    emit directionChanged(d);
}

void FilterWidget::on_mButtonDirectionHorizontal_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit directionChanged(Filter::Horizontal);
}

void FilterWidget::on_mButtonDirectionVertical_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit directionChanged(Filter::Vertical);
}

void FilterWidget::on_mButtonDirectionBoth_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit directionChanged(Filter::Both);
}
