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

#include "linearsplineinterpolator.h"

namespace anitools {
namespace misc {

LinearSplineInterpolator::LinearSplineInterpolator() :
    BaseSplineInterpolator()
{
}

double LinearSplineInterpolator::f(double x)
{
    if (mKnots.size() < 1) return 0.0;

    int piece = getPiece(x);
    if (piece == -1) return floorExtrapolate(x);
    if (piece == -2) return ceilExtrapolate(x);

    double t = (x - mKnots[piece].x()) / (mKnots[piece + 1].x() - mKnots[piece].x());
    return (1.0 - t) * mKnots[piece].y() + t * mKnots[piece + 1].y();
}

// this returns -1 is X is lower than knot[0].x and n if X is higher than knot[n].x
int LinearSplineInterpolator::getPiece(double x)
{
    for (int i = 0; i < mKnots.size(); i++)
        if (x < mKnots[i].x()) return i - 1;
    return -2;
}

double LinearSplineInterpolator::floorExtrapolate(double x)
{
    Q_UNUSED(x)
    return mKnots[0].y();
}
double LinearSplineInterpolator::ceilExtrapolate(double x)
{
    Q_UNUSED(x)
    return mKnots[mKnots.size() - 1].y();
}

}}
