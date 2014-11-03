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

#include "basesplineinterpolator1D.h"
#include "math.h"

namespace anitools {
namespace misc {

BaseSplineInterpolator1D::BaseSplineInterpolator1D() :
    mFloorExtrapolationMode(ExtrapolationMode_Clamp),
    mCeilExtrapolationMode(ExtrapolationMode_Clamp),
    mFloorExtrapolationValue(0.),
    mCeilExtrapolationValue(0.)
{
}

BaseSplineInterpolator1D::~BaseSplineInterpolator1D()
{
}

const Interpolator1DKnots & BaseSplineInterpolator1D::knots() const
{
    return mKnots;
}
const Interpolator1DKnot & BaseSplineInterpolator1D::knot(int i) const
{
    return mKnots.at(i);
}

int BaseSplineInterpolator1D::size() const
{
    return mKnots.size();
}

bool BaseSplineInterpolator1D::setKnots(const Interpolator1DKnots &k)
{
    // look for duplicated X
    Interpolator1DKnots k2 = k;
    qSort(k2.begin(), k2.end(), Interpolator1DKnotsLessThan);
    for (int i = 0; i < k2.size() - 1; i++)
        if (k2[i].x() == k2[i + 1].x()) return false;
    mKnots = k2;
    return true;
}

bool BaseSplineInterpolator1D::setKnot(int i, const Interpolator1DKnot &k)
{
    if (i < 0 || i >= mKnots.size()) return false;

    Interpolator1DKnot k2 = mKnots[i];
    mKnots.remove(i);

    if (!addKnot(k))
    {
        mKnots.insert(i, k2);
        return false;
    }

    return true;
}

bool BaseSplineInterpolator1D::setKnot(int i, double nx, double ny)
{
    return setKnot(i, Interpolator1DKnot(nx, ny));
}

bool BaseSplineInterpolator1D::setKnot(double x, const Interpolator1DKnot &k)
{
    for (int i = 0; i < mKnots.size(); i++)
    {
        if (x == mKnots[i].x())
        {
            return setKnot(i, k);
        }
    }
    return false;
}

bool BaseSplineInterpolator1D::setKnot(double x, double nx, double ny)
{
    return setKnot(x, Interpolator1DKnot(nx, ny));
}

bool BaseSplineInterpolator1D::addKnot(const Interpolator1DKnot &k, bool replace, int * index)
{
    for (int i = 0; i < mKnots.size(); i++)
    {
        if (k.x() < mKnots[i].x())
        {
            mKnots.insert(i, k);
            if (index)
                *index = i;
            return true;
        }
        else if (k.x() == mKnots[i].x())
        {
            if (replace)
            {
                mKnots[i].setY(k.y());
                if (index)
                    *index = i;
                return true;
            }
            return false;
        }
    }
    mKnots.insert(mKnots.size(), k);
    if (index)
        *index = mKnots.size() - 1;
    return true;
}

bool BaseSplineInterpolator1D::addKnot(double nx, double ny, bool replace, int * index)
{
    return addKnot(Interpolator1DKnot(nx, ny), replace, index);
}

bool BaseSplineInterpolator1D::removeKnot(double x)
{
    for (int i = 0; i < mKnots.size(); i++)
    {
        if (mKnots[i].x() == x)
        {
            return removeKnot(i);
        }
    }
    return false;
}

bool BaseSplineInterpolator1D::removeKnot(int i)
{
    if (i < 0 || i >= mKnots.size()) return false;
    mKnots.remove(i);
    return true;
}

double BaseSplineInterpolator1D::f(double x)
{
    if (mKnots.size() < 1) return 0.0;

    if (x < mKnots.first().x())
        return floorExtrapolate(x);
    else if (x > mKnots.last().x())
        return ceilExtrapolate(x);

    return F(x);
}

Interpolator1D::ExtrapolationMode BaseSplineInterpolator1D::floorExtrapolationMode() const
{
    return mFloorExtrapolationMode;
}
Interpolator1D::ExtrapolationMode BaseSplineInterpolator1D::ceilExtrapolationMode() const
{
    return mCeilExtrapolationMode;
}
double BaseSplineInterpolator1D::floorExtrapolationValue() const
{
    return mFloorExtrapolationValue;
}
double BaseSplineInterpolator1D::ceilExtrapolationValue() const
{
    return mCeilExtrapolationValue;
}
void BaseSplineInterpolator1D::setExtrapolationMode(ExtrapolationMode f, ExtrapolationMode c, double fv, double cv)
{
    mFloorExtrapolationMode = f;
    mCeilExtrapolationMode = c;
    mFloorExtrapolationValue = fv;
    mCeilExtrapolationValue = cv;
}

int BaseSplineInterpolator1D::pieceForValue(double x) const
{
    int min = 0, max = mKnots.size() - 1, mid;
    while (max > min + 1)
    {
        mid = (min + max) >> 1;
        if (mKnots[mid].x() < x)
            min = mid;
        else
            max = mid;
    }
    return min;
}

}}
