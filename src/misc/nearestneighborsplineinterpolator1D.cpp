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

#include <math.h>

#include "nearestneighborsplineinterpolator1D.h"

namespace anitools {
namespace misc {

NearestNeighborSplineInterpolator1D::NearestNeighborSplineInterpolator1D() :
    BaseSplineInterpolator1D()
{
}

Interpolator1D * NearestNeighborSplineInterpolator1D::clone() const
{
    NearestNeighborSplineInterpolator1D * si = new NearestNeighborSplineInterpolator1D();
    if (!si)
        return 0;
    si->mKnots = mKnots;
    si->mFloorExtrapolationMode = mFloorExtrapolationMode;
    si->mCeilExtrapolationMode = mCeilExtrapolationMode;
    si->mFloorExtrapolationValue = mFloorExtrapolationValue;
    si->mCeilExtrapolationValue = mCeilExtrapolationValue;
    return si;
}

double NearestNeighborSplineInterpolator1D::F(double x)
{
    const int piece = pieceForValue(x);
    return x < (mKnots[piece].x() + mKnots[piece + 1].x()) / 2. ? mKnots[piece].y() : mKnots[piece + 1].y();
}

double NearestNeighborSplineInterpolator1D::floorExtrapolate(double x)
{
    if (mFloorExtrapolationMode == ExtrapolationMode_Constant)
        return mFloorExtrapolationValue;
    else if (mFloorExtrapolationMode == ExtrapolationMode_Repeat && mKnots.size() > 1)
        x = fmod(x - mKnots.first().x(), mKnots.last().x() - mKnots.first().x()) + mKnots.last().x();
    else if (mFloorExtrapolationMode == ExtrapolationMode_Mirror && mKnots.size() > 1)
    {
        const double a = (mKnots.last().x() - mKnots.first().x()) * 2.;
        const double b = (x - mKnots.first().x()) / a;
        x = fabs(b - floor(b + .5)) * a + mKnots.first().x();
    }
    else
        return mKnots[0].y();
    return F(x);
}

double NearestNeighborSplineInterpolator1D::ceilExtrapolate(double x)
{
    if (mCeilExtrapolationMode == ExtrapolationMode_Constant)
        return mCeilExtrapolationValue;
    else if (mCeilExtrapolationMode == ExtrapolationMode_Repeat && mKnots.size() > 1)
        x = fmod(x - mKnots.first().x(), mKnots.last().x() - mKnots.first().x()) + mKnots.first().x();
    else if (mCeilExtrapolationMode == ExtrapolationMode_Mirror && mKnots.size() > 1)
    {
        const double a = (mKnots.last().x() - mKnots.first().x()) * 2.;
        const double b = (x - mKnots.first().x()) / a;
        x = fabs(b - floor(b + .5)) * a + mKnots.first().x();
    }
    else
        return mKnots.last().y();
    return F(x);
}

}}
