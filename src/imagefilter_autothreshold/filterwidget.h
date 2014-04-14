/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
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

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QSlider>
#include <QSpinBox>

#include "filter.h"

namespace Ui {
class FilterWidget;
}

class FilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget *parent = 0);
    ~FilterWidget();

private:
    Ui::FilterWidget *ui;
    bool mEmitSignals;

    QToolButton * mButtonAffectedChannel[5];

signals:
    void thresholdModeChanged(int m);
    void rgbModeChanged(int m);
    void affectedChannelChanged(int c, bool a);

public slots:
    void setThresholdMode(int m);
    void setRGBMode(int m);
    void setAffectedChannel(int c, bool a);

private slots:
    void on_mButtonThresholdModeGlobal_toggled(bool checked);
    void on_mButtonThresholdModeLocal_toggled(bool checked);
    void on_mButtonRGBModeTight_toggled(bool checked);
    void on_mButtonRGBModeIndependent_toggled(bool checked);
    void on_mButtonAffectedChannelsRGB_toggled(bool checked);
    void on_mButtonAffectedChannelsRed_toggled(bool checked);
    void on_mButtonAffectedChannelsGreen_toggled(bool checked);
    void on_mButtonAffectedChannelsBlue_toggled(bool checked);
    void on_mButtonAffectedChannelsAlpha_toggled(bool checked);
};

#endif // FILTERWIDGET_H
