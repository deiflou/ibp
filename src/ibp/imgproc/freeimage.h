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

#ifndef IBP_IMGPROC_FREEIMAGE_H
#define IBP_IMGPROC_FREEIMAGE_H

#include <QString>
#include <QStringList>
#include <QImage>

#include <FreeImage.h>

namespace ibp {
namespace imgproc {

QImage freeimageLoadAs32Bits(const QString & fileName, bool makeCopy = true, FIBITMAP ** out = 0);
bool freeimageSave32Bits(const QImage & image, const QString & fileName, FREE_IMAGE_FORMAT format, int flags = 0);
bool freeimageSave32Bits(const QImage & image, const QString & fileName, const QString & filter, int flags = 0);
QString freeimageGetOpenFilterString();
QStringList freeimageGetOpenFilterExtensions(bool preppendWilcards = false);
QString freeimageGetSaveFilterString();
FREE_IMAGE_FORMAT freeimageGetFormatFromFilterString(const QString & filter);

}}

#endif // IBP_IMGPROC_FREEIMAGE_H
