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

#ifndef ANITOOLS_IMGPROC_FREEIMAGE_H
#define ANITOOLS_IMGPROC_FREEIMAGE_H

#include <QString>
#include <QStringList>
#include <QImage>

#include <FreeImage.h>

namespace anitools {
namespace imgproc {

QImage freeimageLoadAs32Bits(const QString & fileName, bool makeCopy = true, FIBITMAP ** out = 0);
bool freeimageSave32Bits(const QImage & image, const QString & fileName, FREE_IMAGE_FORMAT format, int flags = 0);
bool freeimageSave32Bits(const QImage & image, const QString & fileName, const QString & filter, int flags = 0);
QString freeimageGetOpenFilterString();
QStringList freeimageGetOpenFilterExtensions(bool preppendWilcards = false);
QString freeimageGetSaveFilterString();
FREE_IMAGE_FORMAT freeimageGetFormatFromFilterString(const QString & filter);

}}

#endif // ANITOOLS_IMGPROC_FREEIMAGE_H
