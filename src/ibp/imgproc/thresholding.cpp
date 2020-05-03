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

#include <opencv2/imgproc.hpp>

#include "thresholding.h"
#include "../misc/util.h"

namespace ibp {
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
        y0 = IBP_maximum(y - radius, 0);
        y1 = IBP_minimum(y + radius, bp);

        bitsSrc = (unsigned char *)src.ptr(y);
        bitsDst = (unsigned char *)dst.ptr(y);

        scanlineIntegralTop = (float *)integral.ptr(y0);
        scanlineIntegralBottom = (float *)integral.ptr(y1 + 1);
        scanlineIntegralSqTop = (double *)integralSq.ptr(y0);
        scanlineIntegralSqBottom = (double *)integralSq.ptr(y1 + 1);

        for (x = 0; x < w; x++, bitsSrc++, bitsDst++)
        {
            x0 = IBP_maximum(x - radius, 0);
            x1 = IBP_minimum(x + radius, rp);

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
} // namespace ibp
