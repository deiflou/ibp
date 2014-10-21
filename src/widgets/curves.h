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
#include <QStyle>
#include <QScrollBar>
#include "../misc/splineinterpolator.h"

using namespace anitools::misc;

namespace anitools {
namespace widgets {

class Curves;
class CurvesPaintDelegate : public QObject
{
    Q_OBJECT

public:
    enum UpdateEvent
    {
        ZoomFactorChanged, OffsetChanged, PeriodicChanged, InputEnabledChanged, InterpolationModeChanged, KnotsChanged
    };

    virtual ~CurvesPaintDelegate() {}
    virtual void update(UpdateEvent e, const Curves * w, const QRect & r) = 0;
    virtual void paint(QPainter & p, const Curves * w, const QRect & r,
                       QStyle::State widgetState, const QPolygonF & graph,
                       const QVector<QPointF> & knotPos, const QVector<QStyle::State> & knotStates,
                       const QSize & knotSize) = 0;
    virtual QRect graphRect(const QRect & r) const = 0;

signals:
    void updateRequired();
};

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

    double zoomFactor() const;
    double offset() const;
    bool isPeriodic() const;
    bool isInputEnabled() const;
    const SplineInterpolatorKnots & knots() const;
    int selectedKnotIndex() const;
    const SplineInterpolatorKnot & selectedKnot() const;
    InterpolationMode interpolationMode() const;
    CurvesPaintDelegate * paintDelegate() const;
    double valueAt(double v);
    double mapToSplineInterpolator(double v) const;
    double mapFromSplineInterpolator(double v) const;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void leaveEvent(QEvent *);
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject * o, QEvent * e);
    void keyPressEvent(QKeyEvent *e);

private:
    enum InputStatus
    {
        NoStatus, DraggingKnot
    };

    static const int kLeftMargin = 2;
    static const int kTopMargin = 1;
    static const int kRightMargin = 2;
    static const int kBottomMargin = 3;
    static const QSize kKnotSize;
    static const int kMinimumSizeForInput = 50;
    static const double kKeypressIncrement = 1.0 / 256.0;
    static const double kMinimumDistanceBetweenKnots = 1.0 / 256.0 * 3.0;
    static const int kMaximumNumberOfKnots = 19;
    static const double kMinimumDistanceToAddKnot = 8.;

    double mZoomFactor;
    double mOffset;
    bool mIsPeriodic;
    bool mIsInputEnabled;
    InputStatus mInputStatus;
    int mKnotIndex;
    SplineInterpolator * mSplineInterpolator;
    InterpolationMode mInterpolationMode;
    CurvesPaintDelegate * mPaintDelegate;
    QStyle::State mWidgetState;
    QVector<QStyle::State> mKnotStates;
    QScrollBar * mScrollBar;
    bool mEmitScrolbarSignals;

    void updateScrollBar();
    QRect rectWithoutMargins() const;
    QRect graphRect() const;
    int knotUnderCoords(const QPoint & p, bool addKnotIfPossible = false);
    void addKnotIfPossible(const QPoint & p);

signals:
    void zoomFactorChanged(double v);
    void offsetChanged(double v);
    void periodicChanged(bool v);
    void inputEnabledChanged(bool v);
    void knotsChanged(const SplineInterpolatorKnots & k);
    void selectedKnotChanged(int i);
    void interpolationModeChanged(InterpolationMode m);
    void paintDelegateChanged(CurvesPaintDelegate * pd);

public slots:
    void setZoomFactor(double v);
    void setOffset(double v);
    void center();
    void fit();
    void setPeriodic(bool v);
    void setInputEnabled(bool v);
    void setKnots(const SplineInterpolatorKnots & k);
    void setSelectedKnot(double x, double y);
    void setSelectedKnot(const SplineInterpolatorKnot &k);
    void setInterpolationMode(InterpolationMode m);
    void setPaintDelegate(CurvesPaintDelegate * pd);

private slots:
    void On_mScrollBar_valueChanged(int v);
};

}}
#endif // ANITOOLS_WIDGETS_CURVES_H
