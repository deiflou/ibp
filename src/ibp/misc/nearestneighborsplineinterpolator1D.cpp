//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <math.h>

#include "nearestneighborsplineinterpolator1D.h"

namespace ibp {
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
