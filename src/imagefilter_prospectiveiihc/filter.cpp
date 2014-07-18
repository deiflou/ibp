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
#include "filterwidget.h"
#include "../imgproc/lut.h"
#include "../imgproc/types.h"
#include "../misc/util.h"

Filter::Filter() :
    mImage()
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mImage = mImage;
    return f;
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

    if (mImage.isNull())
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    QImage bg = mImage.scaled(inputImage.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    register int totalPixels = inputImage.width() * inputImage.height();
    register BGRA * bits1 = (BGRA*)bg.bits(), * bits2 = (BGRA*)inputImage.bits(), *bits3 = (BGRA*)i.bits();

    while (totalPixels--)
    {
        bits3->r = AT_minimum(lut02[bits2->r][bits1->r], 255);
        bits3->g = AT_minimum(lut02[bits2->g][bits1->g], 255);
        bits3->b = AT_minimum(lut02[bits2->b][bits1->b], 255);
        bits3->a = bits2->a;
        bits1++;
        bits2++;
        bits3++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QVariant v;

    v = s.value("image", QImage());
    if (!v.isValid() || !v.canConvert<QImage>())
        return false;

    setImage(v.value<QImage>());

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("image", mImage);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);

    fw->setImage(mImage);
    connect(this, SIGNAL(imageChanged(QImage)), fw, SLOT(setImage(QImage)));
    connect(fw, SIGNAL(imageChanged(QImage)), this, SLOT(setImage(QImage)));
    return fw;
}

void Filter::setImage(const QImage &i)
{
    if (i == mImage)
        return;
    if (i.format() != QImage::Format_ARGB32)
        mImage = i.convertToFormat(QImage::Format_ARGB32);
    else
        mImage = i;
    emit imageChanged(i);
    emit parametersChanged();
}
