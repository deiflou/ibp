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

#ifndef IBP_MISC_LINEARSPLINEINTERPOLATOR1D_H
#define IBP_MISC_LINEARSPLINEINTERPOLATOR1D_H

#include "basesplineinterpolator1D.h"

namespace ibp {
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
#endif // IBP_MISC_LINEARSPLINEINTERPOLATOR1D_H
