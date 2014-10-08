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

#ifndef ANITOOLS_MISC_SPLINEINTERPOLATOR_H
#define ANITOOLS_MISC_SPLINEINTERPOLATOR_H

#include <QPointF>
#include <QVector>

namespace anitools {
namespace misc {

typedef QPointF SplineInterpolatorKnot;
typedef QVector<SplineInterpolatorKnot> SplineInterpolatorKnots;

extern bool SplineInterpolatorKnotsLessThan(const SplineInterpolatorKnot &s1, const SplineInterpolatorKnot &s2);

class SplineInterpolator
{

public:
    enum ExtrapolationMode
    {
        Clamp = 0
    };

    virtual ~SplineInterpolator() {}

    virtual const SplineInterpolatorKnots & knots() const = 0;
    virtual const SplineInterpolatorKnot & knot(int i) const = 0;
    virtual int size() const = 0;
    virtual bool setKnots(const SplineInterpolatorKnots &k) = 0;
    virtual bool setKnot(int i, const SplineInterpolatorKnot &k) = 0;
    virtual bool setKnot(int i, double nx, double ny) = 0;
    virtual bool setKnot(double x, const SplineInterpolatorKnot &k) = 0;
    virtual bool setKnot(double x, double nx, double ny) = 0;
    virtual bool addKnot(const SplineInterpolatorKnot &k, bool replace = false) = 0;
    virtual bool addKnot(double nx, double ny, bool replace = false) = 0;
    virtual bool removeKnot(double x) = 0;
    virtual bool removeKnot(int i) = 0;

    virtual double f(double x) = 0;

    virtual ExtrapolationMode floorExtrapolationMode() const = 0;
    virtual ExtrapolationMode ceilExtrapolationMode() const = 0;
    virtual void setExtrapolationMode(ExtrapolationMode f, ExtrapolationMode c) = 0;
};

}}
#endif // ANITOOLS_MISC_SPLINEINTERPOLATOR_H
