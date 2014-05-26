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

#ifndef ANITOOLS_WIDGETS_OUTPUTLEVELSWIDGET_H
#define ANITOOLS_WIDGETS_OUTPUTLEVELSWIDGET_H

#include <QWidget>

namespace anitools {
namespace widgets {

namespace Ui {
class OutputLevelsWidget;
}

class OutputLevelsWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::OutputLevelsWidget *ui;

    int mUnits;

public:
    explicit OutputLevelsWidget(QWidget *parent = 0);
    ~OutputLevelsWidget();

    double blackPoint();
    double whitePoint();

    int units();

signals:
    void blackPointChanged(double v);
    void whitePointChanged(double v);

public slots:
    void setBlackPoint(double v);
    void setWhitePoint(double v);
    void setValues(double b, double w);

    void setUnits(int u);

private slots:
    void on_mSliderOutputLevels_blackPointChanged(double v);
    void on_mSliderOutputLevels_whitePointChanged(double v);
    void on_mSpinBlackPoint_valueChanged(double v);
    void on_mSpinWhitePoint_valueChanged(double v);
};

}}
#endif // ANITOOLS_WIDGETS_OUTPUTLEVELSWIDGET_H
