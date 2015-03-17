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

#include "filterwidget.h"
#include "ui_filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true),
    mCurvesPaintDelegate(0)
{
    ui->setupUi(this);
    mCurvesPaintDelegate = new LevelsCurvesPaintDelegate(this);
    ui->mWidgetCurves->setPaintDelegate(mCurvesPaintDelegate);
    ui->mWidgetContainerSpins->setEnabled(false);
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

void FilterWidget::setKnots(const Interpolator1DKnots & k)
{
    if (ui->mWidgetCurves->knots() == k)
        return;

    mEmitSignals = false;
    ui->mWidgetCurves->setKnots(k);
    mEmitSignals = true;

    emit knotsChanged(k);
}

void FilterWidget::setInterpolationMode(Filter::InterpolationMode im)
{
    if ((im == Filter::Flat && ui->mButtonInterpolationModeFlat->isChecked()) ||
        (im == Filter::Linear && ui->mButtonInterpolationModeLinear->isChecked()) ||
        (im == Filter::Smooth && ui->mButtonInterpolationModeSmooth->isChecked()))
        return;

    mEmitSignals = false;
    if (im == Filter::Flat)
        ui->mButtonInterpolationModeFlat->setChecked(true);
    else if (im == Filter::Linear)
        ui->mButtonInterpolationModeLinear->setChecked(true);
    else if (im == Filter::Smooth)
        ui->mButtonInterpolationModeSmooth->setChecked(true);
    mEmitSignals = true;

    emit interpolationModeChanged(im);
}

void FilterWidget::on_mButtonWorkingChannelLuma_toggled(bool v)
{
    if (!v)
        return;

    if (mEmitSignals)
        emit workingChannelChanged(Filter::Luma);
}
void FilterWidget::on_mButtonWorkingChannelRed_toggled(bool v)
{
    if (!v)
        return;

    if (mEmitSignals)
        emit workingChannelChanged(Filter::Red);
}
void FilterWidget::on_mButtonWorkingChannelGreen_toggled(bool v)
{
    if (!v)
        return;

    if (mEmitSignals)
        emit workingChannelChanged(Filter::Green);
}
void FilterWidget::on_mButtonWorkingChannelBlue_toggled(bool v)
{
    if (!v)
        return;

    if (mEmitSignals)
        emit workingChannelChanged(Filter::Blue);
}
void FilterWidget::on_mButtonWorkingChannelAlpha_toggled(bool v)
{
    if (!v)
        return;

    if (mEmitSignals)
        emit workingChannelChanged(Filter::Alpha);
}

void FilterWidget::on_mButtonInterpolationModeFlat_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetCurves->setInterpolationMode(anitools::widgets::Curves::NearestNeighbor);

    if (mEmitSignals)
        emit interpolationModeChanged(Filter::Flat);
}
void FilterWidget::on_mButtonInterpolationModeLinear_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetCurves->setInterpolationMode(anitools::widgets::Curves::Linear);

    if (mEmitSignals)
        emit interpolationModeChanged(Filter::Linear);
}
void FilterWidget::on_mButtonInterpolationModeSmooth_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetCurves->setInterpolationMode(anitools::widgets::Curves::Cubic);

    if (mEmitSignals)
        emit interpolationModeChanged(Filter::Smooth);
}

void FilterWidget::on_mWidgetCurves_knotsChanged(const Interpolator1DKnots & k)
{
    if (ui->mWidgetCurves->selectedKnotIndex() == -1)
        ui->mWidgetContainerSpins->setEnabled(false);
    else
    {
        Interpolator1DKnot kn = k.at(ui->mWidgetCurves->selectedKnotIndex());
        ui->mSpinInputValue->setValue(round(kn.x() * 255.0));
        ui->mSpinOutputValue->setValue(round(kn.y() * 255.0));
        ui->mWidgetContainerSpins->setEnabled(true);
    }

    if (mEmitSignals)
        emit knotsChanged(k);
}

void FilterWidget::on_mWidgetCurves_selectedKnotChanged(int i)
{
    Q_UNUSED(i)

    if (ui->mWidgetCurves->selectedKnotIndex() == -1)
        ui->mWidgetContainerSpins->setEnabled(false);
    else
    {
        Interpolator1DKnot k = ui->mWidgetCurves->selectedKnot();
        ui->mSpinInputValue->setValue(round(k.x() * 255.0));
        ui->mSpinOutputValue->setValue(round(k.y() * 255.0));
        ui->mWidgetContainerSpins->setEnabled(true);
    }
}

void FilterWidget::on_mSpinInputValue_valueChanged(double v)
{
    if (ui->mWidgetCurves->selectedKnotIndex() == -1)
        return;

    Interpolator1DKnot k = ui->mWidgetCurves->selectedKnot();
    v = v / 255.0;
    if (v == k.x()) return;
    ui->mWidgetCurves->setSelectedKnot(v, k.y());

    if (mEmitSignals)
        emit knotsChanged(ui->mWidgetCurves->knots());
}

void FilterWidget::on_mSpinOutputValue_valueChanged(double v)
{
    if (ui->mWidgetCurves->selectedKnotIndex() == -1)
        return;

    Interpolator1DKnot k = ui->mWidgetCurves->selectedKnot();
    v = v / 255.0;
    if (v == k.y()) return;
    ui->mWidgetCurves->setSelectedKnot(k.x(), v);

    if (mEmitSignals)
        emit knotsChanged(ui->mWidgetCurves->knots());
}
