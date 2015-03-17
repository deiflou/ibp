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

#include <opencv2/imgproc.hpp>

#include "thresholding.h"
#include "../misc/util.h"

namespace anitools {
namespace imgproc {

void adaptiveThresholdIntegral(cv::InputArray _src, cv::OutputArray _dst, int blockSize, double k)
{
    cv::Mat src = _src.getMat();
    CV_Assert(src.type() == CV_8UC1);
    CV_Assert(blockSize % 2 == 1 && blockSize > 1);
    cv::Size size = src.size();

    _dst.create(size, src.type());
    cv::Mat dst = _dst.getMat();
    cv::Mat integral, integralSq;
    cv::integral(src, integral, integralSq, CV_32F, CV_64F);

    const int radius = blockSize >> 1;
    const double kappa = k, R = 128.;
    register unsigned char * bitsSrc, * bitsDst;
    register float * scanlineIntegralTop, * scanlineIntegralBottom;
    register double * scanlineIntegralSqTop, * scanlineIntegralSqBottom;
    register int x, y, w = src.cols, h = src.rows, rp = w - 1, bp = h - 1, x0, x1, y0, y1, n;
    register int mean, stdDev;

    for (y = 0; y < h; y++)
    {
        y0 = AT_maximum(y - radius, 0);
        y1 = AT_minimum(y + radius, bp);

        bitsSrc = (unsigned char *)src.ptr(y);
        bitsDst = (unsigned char *)dst.ptr(y);

        scanlineIntegralTop = (float *)integral.ptr(y0);
        scanlineIntegralBottom = (float *)integral.ptr(y1 + 1);
        scanlineIntegralSqTop = (double *)integralSq.ptr(y0);
        scanlineIntegralSqBottom = (double *)integralSq.ptr(y1 + 1);

        for (x = 0; x < w; x++, bitsSrc++, bitsDst++)
        {
            x0 = AT_maximum(x - radius, 0);
            x1 = AT_minimum(x + radius, rp);

            n = (x1 - x0 + 1) * (y1 - y0 + 1);

            mean = (scanlineIntegralBottom[x1 + 1] + scanlineIntegralTop[x0] -
                    scanlineIntegralBottom[x0] - scanlineIntegralTop[x1 + 1]) / n;
            stdDev = (scanlineIntegralSqBottom[x1 + 1] + scanlineIntegralSqTop[x0] -
                      scanlineIntegralSqBottom[x0] - scanlineIntegralSqTop[x1 + 1]) / n - mean * mean;

            *bitsDst = (*bitsSrc) < (mean * (1. + kappa * (sqrt(stdDev) / R - 1.))) ? 0 : 255;
        }
    }
}

} // namespace imgproc
} // namespace anitools
