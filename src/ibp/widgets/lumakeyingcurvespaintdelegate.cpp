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

#include <QPainter>
#include <QStyle>
#include <math.h>

#include "lumakeyingcurvespaintdelegate.h"
#include "../imgproc/types.h"

using namespace ibp::imgproc;

namespace ibp {
namespace widgets {

LumaKeyingCurvesPaintDelegate::LumaKeyingCurvesPaintDelegate(QObject *parent) :
    mCheckerboardImage(2, 2, QImage::Format_RGB888),
    mGradientImage(256, 1, QImage::Format_ARGB32),
    mIsInverted(false)
{
    setParent(parent);
    BGRA * bits = (BGRA *)mGradientImage.bits();
    for (int x = 0; x < 256; x++, bits++)
    {
        bits->r = bits->g = bits->b = x;
        bits->a = 255;
    }
}

LumaKeyingCurvesPaintDelegate::~LumaKeyingCurvesPaintDelegate()
{

}

void LumaKeyingCurvesPaintDelegate::update(UpdateEvent e, const Curves *w, const QRect & r)
{
    Q_UNUSED(r)

    if (e == PeriodicChanged || e == InterpolationModeChanged || e == KnotsChanged)
    {
        BGRA * bits = (BGRA *)mGradientImage.bits();
        if (mIsInverted)
            for (int x = 0; x < 256; x++, bits++)
                bits->a = round(((Curves *)w)->valueAt(x / 255.) * 255.);
        else
            for (int x = 0; x < 256; x++, bits++)
                bits->a = 255 - round(((Curves *)w)->valueAt(x / 255.) * 255.);
    }
}

void LumaKeyingCurvesPaintDelegate::paint(QPainter &p, const Curves *w, const QRect &r, QStyle::State widgetState,
                                          const QPolygonF &graph, const QVector<QPointF> &knotPos,
                                          const QVector<QStyle::State> &knotStates, const QSize &knotSize)
{
    paintBackground(p, w, r, widgetState);
    paintGraph(graph, p, w, r, widgetState);
    paintKnots(knotPos, knotStates, knotSize, p, w, r, widgetState);
}

void LumaKeyingCurvesPaintDelegate::paintBackground(QPainter &p, const Curves *w,
                                                    const QRect & r, QStyle::State widgetState)
{
    Q_UNUSED(widgetState)

    QRect gr = graphRect(r);

    // checkerboard background
    int checkerboardColor1 = w->palette().color(QPalette::Light).rgb();
    int checkerboardColor2 = w->palette().color(QPalette::Midlight).rgb();
    mCheckerboardImage.setPixel(0, 0, checkerboardColor1);
    mCheckerboardImage.setPixel(1, 1, checkerboardColor1);
    mCheckerboardImage.setPixel(0, 1, checkerboardColor2);
    mCheckerboardImage.setPixel(1, 0, checkerboardColor2);
    QBrush checkerboardBrush = mCheckerboardImage;
    checkerboardBrush.setTransform(QTransform(6, 0, 0, 0, 6, 0, 0, 0, 1));
    p.setBrush(checkerboardBrush);
    p.setPen(Qt::NoPen);
    p.setBrushOrigin(-w->offset() + gr.left(), gr.top());
    p.drawRect(r);

    QRectF rr(gr.x() - w->offset(), r.y(), gr.width() * w->zoomFactor(), r.height());

    // color gradient
    p.drawImage(rr, mGradientImage);
    if (w->isPeriodic())
    {
        p.drawImage(rr.translated(-rr.width(), 0), mGradientImage);
        p.drawImage(rr.translated(rr.width(), 0), mGradientImage);
    }
}

void LumaKeyingCurvesPaintDelegate::paintGraph(const QPolygonF &g,
                                               QPainter &p, const Curves *w,
                                               const QRect & r, QStyle::State widgetState)
{
    Q_UNUSED(w)
    Q_UNUSED(r)

    int opacity = (widgetState & QStyle::State_MouseOver || widgetState & QStyle::State_HasFocus) ? 64 : 32;
    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(QColor(0, 0, 0, opacity), 2));
    p.drawPolyline(g.translated(1, 1));
    p.setPen(QPen(QColor(255, 255, 255, opacity), 2));
    p.drawPolyline(g);
}

void LumaKeyingCurvesPaintDelegate::paintKnots(const QVector<QPointF> & pts,
                                               const QVector<QStyle::State> & sts,
                                               const QSize & s,
                                               QPainter & p, const Curves * w,
                                               const QRect & r, QStyle::State widgetState)
{
    Q_UNUSED(r)

    const double radiusX = s.width() / 2.;
    const double radiusY = s.height() / 2.;
    QColor fillColor = w->palette().color(QPalette::Highlight);
    int opacity, fillOpacity;

    for (int i = 0; i < pts.size(); i++)
    {
        if (widgetState & QStyle::State_MouseOver || widgetState & QStyle::State_HasFocus)
        {
            if (sts[i] & QStyle::State_MouseOver)
            {
                if (sts[i] & QStyle::State_Sunken)
                {
                    opacity = 200;
                    fillOpacity = (sts[i] & QStyle::State_Selected) ? 255 : 0;
                }
                else
                {
                    opacity = 128;
                    fillOpacity = (sts[i] & QStyle::State_Selected) ? 200 : 0;
                }
            }
            else
            {
                opacity = 64;
                fillOpacity = (sts[i] & QStyle::State_Selected) ? 128 : 0;
            }
        }
        else
        {
            opacity = 32;
            fillOpacity = (sts[i] & QStyle::State_Selected) ? 64 : 0;
        }

        fillColor.setAlpha(fillOpacity);
        p.setBrush(fillColor);
        p.setPen(QPen(QColor(0, 0, 0, opacity), 1));
        p.drawEllipse(pts[i], radiusX, radiusY);

        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(QColor(255, 255, 255, opacity), 1));
        p.drawEllipse(pts[i], radiusX - 1, radiusY - 1);
    }
}

QRect LumaKeyingCurvesPaintDelegate::graphRect(const QRect &r) const
{
    return r.adjusted(0, 5, 0, -5);
}
bool LumaKeyingCurvesPaintDelegate::isInverted() const
{
    return mIsInverted;
}

void LumaKeyingCurvesPaintDelegate::setInverted(bool v)
{
    if (mIsInverted == v)
        return;

    mIsInverted = v;

    BGRA * bits = (BGRA *)mGradientImage.bits();
    for (int x = 0; x < 256; x++, bits++)
        bits->a = 255 - bits->a;

    emit updateRequired();
}

}}
