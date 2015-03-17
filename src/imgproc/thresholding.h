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

#ifndef ANITOOLS_IMGPROC_THRESHOLDING_H
#define ANITOOLS_IMGPROC_THRESHOLDING_H

#include <opencv2/core.hpp>

namespace anitools {
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
} // namespace anitools

#endif // ANITOOLS_IMGPROC_THRESHOLDING_H
