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

#include "basesplineinterpolator.h"
#include "math.h"

namespace anitools {
namespace misc {

BaseSplineInterpolator::BaseSplineInterpolator() :
    mFloorExtrapolationMode(Clamp),
    mCeilExtrapolationMode(Clamp)
{
}

BaseSplineInterpolator::~BaseSplineInterpolator()
{
}

const SplineInterpolatorKnots & BaseSplineInterpolator::knots()
{
    return mKnots;
}
const SplineInterpolatorKnot & BaseSplineInterpolator::knot(int i)
{
    return mKnots.at(i);
}

int BaseSplineInterpolator::size()
{
    return mKnots.size();
}

bool BaseSplineInterpolator::setKnots(const SplineInterpolatorKnots &k)
{
    // look for duplicated X
    SplineInterpolatorKnots k2 = k;
    qSort(k2.begin(), k2.end(), SplineInterpolatorKnotsLessThan);
    for (int i = 0; i < k2.size() - 1; i++)
        if (k2[i].x() == k2[i + 1].x()) return false;
    mKnots = k2;
    return true;
}

bool BaseSplineInterpolator::setKnot(int i, const SplineInterpolatorKnot &k)
{
    if (i < 0 || i >= mKnots.size()) return false;

    SplineInterpolatorKnot k2 = mKnots[i];
    mKnots.remove(i);

    if (!addKnot(k))
    {
        mKnots.insert(i, k2);
        return false;
    }

    return true;
}

bool BaseSplineInterpolator::setKnot(int i, double nx, double ny)
{
    return setKnot(i, SplineInterpolatorKnot(nx, ny));
}

bool BaseSplineInterpolator::setKnot(double x, const SplineInterpolatorKnot &k)
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

bool BaseSplineInterpolator::setKnot(double x, double nx, double ny)
{
    return setKnot(x, SplineInterpolatorKnot(nx, ny));
}

bool BaseSplineInterpolator::addKnot(const SplineInterpolatorKnot &k, bool replace)
{
    for (int i = 0; i < mKnots.size(); i++)
    {
        if (k.x() < mKnots[i].x())
        {
            mKnots.insert(i, k);
            return true;
        }
        else if (k.x() == mKnots[i].x())
        {
            if (replace)
            {
                mKnots[i].setY(k.y());
                return true;
            }
            return false;
        }
    }
    mKnots.insert(mKnots.size(), k);
    return true;
}

bool BaseSplineInterpolator::addKnot(double nx, double ny, bool replace)
{
    return addKnot(SplineInterpolatorKnot(nx, ny), replace);
}

bool BaseSplineInterpolator::removeKnot(double x)
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

bool BaseSplineInterpolator::removeKnot(int i)
{
    if (i < 0 || i >= mKnots.size()) return false;
    mKnots.remove(i);
    return true;
}

SplineInterpolator::ExtrapolationMode BaseSplineInterpolator::floorExtrapolationMode()
{
    return mFloorExtrapolationMode;
}
SplineInterpolator::ExtrapolationMode BaseSplineInterpolator::ceilExtrapolationMode()
{
    return mCeilExtrapolationMode;
}
void BaseSplineInterpolator::setExtrapolationMode(ExtrapolationMode f, ExtrapolationMode c)
{
    mFloorExtrapolationMode = f;
    mCeilExtrapolationMode = c;
}

}}
