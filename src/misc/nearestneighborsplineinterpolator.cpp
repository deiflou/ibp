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

#include <math.h>

#include "nearestneighborsplineinterpolator.h"

namespace anitools {
namespace misc {

NearestNeighborSplineInterpolator::NearestNeighborSplineInterpolator() :
    BaseSplineInterpolator()
{
}

SplineInterpolator * NearestNeighborSplineInterpolator::clone() const
{
    NearestNeighborSplineInterpolator * si = new NearestNeighborSplineInterpolator();
    if (!si)
        return 0;
    si->mKnots = mKnots;
    si->mFloorExtrapolationMode = mFloorExtrapolationMode;
    si->mCeilExtrapolationMode = mCeilExtrapolationMode;
    si->mFloorExtrapolationValue = mFloorExtrapolationValue;
    si->mCeilExtrapolationValue = mCeilExtrapolationValue;
    return si;
}

double NearestNeighborSplineInterpolator::f(double x)
{
    if (mKnots.size() < 1) return 0.0;

    if (x < mKnots.first().x())
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
    }
    else if (x > mKnots.last().x())
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
    }

    int piece = getPiece(x);
    return x < (mKnots[piece].x() + mKnots[piece + 1].x()) / 2. ? mKnots[piece].y() : mKnots[piece + 1].y();
}

int NearestNeighborSplineInterpolator::getPiece(double x)
{
    for (int i = 1; i < mKnots.size(); i++)
        if (x < mKnots[i].x())
            return i - 1;
    return mKnots.size() - 2;
}

}}
