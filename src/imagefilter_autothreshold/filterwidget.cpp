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

    ui->mButtonAffectedChannelsRed->hide();
    ui->mButtonAffectedChannelsGreen->hide();
    ui->mButtonAffectedChannelsBlue->hide();

    mButtonAffectedChannel[0] = ui->mButtonAffectedChannelsLuma;
    mButtonAffectedChannel[1] = ui->mButtonAffectedChannelsRed;
    mButtonAffectedChannel[2] = ui->mButtonAffectedChannelsGreen;
    mButtonAffectedChannel[3] = ui->mButtonAffectedChannelsBlue;
    mButtonAffectedChannel[4] = ui->mButtonAffectedChannelsAlpha;
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setThresholdMode(int m)
{
    if ((m == 0 && ui->mButtonThresholdModeGlobal->isChecked()) ||
        (m == 1 && ui->mButtonThresholdModeLocal->isChecked()))
        return;

    mEmitSignals = false;
    if (m == 0)
        ui->mButtonThresholdModeGlobal->setChecked(true);
    else
        ui->mButtonThresholdModeLocal->setChecked(true);
    mEmitSignals = true;

    emit thresholdModeChanged(m);
}

void FilterWidget::setColorMode(int m)
{
    if ((m == 0 && ui->mButtonColorModeLuma->isChecked()) ||
        (m == 1 && ui->mButtonColorModeRGB->isChecked()))
        return;

    mEmitSignals = false;
    if (m == 0)
        ui->mButtonColorModeLuma->setChecked(true);
    else
        ui->mButtonColorModeRGB->setChecked(true);
    mEmitSignals = true;

    emit colorModeChanged(m);
}

void FilterWidget::setAffectedChannel(int c, bool a)
{
    if (mButtonAffectedChannel[c]->isChecked() == a)
        return;

    mEmitSignals = false;
    mButtonAffectedChannel[c]->setChecked(a);
    mEmitSignals = true;

    emit affectedChannelChanged(c, a);
}

void FilterWidget::on_mButtonThresholdModeGlobal_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit thresholdModeChanged(0);
}

void FilterWidget::on_mButtonThresholdModeLocal_toggled(bool checked)
{
    if (!checked)
        return;
    if (mEmitSignals)
        emit thresholdModeChanged(1);
}

void FilterWidget::on_mButtonColorModeLuma_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mButtonAffectedChannelsLuma->show();
    ui->mButtonAffectedChannelsRed->hide();
    ui->mButtonAffectedChannelsGreen->hide();
    ui->mButtonAffectedChannelsBlue->hide();
    if (mEmitSignals)
        emit colorModeChanged(0);
}

void FilterWidget::on_mButtonColorModeRGB_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mButtonAffectedChannelsLuma->hide();
    ui->mButtonAffectedChannelsRed->show();
    ui->mButtonAffectedChannelsGreen->show();
    ui->mButtonAffectedChannelsBlue->show();
    if (mEmitSignals)
        emit colorModeChanged(1);
}

void FilterWidget::on_mButtonAffectedChannelsLuma_toggled(bool checked)
{
    if (mEmitSignals)
        emit affectedChannelChanged(0, checked);
}

void FilterWidget::on_mButtonAffectedChannelsRed_toggled(bool checked)
{
    if (mEmitSignals)
        emit affectedChannelChanged(1, checked);
}

void FilterWidget::on_mButtonAffectedChannelsGreen_toggled(bool checked)
{
    if (mEmitSignals)
        emit affectedChannelChanged(2, checked);
}

void FilterWidget::on_mButtonAffectedChannelsBlue_toggled(bool checked)
{
    if (mEmitSignals)
        emit affectedChannelChanged(3, checked);
}

void FilterWidget::on_mButtonAffectedChannelsAlpha_toggled(bool checked)
{
    if (mEmitSignals)
        emit affectedChannelChanged(4, checked);
}
