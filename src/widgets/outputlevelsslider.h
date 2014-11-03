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

#ifndef ANITOOLS_WIDGETS_OUTPUTLEVELSSLIDER_H
#define ANITOOLS_WIDGETS_OUTPUTLEVELSSLIDER_H

#include <QWidget>

namespace anitools {
namespace widgets {

class OutputLevelsSlider : public QWidget
{
    Q_OBJECT

private:
    static const int kLeftMargin = 4;
    static const int kTopMargin = 1;
    static const int kRightMargin = 4;
    static const int kBottomMargin = 7;
    static const int kHandleHeight = 8;
    static const int kHandleWidth = 8;

    double mBlackPoint, mWhitePoint;
    QImage mFunction;
    int mHandlePressed, mHandleSelected;

    void makeFunction();
    void paintHandle(QPainter & p, const QPoint & pos, const QColor & c, bool selected = false);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *);

public:
    explicit OutputLevelsSlider(QWidget *parent = 0);

    double blackPoint();
    double whitePoint();

signals:
    void blackPointChanged(double v);
    void whitePointChanged(double v);

public slots:
    void setBlackPoint(double v);
    void setWhitePoint(double v);
    void setValues(double b, double w);
};

}}
#endif // ANITOOLS_WIDGETS_OUTPUTLEVELSSLIDER_H
