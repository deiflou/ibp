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

#include "inputlevelswidget.h"
#include "ui_inputlevelswidget.h"

namespace ibp {
namespace widgets {

InputLevelsWidget::InputLevelsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputLevelsWidget),
    mUnits(0)
{
    ui->setupUi(this);
}

InputLevelsWidget::~InputLevelsWidget()
{
    delete ui;
}

double InputLevelsWidget::blackPoint()
{
    return ui->mSliderInputLevels->blackPoint();
}
double InputLevelsWidget::whitePoint()
{
    return ui->mSliderInputLevels->whitePoint();
}
double InputLevelsWidget::gammaCorrection()
{
    return ui->mSliderInputLevels->gammaCorrection();
}

void InputLevelsWidget::setBlackPoint(double v)
{
    ui->mSliderInputLevels->setBlackPoint(v);
}
void InputLevelsWidget::setWhitePoint(double v)
{
    ui->mSliderInputLevels->setWhitePoint(v);
}
void InputLevelsWidget::setGammaCorrection(double v)
{
    ui->mSliderInputLevels->setGammaCorrection(v);
}

void InputLevelsWidget::setValues(double b, double w, double g)
{
    ui->mSliderInputLevels->setValues(b, w, g);
}

void InputLevelsWidget::on_mSliderInputLevels_blackPointChanged(double v)
{
    double v2 = v;
    if (mUnits == 1)
        v2 *= 255.0;
    else if (mUnits == 2)
        v2 *= 100.0;
    ui->mSpinBlackPoint->setValue(v2);
    emit blackPointChanged(v);
}
void InputLevelsWidget::on_mSliderInputLevels_whitePointChanged(double v)
{
    double v2 = v;
    if (mUnits == 1)
        v2 *= 255.0;
    else if (mUnits == 2)
        v2 *= 100.0;
    ui->mSpinWhitePoint->setValue(v2);
    emit whitePointChanged(v);
}
void InputLevelsWidget::on_mSliderInputLevels_gammaCorrectionChanged(double v)
{
    ui->mSpinGammaCorrection->setValue(v);
    emit gammaCorrectionChanged(v);
}

void InputLevelsWidget::on_mSpinBlackPoint_valueChanged(double v)
{
    if (mUnits == 1)
        v /= 255.0;
    else if (mUnits == 2)
        v /= 100.0;
    if (v == ui->mSliderInputLevels->blackPoint()) return;
    ui->mSliderInputLevels->setBlackPoint(v);
}
void InputLevelsWidget::on_mSpinWhitePoint_valueChanged(double v)
{
    if (mUnits == 1)
        v /= 255.0;
    else if (mUnits == 2)
        v /= 100.0;
    if (v == ui->mSliderInputLevels->whitePoint()) return;
    ui->mSliderInputLevels->setWhitePoint(v);
}
void InputLevelsWidget::on_mSpinGammaCorrection_valueChanged(double v)
{
    if (v == ui->mSliderInputLevels->gammaCorrection()) return;
    ui->mSliderInputLevels->setGammaCorrection(v);
}

int InputLevelsWidget::units()
{
    return mUnits;
}
void InputLevelsWidget::setUnits(int u)
{
    if (u == mUnits) return;
    mUnits = u;
    if (mUnits == 1)
    {
        ui->mSpinBlackPoint->setRange(0.0, 255.0);
        ui->mSpinBlackPoint->setDecimals(0);
        ui->mSpinBlackPoint->setSuffix("");
        ui->mSpinBlackPoint->setSingleStep(1.0);
        ui->mSpinBlackPoint->setValue(round(ui->mSliderInputLevels->blackPoint() * 255.0));

        ui->mSpinWhitePoint->setRange(0.0, 255.0);
        ui->mSpinWhitePoint->setDecimals(0);
        ui->mSpinWhitePoint->setSuffix("");
        ui->mSpinWhitePoint->setSingleStep(1.0);
        ui->mSpinWhitePoint->setValue(round(ui->mSliderInputLevels->whitePoint() * 255.0));
    }
    else if (mUnits == 2)
    {
        ui->mSpinBlackPoint->setRange(0.0, 100.0);
        ui->mSpinBlackPoint->setDecimals(0);
        ui->mSpinBlackPoint->setSuffix("%");
        ui->mSpinBlackPoint->setSingleStep(1.0);
        ui->mSpinBlackPoint->setValue(round(ui->mSliderInputLevels->blackPoint() * 100.0));

        ui->mSpinWhitePoint->setRange(0.0, 100.0);
        ui->mSpinWhitePoint->setDecimals(0);
        ui->mSpinWhitePoint->setSuffix("%");
        ui->mSpinWhitePoint->setSingleStep(1.0);
        ui->mSpinWhitePoint->setValue(round(ui->mSliderInputLevels->whitePoint() * 100.0));
    }
    else
    {
        ui->mSpinBlackPoint->setRange(0.0, 1.0);
        ui->mSpinBlackPoint->setDecimals(2);
        ui->mSpinBlackPoint->setSuffix("");
        ui->mSpinBlackPoint->setSingleStep(0.01);
        ui->mSpinBlackPoint->setValue(ui->mSliderInputLevels->blackPoint());

        ui->mSpinWhitePoint->setRange(0.0, 1.0);
        ui->mSpinWhitePoint->setDecimals(2);
        ui->mSpinWhitePoint->setSuffix("");
        ui->mSpinWhitePoint->setSingleStep(0.01);
        ui->mSpinWhitePoint->setValue(ui->mSliderInputLevels->whitePoint());
    }
}

}}
