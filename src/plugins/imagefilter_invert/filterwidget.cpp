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
#include <QDebug>

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

void FilterWidget::setRedChannel(bool v)
{
    if (v == ui->mButtonRedChannel->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonRedChannel->setChecked(v);
    mEmitSignals = true;
    emit redChannelToggled(v);
}

void FilterWidget::setGreenChannel(bool v)
{
    if (v == ui->mButtonGreenChannel->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonGreenChannel->setChecked(v);
    mEmitSignals = true;
    emit greenChannelToggled(v);
}

void FilterWidget::setBlueChannel(bool v)
{
    if (v == ui->mButtonBlueChannel->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonBlueChannel->setChecked(v);
    mEmitSignals = true;
    emit blueChannelToggled(v);
}

void FilterWidget::setAlphaChannel(bool v)
{
    if (v == ui->mButtonAlphaChannel->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonAlphaChannel->setChecked(v);
    mEmitSignals = true;
    emit alphaChannelToggled(v);
}

void FilterWidget::on_mButtonRedChannel_toggled(bool c)
{
    if (mEmitSignals)
        emit redChannelToggled(c);
}

void FilterWidget::on_mButtonGreenChannel_toggled(bool c)
{
    if (mEmitSignals)
        emit greenChannelToggled(c);
}

void FilterWidget::on_mButtonBlueChannel_toggled(bool c)
{
    if (mEmitSignals)
        emit blueChannelToggled(c);
}

void FilterWidget::on_mButtonAlphaChannel_toggled(bool c)
{
    if (mEmitSignals)
        emit alphaChannelToggled(c);
}
