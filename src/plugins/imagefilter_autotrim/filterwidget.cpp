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
    connect(ui->mSpinMarginsTop, SIGNAL(valueChanged(int)), this, SLOT(On_mSpinMargins_valueChanged(int)));
    connect(ui->mSpinMarginsLeft, SIGNAL(valueChanged(int)), this, SLOT(On_mSpinMargins_valueChanged(int)));
    connect(ui->mSpinMarginsBottom, SIGNAL(valueChanged(int)), this, SLOT(On_mSpinMargins_valueChanged(int)));
    connect(ui->mSpinMarginsRight, SIGNAL(valueChanged(int)), this, SLOT(On_mSpinMargins_valueChanged(int)));
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setReference(Filter::Reference v)
{
    if ((v == Filter::AlphaChannel && ui->mButtonReferenceAlphaChannel->isChecked()) ||
        (v == Filter::Luma && ui->mButtonReferenceLuma->isChecked()))
        return;
    mEmitSignals = false;
    if (v == Filter::AlphaChannel)
        ui->mButtonReferenceAlphaChannel->setChecked(true);
    else
        ui->mButtonReferenceLuma->setChecked(true);
    mEmitSignals = true;
    emit referenceChanged(v);
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

void FilterWidget::setMargins(const QMargins &v)
{
    if (ui->mSpinMarginsTop->value() == v.top() && ui->mSpinMarginsLeft->value() == v.left() &&
        ui->mSpinMarginsBottom->value() == v.bottom() && ui->mSpinMarginsRight->value() == v.left())
        return;
    mEmitSignals = false;
    ui->mSpinMarginsTop->setValue(v.top());
    ui->mSpinMarginsLeft->setValue(v.left());
    ui->mSpinMarginsBottom->setValue(v.bottom());
    ui->mSpinMarginsRight->setValue(v.right());
    mEmitSignals = true;
    emit marginsChanged(v);
}

void FilterWidget::on_mButtonReferenceAlphaChannel_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit referenceChanged(Filter::AlphaChannel);
}

void FilterWidget::on_mButtonReferenceLuma_toggled(bool v)
{
    if (!v)
        return;
    if (mEmitSignals)
        emit referenceChanged(Filter::Luma);
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

void FilterWidget::On_mSpinMargins_valueChanged(int v)
{
    Q_UNUSED(v)
    if (mEmitSignals)
        emit marginsChanged(QMargins(ui->mSpinMarginsLeft->value(), ui->mSpinMarginsTop->value(),
                                     ui->mSpinMarginsRight->value(), ui->mSpinMarginsBottom->value()));
}
