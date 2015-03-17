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

#ifndef ANITOOLS_WIDGETS_ANCHORPOSITIONBOX_H
#define ANITOOLS_WIDGETS_ANCHORPOSITIONBOX_H

#include <QWidget>

#include "../misc/util.h"

namespace anitools {
namespace widgets {

class AnchorPositionBox : public QWidget
{
    Q_OBJECT

public:
    explicit AnchorPositionBox(QWidget *parent = 0);

    anitools::misc::Alignment anchorPosition() const;

private:
    anitools::misc::Alignment mAnchorPosition;

    void paintArrow(QPainter &p, int x0, int y0, int x1, int y1);

public slots:
    void setAnchorPosition(anitools::misc::Alignment ap);

protected:
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent * e);

signals:
    void anchorPositionChanged(anitools::misc::Alignment ap);
    
};

}}
#endif // ANITOOLS_WIDGETS_ANCHORPOSITIONBOX_H
