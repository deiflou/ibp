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
    mLumaKeyingCurvesPaintDelegate(0),
    mZoomFactorIndex(0)
{
    ui->setupUi(this);
    mLumaKeyingCurvesPaintDelegate = new LumaKeyingCurvesPaintDelegate(this);
    ui->mWidgetCurves->setPaintDelegate(mLumaKeyingCurvesPaintDelegate);
    ui->mSpinLumaValue->setEnabled(false);
    ui->mSpinOpacityValue->setEnabled(false);
    mZoomFactors << 1. << 1.2 << 1.4 << 1.6 << 1.8 << 2.;
}

FilterWidget::~FilterWidget()
{
    delete ui;
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

void FilterWidget::setInverted(bool i)
{
    if (i == ui->mButtonInverted->isChecked())
        return;

    mEmitSignals = false;
    ui->mButtonInverted->setChecked(i);
    mEmitSignals = true;

    emit invertedChanged(i);
}

void FilterWidget::setOutputMode(Filter::OutputMode om)
{
    if ((om == Filter::KeyedImage && ui->mButtonOutputModeKeyedImage->isChecked()) ||
        (om == Filter::Matte && ui->mButtonOutputModeMatte->isChecked()))
        return;

    mEmitSignals = false;
    if (om == Filter::KeyedImage)
        ui->mButtonOutputModeKeyedImage->setChecked(true);
    else
        ui->mButtonOutputModeMatte->setChecked(true);
    mEmitSignals = true;

    emit outputModeChanged(om);
}

void FilterWidget::setPreblurRadius(double pbr)
{
    if (ui->mSpinPreblurRadius->value() == pbr)
        return;
    mEmitSignals = false;
    ui->mSpinPreblurRadius->setValue(pbr);
    mEmitSignals = true;
    emit preblurRadiusChanged(pbr);
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
    {
        ui->mSpinLumaValue->setEnabled(false);
        ui->mSpinOpacityValue->setEnabled(false);
    }
    else
    {
        Interpolator1DKnot kn = k.at(ui->mWidgetCurves->selectedKnotIndex());
        ui->mSpinLumaValue->setValue(round(kn.x() * 255.0));
        ui->mSpinOpacityValue->setValue(round(kn.y() * 255.0));
        ui->mSpinLumaValue->setEnabled(true);
        ui->mSpinOpacityValue->setEnabled(true);
    }

    if (mEmitSignals)
        emit knotsChanged(k);
}

void FilterWidget::on_mWidgetCurves_selectedKnotChanged(int i)
{
    Q_UNUSED(i)

    if (i == -1)
    {
        ui->mSpinLumaValue->setEnabled(false);
        ui->mSpinOpacityValue->setEnabled(false);
    }
    else
    {
        Interpolator1DKnot kn = ui->mWidgetCurves->selectedKnot();
        ui->mSpinLumaValue->setValue(round(kn.x() * 255.0));
        ui->mSpinOpacityValue->setValue(round(kn.y() * 255.0));
        ui->mSpinLumaValue->setEnabled(true);
        ui->mSpinOpacityValue->setEnabled(true);
    }
}

void FilterWidget::on_mSpinLumaValue_valueChanged(double v)
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

void FilterWidget::on_mSpinOpacityValue_valueChanged(double v)
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

void FilterWidget::on_mButtonInverted_toggled(bool v)
{
    mLumaKeyingCurvesPaintDelegate->setInverted(v);

    if (mEmitSignals)
        emit invertedChanged(v);
}

void FilterWidget::on_mButtonZoomIn_clicked()
{
    if (mZoomFactorIndex == mZoomFactors.size() - 1)
        return;
    mZoomFactorIndex++;
    ui->mWidgetCurves->setZoomFactor(mZoomFactors[mZoomFactorIndex]);
}

void FilterWidget::on_mButtonZoomOut_clicked()
{
    if (mZoomFactorIndex == 0)
        return;
    mZoomFactorIndex--;
    ui->mWidgetCurves->setZoomFactor(mZoomFactors[mZoomFactorIndex]);
}

void FilterWidget::on_mButtonZoom100_clicked()
{
    mZoomFactorIndex = 0;
    ui->mWidgetCurves->fit();
}

void FilterWidget::on_mButtonOutputModeKeyedImage_toggled(bool c)
{
    if (!c)
        return;

    if (mEmitSignals)
        emit outputModeChanged(Filter::KeyedImage);
}

void FilterWidget::on_mButtonOutputModeMatte_toggled(bool c)
{
    if (!c)
        return;

    if (mEmitSignals)
        emit outputModeChanged(Filter::Matte);
}

void FilterWidget::on_mSliderPreblurRadius_valueChanged(int value)
{
    ui->mSpinPreblurRadius->setValue(value / 100.0);
    if (mEmitSignals)
        emit preblurRadiusChanged(value / 100.0);
}

void FilterWidget::on_mSpinPreblurRadius_valueChanged(double arg1)
{
    ui->mSliderPreblurRadius->setValue((int)round(arg1 * 100.0));
    if (mEmitSignals)
        emit preblurRadiusChanged(arg1);
}
