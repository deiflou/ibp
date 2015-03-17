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

#ifndef ANITOOLS_WIDGETS_HSLKEYINGCURVESPAINTDELEGATE_H
#define ANITOOLS_WIDGETS_HSLKEYINGCURVESPAINTDELEGATE_H

#include <QImage>
#include "curves.h"
#include "../imgproc/types.h"

using namespace anitools::imgproc;

namespace anitools {
namespace widgets {

class HSLKeyingCurvesPaintDelegate : public CurvesPaintDelegate
{
    Q_OBJECT

public:
    explicit HSLKeyingCurvesPaintDelegate(QObject *parent = 0);
    ~HSLKeyingCurvesPaintDelegate();

    void update(UpdateEvent e, const Curves * w, const QRect & r);
    void paint(QPainter & p, const Curves * w, const QRect & r,
               QStyle::State widgetState, const QPolygonF & graph,
               const QVector<QPointF> & knotPos, const QVector<QStyle::State> & knotStates,
               const QSize & knotSize);
    QRect graphRect(const QRect & r) const;

    HSL color() const;
    ColorChannel channel() const;
    bool isInverted() const;
    void setColor(const HSL & color);
    void setChannel(ColorChannel channel);
    void setInverted(bool v);

private:
    QImage mCheckerboardImage, mGradientImage;
    HSL mColor;
    ColorChannel mChannel;
    bool mIsInverted;

    void paintBackground(QPainter &p, const Curves * w, const QRect & r, QStyle::State widgetState);
    void paintGraph(const QPolygonF & g, QPainter & p, const Curves * w, const QRect & r,
                    QStyle::State widgetState);
    void paintKnots(const QVector<QPointF> & pts, const QVector<QStyle::State> & sts,
                    const QSize & s, QPainter & p, const Curves * w, const QRect & r,
                    QStyle::State widgetState);

    void makeGradientImage();
};

}}

#endif // ANITOOLS_WIDGETS_HSLKEYINGCURVESPAINTDELEGATE_H
