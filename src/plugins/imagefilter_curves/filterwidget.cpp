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

    ui->mWidgetCurves->setInterpolationMode(ibp::widgets::Curves::NearestNeighbor);

    if (mEmitSignals)
        emit interpolationModeChanged(Filter::Flat);
}
void FilterWidget::on_mButtonInterpolationModeLinear_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetCurves->setInterpolationMode(ibp::widgets::Curves::Linear);

    if (mEmitSignals)
        emit interpolationModeChanged(Filter::Linear);
}
void FilterWidget::on_mButtonInterpolationModeSmooth_toggled(bool v)
{
    if (!v)
        return;

    ui->mWidgetCurves->setInterpolationMode(ibp::widgets::Curves::Cubic);

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
