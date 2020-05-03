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

#ifndef IBP_MISC_INTERPOLATOR1D_H
#define IBP_MISC_INTERPOLATOR1D_H

#include <QPointF>
#include <QVector>

namespace ibp {
namespace misc {

typedef QPointF Interpolator1DKnot;
typedef QVector<Interpolator1DKnot> Interpolator1DKnots;

extern bool Interpolator1DKnotsLessThan(const Interpolator1DKnot &s1, const Interpolator1DKnot &s2);

class Interpolator1D
{

public:
    enum ExtrapolationMode
    {
        ExtrapolationMode_Constant,
        ExtrapolationMode_Clamp,
        ExtrapolationMode_FollowTangent,
        ExtrapolationMode_Repeat,
        ExtrapolationMode_Mirror
    };

    virtual ~Interpolator1D() {}
    virtual Interpolator1D * clone() const = 0;

    virtual const Interpolator1DKnots & knots() const = 0;
    virtual const Interpolator1DKnot & knot(int i) const = 0;
    virtual int size() const = 0;
    virtual bool setKnots(const Interpolator1DKnots &k) = 0;
    virtual bool setKnot(int i, const Interpolator1DKnot &k) = 0;
    virtual bool setKnot(int i, double nx, double ny) = 0;
    virtual bool setKnot(double x, const Interpolator1DKnot &k) = 0;
    virtual bool setKnot(double x, double nx, double ny) = 0;
    virtual bool addKnot(const Interpolator1DKnot &k, bool replace = false, int * index = 0) = 0;
    virtual bool addKnot(double nx, double ny, bool replace = false, int * index = 0) = 0;
    virtual bool removeKnot(double x) = 0;
    virtual bool removeKnot(int i) = 0;

    virtual double f(double x) = 0;

    virtual ExtrapolationMode floorExtrapolationMode() const = 0;
    virtual ExtrapolationMode ceilExtrapolationMode() const = 0;
    virtual double floorExtrapolationValue() const = 0;
    virtual double ceilExtrapolationValue() const = 0;
    virtual void setExtrapolationMode(ExtrapolationMode f, ExtrapolationMode c, double fv = 0., double cv = 0.) = 0;
};

}}
#endif // IBP_MISC_INTERPOLATOR1D_H
