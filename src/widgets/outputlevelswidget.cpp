/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
**
** This file is part of Anitools
**
** Anitools is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <math.h>

#include "outputlevelswidget.h"
#include "ui_outputlevelswidget.h"

namespace anitools {
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
