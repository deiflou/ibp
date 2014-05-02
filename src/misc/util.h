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

#ifndef ANITOOLS_MISC_UTIL_H
#define ANITOOLS_MISC_UTIL_H

#include <QPointF>
#include <QRectF>
#include <QStringList>
#include <math.h>

namespace anitools {
namespace misc {

#define AT_PI 3.1415926536
#define AT_PIOVER2 (AT_PI / 2.0)
#define AT_DegToRad(deg) ((deg) * AT_PI / 180.0)
#define AT_RadToDeg(rad) ((rad) * 180.0 / AT_PI)
#define AT_X_CUBE_MINUS_X(x) ((x) * (x) * (x) - (x))
#define AT_minimum(a, b) ((a) < (b) ? (a) : (b))
#define AT_minimum3(a, b, c) AT_minimum((a), AT_minimum((b), (c)))
#define AT_maximum(a, b) ((a) > (b) ? (a) : (b))
#define AT_maximum3(a, b, c) AT_maximum((a), AT_maximum((b), (c)))

inline double euclideanDistance(double x0, double y0, double x1, double y1)
{
    double dx = x1 - x0;
    double dy = y1 - y0;
    return sqrt(dx * dx + dy * dy);
}
inline double euclideanDistance(const QPointF & p0, const QPointF & p1)
{
    double dx = p1.x() - p0.x();
    double dy = p1.y() - p0.y();
    return sqrt(dx * dx + dy * dy);
}

inline QRect fitRectWithin(const QRect & inner, const QRect & outer, double * resizeFactor = 0)
{
    double innerAspectRatio = inner.width() / (double)inner.height();
    double outerAspectRatio = outer.width() / (double)outer.height();

    double rf = (innerAspectRatio >= outerAspectRatio) ?
                (outer.width() / (double)inner.width()) :
                (outer.height() / (double)inner.height());

    int newWidth = inner.width() * rf;
    int newHeight = inner.height() * rf;
    int newLeft = outer.left() + ((outer.width() - newWidth) >> 1);
    int newTop = outer.top() + ((outer.height() - newHeight) >> 1);

    if (resizeFactor)
        *resizeFactor = rf;
    return QRect(newLeft, newTop, newWidth, newHeight);
}

inline QRectF fitRectWithin(const QRectF & inner, const QRectF & outer, double * resizeFactor = 0)
{
    double innerAspectRatio = inner.width() / inner.height();
    double outerAspectRatio = outer.width() / outer.height();

    double rf = (innerAspectRatio >= outerAspectRatio) ?
                (outer.width() / inner.width()) :
                (outer.height() / inner.height());

    double newWidth = inner.width() * rf;
    double newHeight = inner.height() * rf;
    double newLeft = outer.left() + ((outer.width() - newWidth) / 2.0);
    double newTop = outer.top() + ((outer.height() - newHeight) / 2.0);

    if (resizeFactor)
        *resizeFactor = rf;
    return QRectF(newLeft, newTop, newWidth, newHeight);
}

}}

#endif // ANITOOLS_MISC_UTIL_H
