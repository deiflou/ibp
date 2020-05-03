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

#include "outputlevelswidget.h"
#include "ui_outputlevelswidget.h"

namespace ibp {
namespace widgets {

OutputLevelsWidget::OutputLevelsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputLevelsWidget),
    mUnits(0)
{
    ui->setupUi(this);
}

OutputLevelsWidget::~OutputLevelsWidget()
{
    delete ui;
}

double OutputLevelsWidget::blackPoint()
{
    return ui->mSliderOutputLevels->blackPoint();
}
double OutputLevelsWidget::whitePoint()
{
    return ui->mSliderOutputLevels->whitePoint();
}

void OutputLevelsWidget::setBlackPoint(double v)
{
    ui->mSliderOutputLevels->setBlackPoint(v);
}
void OutputLevelsWidget::setWhitePoint(double v)
{
    ui->mSliderOutputLevels->setWhitePoint(v);
}

void OutputLevelsWidget::setValues(double b, double w)
{
    ui->mSliderOutputLevels->setValues(b, w);
}

void OutputLevelsWidget::on_mSliderOutputLevels_blackPointChanged(double v)
{
    double v2 = v;
    if (mUnits == 1)
        v2 *= 255.0;
    else if (mUnits == 2)
        v2 *= 100.0;
    ui->mSpinBlackPoint->setValue(v2);
    emit blackPointChanged(v);
}
void OutputLevelsWidget::on_mSliderOutputLevels_whitePointChanged(double v)
{
    double v2 = v;
    if (mUnits == 1)
        v2 *= 255.0;
    else if (mUnits == 2)
        v2 *= 100.0;
    ui->mSpinWhitePoint->setValue(v2);
    emit whitePointChanged(v);
}

void OutputLevelsWidget::on_mSpinBlackPoint_valueChanged(double v)
{
    if (mUnits == 1)
        v /= 255.0;
    else if (mUnits == 2)
        v /= 100.0;
    if (v == ui->mSliderOutputLevels->blackPoint()) return;
    ui->mSliderOutputLevels->setBlackPoint(v);
}
void OutputLevelsWidget::on_mSpinWhitePoint_valueChanged(double v)
{
    if (mUnits == 1)
        v /= 255.0;
    else if (mUnits == 2)
        v /= 100.0;
    if (v == ui->mSliderOutputLevels->whitePoint()) return;
    ui->mSliderOutputLevels->setWhitePoint(v);
}

int OutputLevelsWidget::units()
{
    return mUnits;
}
void OutputLevelsWidget::setUnits(int u)
{
    if (u == mUnits) return;
    mUnits = u;
    if (mUnits == 1)
    {
        ui->mSpinBlackPoint->setRange(0.0, 255.0);
        ui->mSpinBlackPoint->setDecimals(0);
        ui->mSpinBlackPoint->setSuffix("");
        ui->mSpinBlackPoint->setSingleStep(1.0);
        ui->mSpinBlackPoint->setValue(round(ui->mSliderOutputLevels->blackPoint() * 255.0));

        ui->mSpinWhitePoint->setRange(0.0, 255.0);
        ui->mSpinWhitePoint->setDecimals(0);
        ui->mSpinWhitePoint->setSuffix("");
        ui->mSpinWhitePoint->setSingleStep(1.0);
        ui->mSpinWhitePoint->setValue(round(ui->mSliderOutputLevels->whitePoint() * 255.0));
    }
    else if (mUnits == 2)
    {
        ui->mSpinBlackPoint->setRange(0.0, 100.0);
        ui->mSpinBlackPoint->setDecimals(0);
        ui->mSpinBlackPoint->setSuffix("%");
        ui->mSpinBlackPoint->setSingleStep(1.0);
        ui->mSpinBlackPoint->setValue(round(ui->mSliderOutputLevels->blackPoint() * 100.0));

        ui->mSpinWhitePoint->setRange(0.0, 100.0);
        ui->mSpinWhitePoint->setDecimals(0);
        ui->mSpinWhitePoint->setSuffix("%");
        ui->mSpinWhitePoint->setSingleStep(1.0);
        ui->mSpinWhitePoint->setValue(round(ui->mSliderOutputLevels->whitePoint() * 100.0));
    }
    else
    {
        ui->mSpinBlackPoint->setRange(0.0, 1.0);
        ui->mSpinBlackPoint->setDecimals(2);
        ui->mSpinBlackPoint->setSuffix("");
        ui->mSpinBlackPoint->setSingleStep(0.01);
        ui->mSpinBlackPoint->setValue(ui->mSliderOutputLevels->blackPoint());

        ui->mSpinWhitePoint->setRange(0.0, 1.0);
        ui->mSpinWhitePoint->setDecimals(2);
        ui->mSpinWhitePoint->setSuffix("");
        ui->mSpinWhitePoint->setSingleStep(0.01);
        ui->mSpinWhitePoint->setValue(ui->mSliderOutputLevels->whitePoint());
    }
}

}}
