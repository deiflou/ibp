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

#include "filterwidget.h"
#include "ui_filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true)
{
    ui->setupUi(this);

    ui->mComboMorphologyOp->addItems(QStringList() <<
                                     tr("Dilation") <<
                                     tr("Erosion") <<
                                     tr("Closing") <<
                                     tr("Opening"));
    ui->mComboMorphologyOp->setCurrentIndex(0);

    ui->mComboKernelShape->addItems(QStringList() <<
                                    tr("Ellipse") <<
                                    tr("Rectangle") <<
                                    tr("Diamond") <<
                                    tr("Octagon") <<
                                    tr("Plus") <<
                                    tr("Cross") <<
                                    tr("Ring"));
    ui->mComboKernelShape->setCurrentIndex(0);

    ui->mButtonLockRadius->setIcon(QIcon(":/ibp/icons/lock"));
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setModifyRGB(bool v)
{
    if (ui->mButtonAffectedChannelsRGB->isChecked() == v)
        return;
    mEmitSignals = false;
    ui->mButtonAffectedChannelsRGB->setChecked(v);
    mEmitSignals = true;
    emit modifyRGBChanged(v);
}

void FilterWidget::setModifyAlpha(bool v)
{
    if (ui->mButtonAffectedChannelsAlpha->isChecked() == v)
        return;
    mEmitSignals = false;
    ui->mButtonAffectedChannelsAlpha->setChecked(v);
    mEmitSignals = true;
    emit modifyAlphaChanged(v);
}

void FilterWidget::setMorphologyOp(Filter::MorphologyOp mop)
{
    if (mop == (Filter::MorphologyOp)ui->mComboMorphologyOp->currentIndex())
        return;
    ui->mComboMorphologyOp->setCurrentIndex(mop);
}

void FilterWidget::setKernelShape(Filter::KernelShape shape)
{
    if (shape == (Filter::KernelShape)ui->mComboKernelShape->currentIndex())
        return;
    ui->mComboKernelShape->setCurrentIndex(shape);
}

void FilterWidget::setHRadius(int r)
{
    if (ui->mSpinHRadius->value() == r)
        return;
    mEmitSignals = false;
    ui->mSpinHRadius->setValue(r);
    mEmitSignals = true;
    emit hRadiusChanged(r);
}

void FilterWidget::setVRadius(int r)
{
    if (ui->mSpinVRadius->value() == r)
        return;
    mEmitSignals = false;
    ui->mSpinVRadius->setValue(r);
    mEmitSignals = true;
    emit vRadiusChanged(r);
}

void FilterWidget::setLockRadius(bool l)
{
    if (ui->mButtonLockRadius->isChecked() == l)
        return;
    mEmitSignals = false;
    ui->mButtonLockRadius->setChecked(l);
    mEmitSignals = true;
    emit lockRadiusChanged(l);
}

void FilterWidget::on_mButtonAffectedChannelsRGB_toggled(bool c)
{
    if (mEmitSignals)
        emit modifyRGBChanged(c);
}

void FilterWidget::on_mButtonAffectedChannelsAlpha_toggled(bool c)
{
    if (mEmitSignals)
        emit modifyAlphaChanged(c);
}

void FilterWidget::on_mComboMorphologyOp_currentIndexChanged(int index)
{
    if (mEmitSignals)
        emit morphologyOpChanged((Filter::MorphologyOp)index);
}

void FilterWidget::on_mComboKernelShape_currentIndexChanged(int index)
{
    if (mEmitSignals)
        emit kernelShapeChanged((Filter::KernelShape)index);
}

void FilterWidget::on_mSliderHRadius_valueChanged(int value)
{
    ui->mSpinHRadius->setValue(value);
    if (mEmitSignals)
        emit hRadiusChanged(value);
}

void FilterWidget::on_mSpinHRadius_valueChanged(int arg1)
{
    ui->mSliderHRadius->setValue(arg1);
    if (ui->mButtonLockRadius->isChecked())
        ui->mSpinVRadius->setValue(arg1);
    if (mEmitSignals)
        emit hRadiusChanged(arg1);
}

void FilterWidget::on_mSliderVRadius_valueChanged(int value)
{
    ui->mSpinVRadius->setValue(value);
    if (mEmitSignals)
        emit vRadiusChanged(value);
}

void FilterWidget::on_mSpinVRadius_valueChanged(int arg1)
{
    ui->mSliderVRadius->setValue(arg1);
    if (ui->mButtonLockRadius->isChecked())
        ui->mSpinHRadius->setValue(arg1);
    if (mEmitSignals)
        emit vRadiusChanged(arg1);
}

void FilterWidget::on_mButtonLockRadius_toggled(bool checked)
{
    if (checked)
        ui->mSpinVRadius->setValue(ui->mSpinHRadius->value());
    if (mEmitSignals)
        emit lockRadiusChanged(checked);
}
