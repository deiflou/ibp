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

#ifndef IBP_MISC_UTIL_H
#define IBP_MISC_UTIL_H

#include <QPointF>
#include <QRectF>
#include <QStringList>
#include <math.h>

namespace ibp {
namespace misc {

#define IBP_PI 3.1415926536
#define IBP_2PI (IBP_PI * 2.)
#define IBP_PIOVER2 (IBP_PI / 2)
#define IBP_DegToRad(deg) ((deg) * IBP_PI / 180.)
#define IBP_RadToDeg(rad) ((rad) * 180. / IBP_PI)
#define IBP_SQRT2 1.4142135623730950488016887242097
#define IBP_1OVERSQRT2 (1. / IBP_SQRT2)
#define IBP_X_CUBE_MINUS_X(x) ((x) * (x) * (x) - (x))
#define IBP_1OVER255 (1. / 255.)
#define IBP_1OVER256 (1. / 256.)
#define IBP_minimum(a, b) ((a) < (b) ? (a) : (b))
#define IBP_minimum3(a, b, c) IBP_minimum((a), IBP_minimum((b), (c)))
#define IBP_maximum(a, b) ((a) > (b) ? (a) : (b))
#define IBP_maximum3(a, b, c) IBP_maximum((a), IBP_maximum((b), (c)))
#define IBP_round(a) ((int)(a + .5))
#define IBP_clamp(a, b, c) IBP_maximum(a, IBP_minimum(b, c))

enum Alignment
{
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    CenterCenter,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

inline double euclideanDistance(double x0, double y0, double x1, double y1)
{
    double dx = x1 - x0;
    double dy = y1 - y0;
    return sqrt(dx * dx + dy * dy);
}
inline double euclideanDistance(const QPointF & p0, const QPointF & p1)
{
    double dx = p1.x() - p0.x();
    double dy = p1.y() - p0.y();
    return sqrt(dx * dx + dy * dy);
}

inline QRect fitRectWithin(const QRect & inner, const QRect & outer, double * resizeFactor = 0)
{
    double innerAspectRatio = inner.width() / (double)inner.height();
    double outerAspectRatio = outer.width() / (double)outer.height();

    double rf = (innerAspectRatio >= outerAspectRatio) ?
                (outer.width() / (double)inner.width()) :
                (outer.height() / (double)inner.height());

    int newWidth = inner.width() * rf;
    int newHeight = inner.height() * rf;
    int newLeft = outer.left() + ((outer.width() - newWidth) >> 1);
    int newTop = outer.top() + ((outer.height() - newHeight) >> 1);

    if (resizeFactor)
        *resizeFactor = rf;
    return QRect(newLeft, newTop, newWidth, newHeight);
}

inline QRectF fitRectWithin(const QRectF & inner, const QRectF & outer, double * resizeFactor = 0)
{
    double innerAspectRatio = inner.width() / inner.height();
    double outerAspectRatio = outer.width() / outer.height();

    double rf = (innerAspectRatio >= outerAspectRatio) ?
                (outer.width() / inner.width()) :
                (outer.height() / inner.height());

    double newWidth = inner.width() * rf;
    double newHeight = inner.height() * rf;
    double newLeft = outer.left() + ((outer.width() - newWidth) / 2.0);
    double newTop = outer.top() + ((outer.height() - newHeight) / 2.0);

    if (resizeFactor)
        *resizeFactor = rf;
    return QRectF(newLeft, newTop, newWidth, newHeight);
}

inline double angleBetweenPoints(double x0, double y0, double x1, double y1)
{
    double dx = x1 - x0;
    double dy = y1 - y0;
    return atan2(dy, dx);
}
inline double angleBetweenPoints(const QPointF & p0, const QPointF & p1)
{
    double dx = p1.x() - p0.x();
    double dy = p1.y() - p0.y();
    return atan2(dy, dx);
}

}}

#endif // IBP_MISC_UTIL_H
