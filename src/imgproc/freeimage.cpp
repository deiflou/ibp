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

#include <QStringList>
#include <QHash>
#include <QDebug>

#include "freeimage.h"
#include "lut.h"

namespace anitools {
namespace imgproc {

QImage freeimageLoadAs32Bits(const QString &fileName, bool makeCopy, FIBITMAP **out)
{
    if (!makeCopy && out == 0)
        return QImage();

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.toLocal8Bit(), 0);
    if (format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(fileName.toLocal8Bit());

    if (format == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(format))
        return QImage();

    FIBITMAP * bm = 0, * bm2 = 0;
    bm = FreeImage_Load(format, fileName.toLocal8Bit(), 0);
    if (!bm)
        return QImage();

    FREE_IMAGE_TYPE type = FreeImage_GetImageType(bm);
    if (type != FIT_BITMAP && type != FIT_RGB16 && type != FIT_RGBA16)
    {
        bm2 = FreeImage_ConvertToStandardType(bm);
        FreeImage_Unload(bm);
        if (!bm2)
            return QImage();
        bm = bm2;
        bm2 = 0;
    }
    if (FreeImage_GetBPP(bm) != 32)
    {
        bm2 = FreeImage_ConvertTo32Bits(bm);
        FreeImage_Unload(bm);
        if (!bm2)
            return QImage();
        bm = bm2;
    }

    FreeImage_FlipVertical(bm);

    QImage img;
    if (makeCopy)
    {
        img = QImage(FreeImage_GetWidth(bm), FreeImage_GetHeight(bm), QImage::Format_ARGB32);
        memcpy(img.bits(), FreeImage_GetBits(bm), img.bytesPerLine() * img.height());
        FreeImage_Unload(bm);
    }
    else
    {
        img = QImage(FreeImage_GetBits(bm), FreeImage_GetWidth(bm), FreeImage_GetHeight(bm), QImage::Format_ARGB32);
        *out = bm;
    }
    return img;
}

QString freeimageGetOpenFilterString()
{
    static QString filterString;

    if (!filterString.isEmpty())
        return filterString;

    int nFormats = FreeImage_GetFIFCount();
    QStringList filters, extList;
    QString filter, ext, allExt = "";
    for (int i = 0; i < nFormats; i++)
    {
        if (!FreeImage_FIFSupportsReading((FREE_IMAGE_FORMAT)i))
            continue;

        filter = FreeImage_GetFIFDescription((FREE_IMAGE_FORMAT)i);
        extList = QString(FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT)i)).split(',', QString::SkipEmptyParts);
        if (extList.isEmpty())
            ext = "*";
        else
            ext = "*." + extList.join(" *.");
        filter += " (" + ext + ")";
        allExt += (allExt.isEmpty() ? "" : " ") + ext;
        filters.append(filter);
    }

    filters.sort(Qt::CaseInsensitive);

    if (!allExt.isEmpty())
        filters.prepend(QObject::tr("All Supported Images") + " (" + allExt + ")");
    filters.append(QObject::tr("All Files (*)"));

    filterString = filters.join(";;");
    return filterString;
}

QString freeimageGetSaveFilterString()
{
    static QString filterString;

    if (!filterString.isEmpty())
        return filterString;

    QList<FREE_IMAGE_FORMAT> formatsList;
    formatsList << FIF_BMP << FIF_JPEG << FIF_PCX <<
                   FIF_PNG << FIF_TARGA << FIF_TIFF << FIF_PSD <<
                   FIF_RAW;

    QStringList filters, extList;
    QString filter, ext;
    FREE_IMAGE_FORMAT format;

    for (int i = 0; i < formatsList.size(); i++)
    {
        format = formatsList.at(i);
        if (!FreeImage_FIFSupportsWriting(format))
            continue;

        filter = FreeImage_GetFIFDescription(format);
        extList = QString(FreeImage_GetFIFExtensionList(format)).split(',', QString::SkipEmptyParts);
        if (extList.isEmpty())
            ext = "*";
        else
            ext = "*." + extList.join(" *.");
        filter += " (" + ext + ")";
        filters.append(filter);
    }

    filters.sort(Qt::CaseInsensitive);

    filterString = filters.join(";;");
    return filterString;
}


