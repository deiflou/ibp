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

    ui->mWidgetInputLevels->setUnits(1);
    ui->mWidgetOutputLevels->setUnits(1);
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setWorkingChannel(Filter::WorkingChannel s)
{
    if ((s == Filter::Luma && ui->mButtonWorkingChannelLuma->isChecked()) ||
        (s == Filter::Red && ui->mButtonWorkingChannelRed->isChecked()) ||
        (s == Filter::Green && ui->mButtonWorkingChannelGreen->isChecked()) ||
        (s == Filter::Blue && ui->mButtonWorkingChannelBlue->isChecked()) ||
        (s == Filter::Alpha && ui->mButtonWorkingChannelAlpha->isChecked()))
        return;
    mEmitSignals = false;
    if (s == Filter::Luma)
        ui->mButtonWorkingChannelLuma->setChecked(true);
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

void FilterWidget::setInputBlackPoint(double v)
{
    if (v == ui->mWidgetInputLevels->blackPoint())
        return;
    mEmitSignals = false;
    ui->mWidgetInputLevels->setBlackPoint(v);
    mEmitSignals = true;
    emit inputBlackPointChanged(v);
}

void FilterWidget::setInputGamma(double v)
{
    if (v == ui->mWidgetInputLevels->gammaCorrection())
        return;
    mEmitSignals = false;
    ui->mWidgetInputLevels->setGammaCorrection(v);
    mEmitSignals = true;
    emit inputGammaChanged(v);
}

void FilterWidget::setInputWhitePoint(double v)
{
    if (v == ui->mWidgetInputLevels->whitePoint())
        return;
    mEmitSignals = false;
    ui->mWidgetInputLevels->setWhitePoint(v);
    mEmitSignals = true;
    emit inputWhitePointChanged(v);
}

void FilterWidget::setOutputBlackPoint(double v)
{
    if (v == ui->mWidgetOutputLevels->blackPoint())
        return;
    mEmitSignals = false;
    ui->mWidgetOutputLevels->setBlackPoint(v);
    mEmitSignals = true;
    emit outputBlackPointChanged(v);
}

void FilterWidget::setOutputWhitePoint(double v)
{
    if (v == ui->mWidgetOutputLevels->whitePoint())
        return;
    mEmitSignals = false;
    ui->mWidgetOutputLevels->setWhitePoint(v);
    mEmitSignals = true;
    emit outputWhitePointChanged(v);
}

void FilterWidget::on_mButtonWorkingChannelLuma_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit workingChannelChanged(Filter::Luma);
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

void FilterWidget::on_mWidgetInputLevels_blackPointChanged(double v)
{
    if (mEmitSignals)
        emit inputBlackPointChanged(v);
}

void FilterWidget::on_mWidgetInputLevels_whitePointChanged(double v)
{
    if (mEmitSignals)
        emit inputWhitePointChanged(v);
}

void FilterWidget::on_mWidgetInputLevels_gammaCorrectionChanged(double v)
{
    if (mEmitSignals)
        emit inputGammaChanged(v);
}

void FilterWidget::on_mWidgetOutputLevels_blackPointChanged(double v)
{
    if (mEmitSignals)
        emit outputBlackPointChanged(v);
}

void FilterWidget::on_mWidgetOutputLevels_whitePointChanged(double v)
{
    if (mEmitSignals)
        emit outputWhitePointChanged(v);
}

