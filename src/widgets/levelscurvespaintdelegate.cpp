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

#include <QPainter>
#include <QStyle>
#include <math.h>

#include "levelscurvespaintdelegate.h"

namespace anitools {
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

void LevelsCurvesPaintDelegate::paintBackground(QPainter &p, const Curves *w,
                                                const QRect & r, QStyle::State widgetState)
{
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

    // focus rect
    if (widgetState & QStyle::State_HasFocus)
    {
        p.setPen(QPen(w->palette().color(QPalette::Highlight), 4));
        p.setBrush(Qt::NoBrush);
        p.drawRect(r);
    }

    // paint if disabled
    if (!(widgetState & QStyle::State_Enabled))
    {
        QColor disabledColor = w->palette().color(QPalette::Button);
        disabledColor.setAlpha(200);
        p.fillRect(r, disabledColor);
    }
}

void LevelsCurvesPaintDelegate::paintGraph(const QPolygonF &g,
                                           QPainter &p, const Curves *w,
                                           const QRect & r, QStyle::State widgetState)
{
    Q_UNUSED(w)

    QPolygonF poly = g;
    QRect gr = graphRect(r);
    for (int i = 0; i < poly.size(); i++)
        poly[i].setY(poly[i].y() * gr.height() + gr.top());

    int opacity = (widgetState & QStyle::State_Enabled) ? (widgetState & QStyle::State_MouseOver ||
                                                           widgetState & QStyle::State_HasFocus) ? 64 : 32 : 8;
    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(QColor(0, 0, 0, opacity), 2));
    p.drawPolyline(poly.translated(1, 1));
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
        if (widgetState & QStyle::State_Enabled)
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
        }
        else
        {
            opacity = 16;
            fillOpacity = (sts[i] & QStyle::State_Selected) ? 16 : 0;
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