QStringList freeimageGetOpenFilterExtensions(bool preppendWilcards)
{
    static QStringList extensions, extensions2;

    if (preppendWilcards && !extensions2.isEmpty())
        return extensions2;
    if (!preppendWilcards && !extensions.isEmpty())
        return extensions;

    int nFormats = FreeImage_GetFIFCount();
    QString e, e2;
    for (int i = 0; i < nFormats; i++)
    {
        if (!FreeImage_FIFSupportsReading((FREE_IMAGE_FORMAT)i))
            continue;

        e = FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT)i);
        if (e.isEmpty())
            continue;
        e2 = "*." + QString(e).replace(",", ",*.");
        extensions.append(e.split(',', QString::SkipEmptyParts));
        extensions2.append(e2.split(',', QString::SkipEmptyParts));
    }

    extensions.sort(Qt::CaseInsensitive);
    extensions2.sort(Qt::CaseInsensitive);

    return preppendWilcards ? extensions2 : extensions;
}


bool freeimageSave32Bits(const QImage &image, const QString &fileName, const QString &filter, int flags)
{
    if (image.isNull() || fileName.isEmpty() || filter.isEmpty())
        return false;
    if (image.format() != QImage::Format_ARGB32)
        return false;

    FREE_IMAGE_FORMAT format = freeimageGetFormatFromFilterString(filter);
    if (format == FIF_UNKNOWN)
        return false;

    return freeimageSave32Bits(image, fileName, format, flags);
}


bool freeimageSave32Bits(const QImage &image, const QString &fileName, FREE_IMAGE_FORMAT format, int flags)
{
    if (image.isNull() || fileName.isEmpty() || !FreeImage_IsPluginEnabled(format))
        return false;
    if (image.format() != QImage::Format_ARGB32)
        return false;
    if (!FreeImage_FIFSupportsWriting(format) || !FreeImage_FIFSupportsExportType(format, FIT_BITMAP))
        return false;

    FIBITMAP * bm = 0;
    if (FreeImage_FIFSupportsExportBPP(format, 32))
    {
        bm = FreeImage_ConvertFromRawBits((BYTE*)image.bits(),
                                          image.width(), image.height(), image.bytesPerLine(), 32,
                                          0xFF0000, 0xFF00, 0xFF, 1);
        if (!bm)
            return false;
    }
    else if (FreeImage_FIFSupportsExportBPP(format, 24))
    {
        bm = FreeImage_Allocate(image.width(), image.height(), 24, 0xFF0000, 0xFF00, 0xFF);
        if (!bm)
            return false;

        register unsigned char * bitsSrc = (unsigned char *)image.bits();
        register unsigned char * bitsDst;
        register int x, y, w = image.width(), h = image.height();
        for (y = 0; y < h; y++)
        {
            bitsDst = (unsigned char *)FreeImage_GetScanLine(bm, h - 1 - y);
            for(x = 0; x < w; x++)
            {
                bitsDst[0] = bitsSrc[0] + lut01[255 - bitsSrc[0]][255 - bitsSrc[3]];
                bitsDst[1] = bitsSrc[1] + lut01[255 - bitsSrc[1]][255 - bitsSrc[3]];
                bitsDst[2] = bitsSrc[2] + lut01[255 - bitsSrc[2]][255 - bitsSrc[3]];
                bitsSrc += 4;
                bitsDst += 3;
            }
        }
    }
    else
    {
        return false;
    }

    bool ret = FreeImage_Save(format, bm, fileName.toLocal8Bit(), flags);
    FreeImage_Unload(bm);

    return ret;
}


FREE_IMAGE_FORMAT freeimageGetFormatFromFilterString(const QString &filter)
{
    static QHash<QString, FREE_IMAGE_FORMAT> formats;

    if (!formats.isEmpty())
        return formats.value(filter, FIF_UNKNOWN);

    int nFormats = FreeImage_GetFIFCount();
    QStringList extList;
    QString filter2, ext;
    for (int i = 0; i < nFormats; i++)
    {
        filter2 = FreeImage_GetFIFDescription((FREE_IMAGE_FORMAT)i);
        extList = QString(FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT)i)).split(',', QString::SkipEmptyParts);
        if (extList.isEmpty())
            ext = "*";
        else
            ext = "*." + extList.join(" *.");
        filter2 += " (" + ext + ")";
        formats.insert(filter2, (FREE_IMAGE_FORMAT)i);
    }

    formats.insert(QObject::tr("All Files (*)"), FIF_PNG);

    return formats.value(filter, FIF_UNKNOWN);
}

}}
