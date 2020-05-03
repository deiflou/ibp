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

void FilterWidget::setWorkingChannel(Filter::WorkingChannel s)
{
    if ((s == Filter::RGB && ui->mButtonWorkingChannelRGB->isChecked()) ||
        (s == Filter::Red && ui->mButtonWorkingChannelRed->isChecked()) ||
        (s == Filter::Green && ui->mButtonWorkingChannelGreen->isChecked()) ||
        (s == Filter::Blue && ui->mButtonWorkingChannelBlue->isChecked()) ||
        (s == Filter::Alpha && ui->mButtonWorkingChannelAlpha->isChecked()))
        return;
    mEmitSignals = false;
    if (s == Filter::RGB)
        ui->mButtonWorkingChannelRGB->setChecked(true);
    else if (s == Filter::Red)
        ui->mButtonWorkingChannelRed->setChecked(true);
    else if (s == Filter::Green)
        ui->mButtonWorkingChannelGreen->setChecked(true);
    else if (s == Filter::Blue)
        ui->mButtonWorkingChannelBlue->setChecked(true);
    else if (s == Filter::Alpha)
        ui->mButtonWorkingChannelAlpha->setChecked(true);
    mEmitSignals = true;
    emit workingChannelChanged(s);
}

void FilterWidget::setBrightness(int v)
{
    if (v == ui->mSpinBrightness->value())
        return;
    mEmitSignals = false;
    ui->mSpinBrightness->setValue(v);
    mEmitSignals = true;
    emit brightnessChanged(v);
}

void FilterWidget::setContrast(int v)
{
    if (v == ui->mSpinContrast->value())
        return;
    mEmitSignals = false;
    ui->mSpinContrast->setValue(v);
    mEmitSignals = true;
    emit contrastChanged(v);
}

void FilterWidget::setUseSoftMode(bool v)
{
    if (v == ui->mButtonOptionsUseSoftMode->isChecked())
        return;
    mEmitSignals = false;
    ui->mButtonOptionsUseSoftMode->setChecked(v);
    mEmitSignals = true;
    emit useSoftModeChanged(v);
}

void FilterWidget::on_mButtonWorkingChannelRGB_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::RGB);
}

void FilterWidget::on_mButtonWorkingChannelRed_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::Red);
}

void FilterWidget::on_mButtonWorkingChannelGreen_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::Green);
}

void FilterWidget::on_mButtonWorkingChannelBlue_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::Blue);
}

void FilterWidget::on_mButtonWorkingChannelAlpha_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::Alpha);
}

void FilterWidget::on_mSliderBrightness_valueChanged(int v)
{
    ui->mSpinBrightness->setValue(v);
}

void FilterWidget::on_mSpinBrightness_valueChanged(int v)
{
    ui->mSliderBrightness->setValue(v);
    if (mEmitSignals)
        emit brightnessChanged(v);
}

void FilterWidget::on_mSliderContrast_valueChanged(int v)
{
    ui->mSpinContrast->setValue(v);
}

void FilterWidget::on_mSpinContrast_valueChanged(int v)
{
    ui->mSliderContrast->setValue(v);
    if (mEmitSignals)
        emit contrastChanged(v);
}

void FilterWidget::on_mButtonOptionsUseSoftMode_toggled(bool c)
{
    if (mEmitSignals)
        emit useSoftModeChanged(c);
}
