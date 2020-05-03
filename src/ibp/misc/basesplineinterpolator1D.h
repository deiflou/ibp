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

#ifndef IBP_MISC_BASESPLINEINTERPOLATOR1D_H
#define IBP_MISC_BASESPLINEINTERPOLATOR1D_H

#include "interpolator1D.h"

namespace ibp {
namespace misc {

class BaseSplineInterpolator1D : public Interpolator1D
{

public:
    BaseSplineInterpolator1D();
    ~BaseSplineInterpolator1D();

    virtual const Interpolator1DKnots &knots() const;
    virtual const Interpolator1DKnot &knot(int i) const;
    virtual int size() const;
    virtual bool setKnots(const Interpolator1DKnots &k);
    virtual bool setKnot(int i, const Interpolator1DKnot &k);
    virtual bool setKnot(int i, double nx, double ny);
    virtual bool setKnot(double x, const Interpolator1DKnot &k);
    virtual bool setKnot(double x, double nx, double ny);
    virtual bool addKnot(const Interpolator1DKnot &k, bool replace = false, int * index = 0);
    virtual bool addKnot(double nx, double ny, bool replace = false, int * index = 0);
    virtual bool removeKnot(double x);
    virtual bool removeKnot(int i);

    virtual double f(double x);

    virtual ExtrapolationMode floorExtrapolationMode() const;
    virtual ExtrapolationMode ceilExtrapolationMode() const;
    virtual double floorExtrapolationValue() const;
    virtual double ceilExtrapolationValue() const;
    virtual void setExtrapolationMode(ExtrapolationMode f, ExtrapolationMode c, double fv = 0., double cv = 0.);

protected:
    Interpolator1DKnots mKnots;
    ExtrapolationMode mFloorExtrapolationMode;
    ExtrapolationMode mCeilExtrapolationMode;
    double mFloorExtrapolationValue;
    double mCeilExtrapolationValue;

    virtual int pieceForValue(double x) const;
    virtual double F(double x) = 0;
    virtual double floorExtrapolate(double x) = 0;
    virtual double ceilExtrapolate(double x) = 0;
};

}}
#endif // IBP_MISC_BASESPLINEINTERPOLATOR1D_H
