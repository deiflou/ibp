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
#include <QToolButton>
#include <QSlider>
#include <QSpinBox>

#include "filter.h"
#include "../widgets/anchorpositionbox.h"

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
    void widthChanged(int w);
    void heightChanged(int h);
    void widthModeChanged(Filter::SizeMode wm);
    void heightModeChanged(Filter::SizeMode hm);
    void resizeModeChanged(Filter::ResizeMode rm);
    void anchorPositionChanged(anitools::misc::Alignment ap);
    void backgroundColorChanged(const QColor & c);

public slots:
    void setWidth(int w);
    void setHeight(int h);
    void setWidthMode(Filter::SizeMode wm);
    void setHeightMode(Filter::SizeMode hm);
    void setResizeMode(Filter::ResizeMode rm);
    void setAnchorPosition(anitools::misc::Alignment ap);
    void setBackgroundColor(const QColor & c);

private slots:
    void on_mSpinWidth_valueChanged(int arg1);
    void on_mSpinHeight_valueChanged(int arg1);
    void on_mComboWidthMode_currentIndexChanged(int index);
    void on_mComboHeightMode_currentIndexChanged(int index);
    void on_mButtonResizeModeAbsolute_toggled(bool c);
    void on_mButtonResizeModeRelative_toggled(bool c);
    void on_mAnchorPositionBox_anchorPositionChanged(anitools::misc::Alignment ap);
    void on_mButtonBackgroundColor_colorChanged(const QColor & c);

};

#endif // FILTERWIDGET_H
