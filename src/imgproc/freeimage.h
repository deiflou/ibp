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
