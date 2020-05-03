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

#ifndef IBP_IMGPROC_THRESHOLDING_H
#define IBP_IMGPROC_THRESHOLDING_H

#include <opencv2/core.hpp>

namespace ibp {
namespace imgproc {

/*******************************************************
** Implementation of the local adaptive thresholding
** technique using the integral image:
**
** Faisal Shafait, Daniel Keysers, Thomas M. Breuel.
** “Efficient Implementation of Local Adaptive
** Thresholding Techniques Using Integral Images”,
** SPIE Document Recognition and Retrieval XV, DRR’08,
** San Jose, CA, USA. Jan. 2008
********************************************************/
void adaptiveThresholdIntegral(cv::InputArray _src, cv::OutputArray _dst, int blockSize, double k);

} // namespace imgproc
} // namespace ibp

#endif // IBP_IMGPROC_THRESHOLDING_H
