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

/****************************************************************************
**
** The research and algorithms in part of this class where obtained from
** the following book:
**
** Numerical Methods Using Matlab, 4th Edition, 2004
** John H. Mathews and Kurtis K. Fink
** ISBN: 0-13-065248-2
**
****************************************************************************/

#ifndef ANITOOLS_MISC_CUBICSPLINEINTERPOLATOR1D_H
#define ANITOOLS_MISC_CUBICSPLINEINTERPOLATOR1D_H

#include "basesplineinterpolator1D.h"

namespace anitools {
namespace misc {

class CubicSplineInterpolator1D : public BaseSplineInterpolator1D
{
public:
    enum BoundaryConditions
    {
        // natural = relaxed curve = 2nd derivatives are 0 at end knots.
        // The natural spline is the curve obtained by forcing a flexible elastic rod
        // through the data points but letting the slope at the ends be free to equilibrate to the
        // position that minimizes the oscillatory behavior of the curve. It is useful for fitting a
        // curve to experimental data that are significant to several significant digits.
        BoundaryConditions_Natural,
        // The 2dn derivatives at the end knots are calculated from the
        // user given 1st derivatives.
        // The clamped spline involves slope at the ends. This spline can be visualized
        // as the curve obtained when a flexible elastic rod is forced to pass through the data
        // points, and the rod is clamped at each end with a fixed slope. This spline would be
        // useful to a draftsman for drawing a smooth curve through several points.
        BoundaryConditions_Fixed1stDerivatives,
        // The 2dn derivatives at the end knots are given by the user.
        // Imposing values for S''(a) and S''(b) permits the practitioner to adjust the
        // curvature at each endpoint.
        BoundaryConditions_Fixed2ndDerivatives,
        // The 2nd derivatives at the end knots are copied from the
        // 2nd derivatives of the adjacent points.
        // The assumption that S'''(x) = 0 on the interval [x0, x1] forces the cubic to
        // degenerate to a quadratic over [x0, x1], and a similar situation occurs over [xN−1, xN].
        BoundaryConditions_Copy,
        // the 2nd derivatives at the end knots are computed by linear
        // extrapolation using the 2 adjacent points.
        // the extrapolated spline is equivalent to assuming that the end cubic is an
        // extension of the adjacent cubic; that is, the spline forms a single cubic curve over the
        // interval [x0, x2] and another single cubic over the interval [xN−2, xN]
        BoundaryConditions_Extrapolate,
        // the 2nd derivatives are forced to be equal at the end knots
        BoundaryConditions_Periodic
    };

    CubicSplineInterpolator1D();
    Interpolator1D * clone() const;

    bool setKnots(const Interpolator1DKnots &k);
    bool setKnot(int i, const Interpolator1DKnot &k);
    bool setKnot(int i, double nx, double ny);
    bool setKnot(double x, const Interpolator1DKnot &k);
    bool setKnot(double x, double nx, double ny);
    bool addKnot(const Interpolator1DKnot &k, bool replace = false, int * index = 0);
    bool addKnot(double nx, double ny, bool replace = false, int * index = 0);
    bool removeKnot(double x);
    bool removeKnot(int i);

    BoundaryConditions floorBoundaryConditions() const;
    BoundaryConditions ceilBoundaryConditions() const;
    double floorBoundaryConditionsValue() const;
    double ceilBoundaryConditionsValue() const;
    void setBoundaryConditions(BoundaryConditions f, BoundaryConditions c, double fv = 0., double cv = 0.);

protected:
    double F(double x);
    double floorExtrapolate(double x);
    double ceilExtrapolate(double x);

private:
    struct Coefficients
    {
        double a, b, c, d;
    };

    BoundaryConditions mFloorBoundaryConditions;
    BoundaryConditions mCeilBoundaryConditions;
    double mFloorBoundaryConditionsValue;
    double mCeilBoundaryConditionsValue;

    bool mIsDirty;
    QVector<Coefficients> mCoefficients;

    void calculateCoefficients();
};

}}
#endif // ANITOOLS_MISC_CUBICSPLINEINTERPOLATOR1D_H
