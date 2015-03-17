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
    void modifyRGBChanged(bool v);
    void modifyAlphaChanged(bool v);
    void morphologyOpChanged(Filter::MorphologyOp mop);
    void kernelShapeChanged(Filter::KernelShape shape);
    void hRadiusChanged(int r);
    void vRadiusChanged(int r);
    void lockRadiusChanged(bool l);

public slots:
    void setModifyRGB(bool v);
    void setModifyAlpha(bool v);
    void setMorphologyOp(Filter::MorphologyOp mop);
    void setKernelShape(Filter::KernelShape shape);
    void setHRadius(int r);
    void setVRadius(int r);
    void setLockRadius(bool l);

private slots:
    void on_mButtonAffectedChannelsRGB_toggled(bool c);
    void on_mButtonAffectedChannelsAlpha_toggled(bool c);
    void on_mComboMorphologyOp_currentIndexChanged(int index);
    void on_mComboKernelShape_currentIndexChanged(int index);
    void on_mSliderHRadius_valueChanged(int value);
    void on_mSpinHRadius_valueChanged(int arg1);
    void on_mSliderVRadius_valueChanged(int value);
    void on_mSpinVRadius_valueChanged(int arg1);
    void on_mButtonLockRadius_toggled(bool checked);
};

#endif // FILTERWIDGET_H
