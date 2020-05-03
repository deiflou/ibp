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

void FilterWidget::setResizeMode(Filter::ResizeMode rm)
{
    if ((rm == Filter::Absolute && ui->mButtonResizeModeAbsolute->isChecked()) ||
        (rm == Filter::Relative && ui->mButtonResizeModeRelative->isChecked()))
        return;

    mEmitSignals = false;
    if (rm == Filter::Absolute)
        ui->mButtonResizeModeAbsolute->setChecked(true);
    else
        ui->mButtonResizeModeRelative->setChecked(true);
    mEmitSignals = true;

    emit resizeModeChanged(rm);
}

void FilterWidget::setAnchorPosition(ibp::misc::Alignment ap)
{
    if (ap == ui->mAnchorPositionBox->anchorPosition())
        return;

    mEmitSignals = false;
    ui->mAnchorPositionBox->setAnchorPosition(ap);
    mEmitSignals = true;

    emit anchorPositionChanged(ap);
}

void FilterWidget::setBackgroundColor(const QColor &c)
{
    if (c == ui->mButtonBackgroundColor->color())
        return;

    mEmitSignals = false;
    ui->mButtonBackgroundColor->setColor(c);
    mEmitSignals = true;

    emit backgroundColorChanged(c);
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
        if (ui->mButtonResizeModeAbsolute->isChecked())
            ui->mSpinWidth->setRange(1, 30000);
        else
            ui->mSpinWidth->setRange(-99, 30000);
        ui->mSpinWidth->setVisible(true);
    }
    else if (index == 1)
    {
        ui->mSpinWidth->setSuffix("px");
        if (ui->mButtonResizeModeAbsolute->isChecked())
            ui->mSpinWidth->setRange(1, 30000);
        else
            ui->mSpinWidth->setRange(-30000, 30000);
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
        if (ui->mButtonResizeModeAbsolute->isChecked())
            ui->mSpinHeight->setRange(1, 30000);
        else
            ui->mSpinHeight->setRange(-99, 30000);
        ui->mSpinHeight->setVisible(true);
    }
    else if (index == 1)
    {
        ui->mSpinHeight->setSuffix("px");
        if (ui->mButtonResizeModeAbsolute->isChecked())
            ui->mSpinHeight->setRange(1, 30000);
        else
            ui->mSpinHeight->setRange(-30000, 30000);
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

void FilterWidget::on_mButtonResizeModeAbsolute_toggled(bool c)
{
    if (!c)
        return;

    ui->mSpinWidth->setRange(1, 30000);
    ui->mSpinHeight->setRange(1, 30000);

    if (mEmitSignals)
        emit resizeModeChanged(Filter::Absolute);
}

void FilterWidget::on_mButtonResizeModeRelative_toggled(bool c)
{
    if (!c)
        return;

    if (ui->mComboWidthMode->currentIndex() == 0)
        ui->mSpinWidth->setRange(-99, 30000);
    else if (ui->mComboWidthMode->currentIndex() == 1)
        ui->mSpinWidth->setRange(-30000, 30000);

    if (ui->mComboHeightMode->currentIndex() == 0)
        ui->mSpinHeight->setRange(-99, 30000);
    else if (ui->mComboHeightMode->currentIndex() == 1)
        ui->mSpinHeight->setRange(-30000, 30000);

    if (mEmitSignals)
        emit resizeModeChanged(Filter::Relative);
}

void FilterWidget::on_mAnchorPositionBox_anchorPositionChanged(ibp::misc::Alignment ap)
{
    if (mEmitSignals)
        emit anchorPositionChanged(ap);
}

void FilterWidget::on_mButtonBackgroundColor_colorChanged(const QColor &c)
{
    if (mEmitSignals)
        emit backgroundColorChanged(c);
}
