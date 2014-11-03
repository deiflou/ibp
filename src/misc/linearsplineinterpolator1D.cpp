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

#include "linearsplineinterpolator1D.h"

namespace anitools {
namespace misc {

LinearSplineInterpolator1D::LinearSplineInterpolator1D() :
    BaseSplineInterpolator1D(),
    mIsDirty(true)
{
}

Interpolator1D * LinearSplineInterpolator1D::clone() const
{
    LinearSplineInterpolator1D * si = new LinearSplineInterpolator1D();
    if (!si)
        return 0;
    si->mKnots = mKnots;
    si->mFloorExtrapolationMode = mFloorExtrapolationMode;
    si->mCeilExtrapolationMode = mCeilExtrapolationMode;
    si->mFloorExtrapolationValue = mFloorExtrapolationValue;
    si->mCeilExtrapolationValue = mCeilExtrapolationValue;
    return si;
}

double LinearSplineInterpolator1D::F(double x)
{
    if (mIsDirty)
        calculateCoefficients();
    const int piece = pieceForValue(x);
    return mCoefficients[piece].a * (x - mKnots[piece].x()) + mCoefficients[piece].b;
}

double LinearSplineInterpolator1D::floorExtrapolate(double x)
{
    if (mFloorExtrapolationMode == ExtrapolationMode_Constant)
        return mFloorExtrapolationValue;
    else if (mFloorExtrapolationMode == ExtrapolationMode_FollowTangent && mKnots.size() > 1)
    {
        if (mIsDirty)
            calculateCoefficients();
        return mCoefficients.first().a * (x - mKnots[0].x()) + mCoefficients.first().b;
    }
    else if (mFloorExtrapolationMode == ExtrapolationMode_Repeat && mKnots.size() > 1)
        x = fmod(x - mKnots.first().x(), mKnots.last().x() - mKnots.first().x()) + mKnots.last().x();
    else if (mFloorExtrapolationMode == ExtrapolationMode_Mirror && mKnots.size() > 1)
    {
        const double a = (mKnots.last().x() - mKnots.first().x()) * 2.;
        const double b = (x - mKnots.first().x()) / a;
        x = fabs(b - floor(b + .5)) * a + mKnots.first().x();
    }
    else
        return mKnots.first().y();
    return F(x);
}

double LinearSplineInterpolator1D::ceilExtrapolate(double x)
{
    if (mCeilExtrapolationMode == ExtrapolationMode_Constant)
        return mCeilExtrapolationValue;
    else if (mCeilExtrapolationMode == ExtrapolationMode_FollowTangent && mKnots.size() > 1)
    {
        if (mIsDirty)
            calculateCoefficients();
        return mCoefficients[mKnots.size() - 2].a * (x - mKnots[mKnots.size() - 2].x()) +
               mCoefficients[mKnots.size() - 2].b;
    }
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

void LinearSplineInterpolator1D::calculateCoefficients()
{
    if (mKnots.size() < 1) return;

    register int kSize = mKnots.size();

    mCoefficients.clear();
    mCoefficients.resize(kSize > 1 ? kSize - 1 : 1);

    if (kSize == 1)
    {
        // ------------------------------------------
        // constant, the final value is extrapolated
        // from the unique knot
        // ------------------------------------------
        mCoefficients[0].a = 0.;
        mCoefficients[0].b = mKnots[0].y();
    }
    else
    {
        // ------------------------------------------
        // linear interpolation
        // ------------------------------------------
        for (int i = 0; i < kSize - 1; i++)
        {
            // compute coefficients
            mCoefficients[i].a = (mKnots[i + 1].y() - mKnots[i].y()) / (mKnots[i + 1].x() - mKnots[i].x());
            mCoefficients[i].b = mKnots[i].y();
        }
    }

    mIsDirty = false;
}

bool LinearSplineInterpolator1D::setKnots(const Interpolator1DKnots &k)
{
    bool b = BaseSplineInterpolator1D::setKnots(k);
    if (b) mIsDirty = true;
    return b;
}

bool LinearSplineInterpolator1D::setKnot(int i, const Interpolator1DKnot &k)
{
    bool b = BaseSplineInterpolator1D::setKnot(i, k);
    if (b) mIsDirty = true;
    return b;
}

bool LinearSplineInterpolator1D::setKnot(int i, double nx, double ny)
{
    bool b = BaseSplineInterpolator1D::setKnot(i, nx, ny);
    if (b) mIsDirty = true;
    return b;
}

bool LinearSplineInterpolator1D::setKnot(double x, const Interpolator1DKnot &k)
{
    bool b = BaseSplineInterpolator1D::setKnot(x, k);
    if (b) mIsDirty = true;
    return b;
}

bool LinearSplineInterpolator1D::setKnot(double x, double nx, double ny)
{
    bool b = BaseSplineInterpolator1D::setKnot(x, nx, ny);
    if (b) mIsDirty = true;
    return b;
}

bool LinearSplineInterpolator1D::addKnot(const Interpolator1DKnot &k, bool replace, int * index)
{
    bool b = BaseSplineInterpolator1D::addKnot(k, replace, index);
    if (b) mIsDirty = true;
    return b;
}

bool LinearSplineInterpolator1D::addKnot(double nx, double ny, bool replace, int * index)
{
    bool b = BaseSplineInterpolator1D::addKnot(nx, ny, replace, index);
    if (b) mIsDirty = true;
    return b;
}

bool LinearSplineInterpolator1D::removeKnot(double x)
{
    bool b = BaseSplineInterpolator1D::removeKnot(x);
    if (b) mIsDirty = true;
    return b;
}

bool LinearSplineInterpolator1D::removeKnot(int i)
{
    bool b = BaseSplineInterpolator1D::removeKnot(i);
    if (b) mIsDirty = true;
    return b;
}

}}
