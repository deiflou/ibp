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

#ifndef ANITOOLS_WIDGETS_CURVES_H
#define ANITOOLS_WIDGETS_CURVES_H

#include <QWidget>
#include "../misc/splineinterpolator.h"

using namespace anitools::misc;

namespace anitools {
namespace widgets {

class Curves : public QWidget
{
    Q_OBJECT

public:
    enum InterpolationMode
    {
        NearestNeighbor,
        Linear,
        Cubic
    };

    explicit Curves(QWidget *parent = 0);

    SplineInterpolatorKnots knots() const;
    int selectedKnotIndex() const;
    SplineInterpolatorKnot selectedKnot() const;
    InterpolationMode interpolationMode() const;

    QSize optimalSizeForWidth(int w) const;

public slots:
    void setKnots(const SplineInterpolatorKnots & k);
    void setSelectedKnot(double x, double y);
    void setSelectedKnot(const SplineInterpolatorKnot &k);
    void setInterpolationMode(InterpolationMode m);

signals:
    void knotsChanged(const SplineInterpolatorKnots & k);
    void selectedKnotChanged(int i);
    void interpolationModeChanged(InterpolationMode m);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    static const int KNOT_RADIUS = 5;
    static const int KNOT_MOUSE_DISTANCE = KNOT_RADIUS * 2;
    static const double KEYPRESS_INCREMENT = 1.0 / 256.0;
    static const double MINIMUM_DISTANCE_BETWEEN_KNOTS = 1.0 / 256.0 * 2.0;
    static const int MAX_N_KNOTS = 19;
    static const int BAR_SIZE = 10;
    static const int TOTAL_BAR_SIZE = BAR_SIZE * 2;
    static const int SEPARATOR_SIZE = 5;

    SplineInterpolator * mSplineInterpolator;
    InterpolationMode mInterpolationMode;

    int mKnotSelected, mKnotPressed;

    QPolygonF mPolygon;
    QImage mBar1, mBar2;
    bool mFunctionValuesCalculated;

    bool mShowBars;

    void calculateFunctionValues();
};

}}
#endif // ANITOOLS_WIDGETS_CURVES_H
