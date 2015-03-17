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

void FilterWidget::setAnchorPosition(anitools::misc::Alignment ap)
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

void FilterWidget::on_mAnchorPositionBox_anchorPositionChanged(anitools::misc::Alignment ap)
{
    if (mEmitSignals)
        emit anchorPositionChanged(ap);
}

void FilterWidget::on_mButtonBackgroundColor_colorChanged(const QColor &c)
{
    if (mEmitSignals)
        emit backgroundColorChanged(c);
}
