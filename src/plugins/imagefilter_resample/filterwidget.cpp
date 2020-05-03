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

    ui->mComboWidthMode->addItems(QStringList() << tr("Percent") << tr("Pixels") << tr("Keep Aspect Ratio"));
    ui->mComboHeightMode->addItems(QStringList() << tr("Percent") << tr("Pixels") << tr("Keep Aspect Ratio"));
    ui->mComboResamplingMode->addItems(QStringList() << tr("Nearest Neighbor") << tr("Bilinear") <<
                                       tr("Bicubic") << tr("Lanczos"));
    ui->mComboResamplingMode->setCurrentIndex(2);
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setWidth(int w)
{
    if (w == ui->mSpinWidth->value())
        return;

    mEmitSignals = false;
    ui->mSpinWidth->setValue(w);
    mEmitSignals = true;

    emit widthChanged(w);
}

void FilterWidget::setHeight(int h)
{
    if (h == ui->mSpinHeight->value())
        return;

    mEmitSignals = false;
    ui->mSpinHeight->setValue(h);
    mEmitSignals = true;

    emit heightChanged(h);
}

void FilterWidget::setWidthMode(Filter::SizeMode wm)
{
    if (wm == ui->mComboWidthMode->currentIndex())
        return;

    mEmitSignals = false;
    ui->mComboWidthMode->setCurrentIndex(wm);
    mEmitSignals = true;

    emit widthModeChanged(wm);
}

void FilterWidget::setHeightMode(Filter::SizeMode hm)
{
    if (hm == ui->mComboHeightMode->currentIndex())
        return;

    mEmitSignals = false;
    ui->mComboHeightMode->setCurrentIndex(hm);
    mEmitSignals = true;

    emit heightModeChanged(hm);
}

void FilterWidget::setResamplingMode(Filter::ResamplingMode rm)
{
    if (rm == ui->mComboResamplingMode->currentIndex())
        return;

    mEmitSignals = false;
    ui->mComboResamplingMode->setCurrentIndex(rm);
    mEmitSignals = true;

    emit resamplingModeChanged(rm);
}

void FilterWidget::on_mSpinWidth_valueChanged(int arg1)
{
    if (mEmitSignals)
        emit widthChanged(arg1);
}

void FilterWidget::on_mSpinHeight_valueChanged(int arg1)
{
    if (mEmitSignals)
        emit heightChanged(arg1);
}

void FilterWidget::on_mComboWidthMode_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->mSpinWidth->setSuffix("%");
        ui->mSpinWidth->setMaximum(1000);
        ui->mSpinWidth->setVisible(true);
    }
    else if (index == 1)
    {
        ui->mSpinWidth->setSuffix("px");
        ui->mSpinWidth->setMaximum(100000);
        ui->mSpinWidth->setVisible(true);
    }
    else
    {
        if (ui->mComboHeightMode->currentIndex() == 2)
            ui->mComboHeightMode->setCurrentIndex(0);
        ui->mSpinWidth->setVisible(false);
    }
    if (mEmitSignals)
        emit widthModeChanged((Filter::SizeMode)index);
}

void FilterWidget::on_mComboHeightMode_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->mSpinHeight->setSuffix("%");
        ui->mSpinHeight->setMaximum(1000);
        ui->mSpinHeight->setVisible(true);
    }
    else if (index == 1)
    {
        ui->mSpinHeight->setSuffix("px");
        ui->mSpinHeight->setMaximum(100000);
        ui->mSpinHeight->setVisible(true);
    }
    else
    {
        if (ui->mComboWidthMode->currentIndex() == 2)
            ui->mComboWidthMode->setCurrentIndex(0);
        ui->mSpinHeight->setVisible(false);
    }
    if (mEmitSignals)
        emit heightModeChanged((Filter::SizeMode)index);
}

void FilterWidget::on_mComboResamplingMode_currentIndexChanged(int index)
{
    if (mEmitSignals)
        emit resamplingModeChanged((Filter::ResamplingMode)index);
}
