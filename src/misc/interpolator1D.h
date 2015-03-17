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

#ifndef ANITOOLS_MISC_INTERPOLATOR1D_H
#define ANITOOLS_MISC_INTERPOLATOR1D_H

#include <QPointF>
#include <QVector>

namespace anitools {
namespace misc {

typedef QPointF Interpolator1DKnot;
typedef QVector<Interpolator1DKnot> Interpolator1DKnots;

extern bool Interpolator1DKnotsLessThan(const Interpolator1DKnot &s1, const Interpolator1DKnot &s2);

class Interpolator1D
{

public:
    enum ExtrapolationMode
    {
        ExtrapolationMode_Constant,
        ExtrapolationMode_Clamp,
        ExtrapolationMode_FollowTangent,
        ExtrapolationMode_Repeat,
        ExtrapolationMode_Mirror
    };

    virtual ~Interpolator1D() {}
    virtual Interpolator1D * clone() const = 0;

    virtual const Interpolator1DKnots & knots() const = 0;
    virtual const Interpolator1DKnot & knot(int i) const = 0;
    virtual int size() const = 0;
    virtual bool setKnots(const Interpolator1DKnots &k) = 0;
    virtual bool setKnot(int i, const Interpolator1DKnot &k) = 0;
    virtual bool setKnot(int i, double nx, double ny) = 0;
    virtual bool setKnot(double x, const Interpolator1DKnot &k) = 0;
    virtual bool setKnot(double x, double nx, double ny) = 0;
    virtual bool addKnot(const Interpolator1DKnot &k, bool replace = false, int * index = 0) = 0;
    virtual bool addKnot(double nx, double ny, bool replace = false, int * index = 0) = 0;
    virtual bool removeKnot(double x) = 0;
    virtual bool removeKnot(int i) = 0;

    virtual double f(double x) = 0;

    virtual ExtrapolationMode floorExtrapolationMode() const = 0;
    virtual ExtrapolationMode ceilExtrapolationMode() const = 0;
    virtual double floorExtrapolationValue() const = 0;
    virtual double ceilExtrapolationValue() const = 0;
    virtual void setExtrapolationMode(ExtrapolationMode f, ExtrapolationMode c, double fv = 0., double cv = 0.) = 0;
};

}}
#endif // ANITOOLS_MISC_INTERPOLATOR1D_H
