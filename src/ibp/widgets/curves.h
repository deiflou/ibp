//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef IBP_WIDGETS_CURVES_H
#define IBP_WIDGETS_CURVES_H

#include <QWidget>
#include <QStyle>
#include <QScrollBar>
#include "../misc/interpolator1D.h"

using namespace ibp::misc;

namespace ibp {
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
    const Interpolator1DKnots & knots() const;
    int selectedKnotIndex() const;
    const Interpolator1DKnot & selectedKnot() const;
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
    static const int kMinimumSizeForInput = 40;
    static constexpr double kKeypressIncrement = 1.0 / 256.0;
    static constexpr double kMinimumDistanceBetweenKnots = 1.0 / 256.0 * 3.0;
    static const int kMaximumNumberOfKnots = 19;
    static constexpr double kMinimumDistanceToAddKnot = 8.;

    double mZoomFactor;
    double mOffset;
    bool mIsPeriodic;
    bool mIsInputEnabled;
    InputStatus mInputStatus;
    int mKnotIndex;
    Interpolator1D * mSplineInterpolator;
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
    void knotsChanged(const Interpolator1DKnots & k);
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
    void setKnots(const Interpolator1DKnots & k);
    void setSelectedKnot(double x, double y);
    void setSelectedKnot(const Interpolator1DKnot &k);
    void setInterpolationMode(InterpolationMode m);
    void setPaintDelegate(CurvesPaintDelegate * pd);

private slots:
    void On_mScrollBar_valueChanged(int v);
};

}}
#endif // IBP_WIDGETS_CURVES_H
