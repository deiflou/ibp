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
    void referenceChanged(Filter::Reference v);
    void thresholdChanged(int v);
    void marginsChanged(const QMargins & v);

public slots:
    void setReference(Filter::Reference v);
    void setThreshold(int v);
    void setMargins(const QMargins & v);

private slots:
    void on_mButtonReferenceAlphaChannel_toggled(bool v);
    void on_mButtonReferenceLuma_toggled(bool v);
    void on_mSliderThreshold_valueChanged(int v);
    void on_mSpinThreshold_valueChanged(int v);
    void On_mSpinMargins_valueChanged(int v);
};

#endif // FILTERWIDGET_H
