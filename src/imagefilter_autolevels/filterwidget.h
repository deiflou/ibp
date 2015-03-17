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
    void enhanceChannelsSeparatelyChanged(bool v);
    void adjustMidtonesChanged(bool v);
    void targetColorShadowsChanged(const QColor & v);
    void targetColorMidtonesChanged(const QColor & v);
    void targetColorHighlightsChanged(const QColor & v);
    void clippingShadowsChanged(double v);
    void clippingHighlightsChanged(double v);

public slots:
    void setEnhanceChannelsSeparately(bool v);
    void setAdjustMidtones(bool v);
    void setTargetColorShadows(const QColor & v);
    void setTargetColorMidtones(const QColor & v);
    void setTargetColorHighlights(const QColor & v);
    void setClippingShadows(double v);
    void setClippingHighlights(double v);

private slots:
    void on_mButtonHistogramOptionsEnhanceChannelsSeparately_toggled(bool v);
    void on_mButtonHistogramOptionsAdjustMidtones_toggled(bool v);
    void on_mButtonTargetColorsShadows_colorChanged(const QColor & v);
    void on_mButtonTargetColorsMidtones_colorChanged(const QColor & v);
    void on_mButtonTargetColorsHighlights_colorChanged(const QColor & v);
    void on_mSpinClippingShadows_valueChanged(double v);
    void on_mSpinClippingHighlights_valueChanged(double v);
};

#endif // FILTERWIDGET_H
