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

#include "levelscurvespaintdelegate.h"

namespace ibp {
namespace widgets {

LevelsCurvesPaintDelegate::LevelsCurvesPaintDelegate(QObject *parent) :
    mBar1(256, 1, QImage::Format_ARGB32),
    mBar2(256, 1, QImage::Format_ARGB32)
{
    setParent(parent);
    for (int i = 0; i < 256; i++)
    {
        mBar1.setPixel(i, 0, qRgb(i, i, i));
        mBar2.setPixel(i, 0, qRgb(i, i, i));
    }
}

LevelsCurvesPaintDelegate::~LevelsCurvesPaintDelegate()
{

}

void LevelsCurvesPaintDelegate::update(UpdateEvent e, const Curves *w, const QRect & r)
{
    Q_UNUSED(r)

    if (e == PeriodicChanged || e == InterpolationModeChanged || e == KnotsChanged)
    {
        BGRA * bits = (BGRA *)mBar2.bits();
        for (int x = 0; x < 256; x++, bits++)
            bits->r = bits->g = bits->b = round(((Curves *)w)->valueAt(x / 255.) * 255.);
    }
}

void LevelsCurvesPaintDelegate::paint(QPainter &p, const Curves *w, const QRect &r, QStyle::State widgetState,
                                      const QPolygonF &graph, const QVector<QPointF> &knotPos,
                                      const QVector<QStyle::State> &knotStates, const QSize &knotSize)
{
    paintBackground(p, w, r, widgetState);

    QPainterPath path;
    path.addRect(r.left() - 10, r.bottom() - kTotalBarSize + 1, r.width() + 20, kTotalBarSize);
    p.setClipPath(p.clipPath().subtracted(path));

    paintGraph(graph, p, w, r, widgetState);
    paintKnots(knotPos, knotStates, knotSize, p, w, r, widgetState);
}

void LevelsCurvesPaintDelegate::paintBackground(QPainter &p, const Curves *w,
                                                const QRect & r, QStyle::State widgetState)
{
    Q_UNUSED(widgetState)

    QRect gr = graphRect(r);
    QRect pr = r.adjusted(0, 0, 0, -kTotalBarSize);
    double off = w->offset();
    double scale = w->zoomFactor();

    if (pr.isValid())
    {
        // background
        //p.setRenderHint(QPainter::Antialiasing, false);
        p.setPen(Qt::NoPen);
        p.setBrush(w->palette().color(QPalette::Dark).darker(150));
        p.drawRoundedRect(pr, 1, 1);
        // grid lines
        p.setPen(QPen(w->palette().color(QPalette::Mid), 1, Qt::DotLine));

        p.drawLine(gr.left() - off, pr.top(), gr.left() - off, pr.top() + pr.height());
        p.drawLine(gr.left() - off + gr.width() * scale * 0.25, pr.top(),
                   gr.left() - off + gr.width() * scale * 0.25, pr.top() + pr.height());
        p.drawLine(gr.left() - off + gr.width() * scale * 0.50, pr.top(),
                   gr.left() - off + gr.width() * scale * 0.50, pr.top() + pr.height());
        p.drawLine(gr.left() - off + gr.width() * scale * 0.75, pr.top(),
                   gr.left() - off + gr.width() * scale * 0.75, pr.top() + pr.height());
        p.drawLine(gr.left() - off + gr.width() * scale, pr.top(),
                   gr.left() - off + gr.width() * scale, pr.top() + pr.height());

        p.drawLine(pr.left(), gr.top(), pr.left() + pr.width(), gr.top());
        p.drawLine(pr.left(), gr.top() + gr.height() * 0.25,
                   pr.left() + pr.width(), gr.top() + gr.height() * 0.25);
        p.drawLine(pr.left(), gr.top() + gr.height() * 0.50,
                   pr.left() + pr.width(), gr.top() + gr.height() * 0.50);
        p.drawLine(pr.left(), gr.top() + gr.height() * 0.75,
                   pr.left() + pr.width(), gr.top() + gr.height() * 0.75);
        p.drawLine(pr.left(), gr.top() + gr.height(), pr.left() + pr.width(), gr.top() + gr.height());

        // base line
        p.setRenderHint(QPainter::Antialiasing);
        p.drawLine(gr.left() - off + (pr.left() - gr.left()) * scale, pr.top() + pr.height(),
                   gr.left() - off + (pr.left() - gr.left()) * scale + pr.width() * scale, pr.top());
    }

    // bars
    QRect br = r.adjusted(0, pr.height(), 0, 0);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.drawImage(br, mBar1);
    p.drawImage(br.adjusted(0, kBarSize, 0, 0), mBar2);
}

void LevelsCurvesPaintDelegate::paintGraph(const QPolygonF &g,
                                           QPainter &p, const Curves *w,
                                           const QRect & r, QStyle::State widgetState)
{
    Q_UNUSED(w)

    QRect pr = r.adjusted(0, 0, 0, -kTotalBarSize);
    QPolygonF poly = g;
    poly.append(QPointF(pr.right() + 10, poly.last().y()));
    poly.append(QPointF(pr.right() + 10, pr.bottom() + 10));
    poly.append(QPointF(pr.left() - 10, pr.bottom() + 10));
    poly.append(QPointF(pr.left() - 10, poly.first().y()));

    int opacity = (widgetState & QStyle::State_MouseOver || widgetState & QStyle::State_HasFocus) ? 64 : 32;
    QColor fillColor = w->palette().color(QPalette::Midlight);

    fillColor.setAlpha(opacity);
    p.setBrush(fillColor);
    p.setPen(QPen(QColor(0, 0, 0, opacity), 2));
    p.drawPolygon(poly.translated(1, 1));
    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(QColor(255, 255, 255, opacity), 2));
    p.drawPolyline(poly);
}

void LevelsCurvesPaintDelegate::paintKnots(const QVector<QPointF> & pts,
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

QRect LevelsCurvesPaintDelegate::graphRect(const QRect &r) const
{
    return r.adjusted(10, 10, -10, -(kTotalBarSize + 10));
}

}}
