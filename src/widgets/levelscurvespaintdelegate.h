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

#ifndef ANITOOLS_WIDGETS_LEVELSCURVESPAINTDELEGATE_H
#define ANITOOLS_WIDGETS_LEVELSCURVESPAINTDELEGATE_H

#include <QImage>
#include "curves.h"
#include "../imgproc/types.h"

using namespace anitools::imgproc;

namespace anitools {
namespace widgets {

class LevelsCurvesPaintDelegate : public CurvesPaintDelegate
{
    Q_OBJECT

public:
    explicit LevelsCurvesPaintDelegate(QObject *parent = 0);
    ~LevelsCurvesPaintDelegate();

    void update(UpdateEvent e, const Curves * w, const QRect & r);
    void paint(QPainter & p, const Curves * w, const QRect & r,
               QStyle::State widgetState, const QPolygonF & graph,
               const QVector<QPointF> & knotPos, const QVector<QStyle::State> & knotStates,
               const QSize & knotSize);
    QRect graphRect(const QRect & r) const;

private:
    static const int kBarSize = 10;
    static const int kTotalBarSize = kBarSize * 2;

    QImage mBar1, mBar2;

    void paintBackground(QPainter &p, const Curves * w, const QRect & r, QStyle::State widgetState);
    void paintGraph(const QPolygonF & g, QPainter & p, const Curves * w, const QRect & r,
                    QStyle::State widgetState);
    void paintKnots(const QVector<QPointF> & pts, const QVector<QStyle::State> & sts,
                    const QSize & s, QPainter & p, const Curves * w, const QRect & r,
                    QStyle::State widgetState);
};

}}

#endif // ANITOOLS_WIDGETS_LEVELSCURVESPAINTDELEGATE_H
