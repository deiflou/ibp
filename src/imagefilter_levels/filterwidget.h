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

signals:
    void workingChannelChanged(Filter::WorkingChannel s);
    void inputBlackPointChanged(double v);
    void inputGammaChanged(double v);
    void inputWhitePointChanged(double v);
    void outputBlackPointChanged(double v);
    void outputWhitePointChanged(double v);

public slots:
    void setWorkingChannel(Filter::WorkingChannel s);
    void setInputBlackPoint(double v);
    void setInputGamma(double v);
    void setInputWhitePoint(double v);
    void setOutputBlackPoint(double v);
    void setOutputWhitePoint(double v);

private slots:
    void on_mButtonWorkingChannelRGB_toggled(bool c);
    void on_mButtonWorkingChannelRed_toggled(bool c);
    void on_mButtonWorkingChannelGreen_toggled(bool c);
    void on_mButtonWorkingChannelBlue_toggled(bool c);
    void on_mButtonWorkingChannelAlpha_toggled(bool c);

    void on_mWidgetInputLevels_blackPointChanged(double v);
    void on_mWidgetInputLevels_whitePointChanged(double v);
    void on_mWidgetInputLevels_gammaCorrectionChanged(double v);

    void on_mWidgetOutputLevels_blackPointChanged(double v);
    void on_mWidgetOutputLevels_whitePointChanged(double v);
};

#endif // FILTERWIDGET_H
