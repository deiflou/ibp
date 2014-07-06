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

#include "cubicsplineinterpolator.h"
#include <QDebug>

namespace anitools {
namespace misc {

CubicSplineInterpolator::CubicSplineInterpolator() :
    BaseSplineInterpolator(),
    mDirty(true)
{
}

double CubicSplineInterpolator::f(double x)
{
    if (mKnots.size() < 1) return 0.0;

    if (mDirty) calculateCoeficients();

    int piece = getPiece(x);
    if (piece == -1) return floorExtrapolate(x);
    if (piece == -2) return ceilExtrapolate(x);

    double t = (x - mKnots[piece].x()) / u[piece];
    return t * mKnots[piece + 1].y() + (1.0 - t) * mKnots[piece].y() +
            u[piece] * u[piece] * (F(t) * p[piece + 1] + F(1 - t) * p[piece]) / 6.0;
}

// this returns -1 is X is lower than knot[0].x and n if X is higher than knot[n].x
int CubicSplineInterpolator::getPiece(double x)
{
    for (int i = 0; i < mKnots.size(); i++)
        if (x < mKnots[i].x()) return i - 1;
    return -2;
}

double CubicSplineInterpolator::floorExtrapolate(double x)
{
    Q_UNUSED(x)
    return mKnots[0].y();
}
double CubicSplineInterpolator::ceilExtrapolate(double x)
{
    Q_UNUSED(x)
    return mKnots[mKnots.size() - 1].y();
}

void CubicSplineInterpolator::calculateCoeficients()
{
    if (mKnots.size() < 1) return;

    QVector<double> d, w;
    int i;

    p.clear();
    u.clear();
    d.resize(mKnots.size());
    w.resize(mKnots.size());
    p.resize(mKnots.size());
    u.resize(mKnots.size());

    for (i = 1; i < mKnots.size() - 1; i++)
        d[i] = 2.0 * (mKnots[i + 1].x() - mKnots[i - 1].x());

    for (i = 0; i < mKnots.size() - 1; i++)
        u[i] = mKnots[i + 1].x() - mKnots[i].x();

    for (i = 1; i < mKnots.size() - 1; i++)
        w[i] = 6.0 * ((mKnots[i + 1].y() - mKnots[i].y()) / u[i] - (mKnots[i].y() - mKnots[i - 1].y()) / u[i - 1]);

    for (i = 1; i < mKnots.size() - 2; i++)
    {
        w[i + 1] = w[i + 1] - w[i] * u[i] / d[i];
        d[i + 1] = d[i + 1] - u[i] * u[i] / d[i];
    }

    p[0] = 0.0;

    for (i = mKnots.size() - 2; i > 0; i--)
        p[i] = (w[i] - u[i] * p[i + 1]) / d[i];


    p[mKnots.size() - 1] = 0.0;

    mDirty = false;
}

bool CubicSplineInterpolator::setKnots(const SplineInterpolatorKnots &k)
{
    bool b = BaseSplineInterpolator::setKnots(k);
    if (b) mDirty = true;
    return b;
}

bool CubicSplineInterpolator::setKnot(int i, const SplineInterpolatorKnot &k)
{
    bool b = BaseSplineInterpolator::setKnot(i, k);
    if (b) mDirty = true;
    return b;
}

bool CubicSplineInterpolator::setKnot(int i, double nx, double ny)
{
    bool b = BaseSplineInterpolator::setKnot(i, nx, ny);
    if (b) mDirty = true;
    return b;
}

bool CubicSplineInterpolator::setKnot(double x, const SplineInterpolatorKnot &k)
{
    bool b = BaseSplineInterpolator::setKnot(x, k);
    if (b) mDirty = true;
    return b;
}

bool CubicSplineInterpolator::setKnot(double x, double nx, double ny)
{
    bool b = BaseSplineInterpolator::setKnot(x, nx, ny);
    if (b) mDirty = true;
    return b;
}

bool CubicSplineInterpolator::addKnot(const SplineInterpolatorKnot &k, bool replace)
{
    bool b = BaseSplineInterpolator::addKnot(k, replace);
    if (b) mDirty = true;
    return b;
}

bool CubicSplineInterpolator::addKnot(double nx, double ny, bool replace)
{
    bool b = BaseSplineInterpolator::addKnot(nx, ny, replace);
    if (b) mDirty = true;
    return b;
}

bool CubicSplineInterpolator::removeKnot(double x)
{
    bool b = BaseSplineInterpolator::removeKnot(x);
    if (b) mDirty = true;
    return b;
}

bool CubicSplineInterpolator::removeKnot(int i)
{
    bool b = BaseSplineInterpolator::removeKnot(i);
    if (b) mDirty = true;
    return b;
}

}}
