/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
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

#include "filter.h"
#include "../imgproc/types.h"

Filter::Filter()
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    return new Filter();
}

extern "C" QHash<QString, QString> getAnitoolsPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getAnitoolsPluginInfo();
}

QImage Filter::process(const QImage &inputImage)
{
    if (inputImage.isNull() || inputImage.format() != QImage::Format_ARGB32)
        return inputImage;

    QImage i = inputImage.copy();

    register BGRA * bits = (BGRA*)i.bits();
    register int totalPixels = i.width() * i.height();
    register unsigned int gR = .2126 * 0x10000;
    register unsigned int gG = .7152 * 0x10000;
    register unsigned int gB = .0722 * 0x10000;

    while (totalPixels--)
    {
        bits->r = bits->g = bits->b = (bits->r * gR >> 16) + (bits->g * gG >> 16) + (bits->b * gB >> 16);
        bits++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    Q_UNUSED(parent)
    return 0;
}
