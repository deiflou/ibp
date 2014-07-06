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

#ifndef ANITOOLS_MISC_CUBICSPLINEINTERPOLATOR_H
#define ANITOOLS_MISC_CUBICSPLINEINTERPOLATOR_H

#include "basesplineinterpolator.h"

namespace anitools {
namespace misc {

class CubicSplineInterpolator : public BaseSplineInterpolator
{
public:
    CubicSplineInterpolator();

    bool setKnots(const SplineInterpolatorKnots &k);
    bool setKnot(int i, const SplineInterpolatorKnot &k);
    bool setKnot(int i, double nx, double ny);
    bool setKnot(double x, const SplineInterpolatorKnot &k);
    bool setKnot(double x, double nx, double ny);
    bool addKnot(const SplineInterpolatorKnot &k, bool replace = false);
    bool addKnot(double nx, double ny, bool replace = false);
    bool removeKnot(double x);
    bool removeKnot(int i);

    double f(double x);

private:
    bool mDirty;
    QVector<double> p, u;

    int getPiece(double x);
    double floorExtrapolate(double x);
    double ceilExtrapolate(double x);
    void calculateCoeficients();
    inline double F(double x) { return x * x * x - x; }
};

}}
#endif // ANITOOLS_MISC_CUBICSPLINEINTERPOLATOR_H
