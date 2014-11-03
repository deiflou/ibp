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

#ifndef ANITOOLS_MISC_LINEARSPLINEINTERPOLATOR1D_H
#define ANITOOLS_MISC_LINEARSPLINEINTERPOLATOR1D_H

#include "basesplineinterpolator1D.h"

namespace anitools {
namespace misc {

class LinearSplineInterpolator1D : public BaseSplineInterpolator1D
{
public:
    LinearSplineInterpolator1D();
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

protected:
    double F(double x);
    double floorExtrapolate(double x);
    double ceilExtrapolate(double x);

private:
    struct Coefficients
    {
        double a, b;
    };

    bool mIsDirty;
    QVector<Coefficients> mCoefficients;

    void calculateCoefficients();
};

}}
#endif // ANITOOLS_MISC_LINEARSPLINEINTERPOLATOR1D_H
