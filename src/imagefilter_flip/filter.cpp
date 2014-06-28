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
#include "../imgproc/types.h"

Filter::Filter() :
    mDirection(Horizontal)
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mDirection = mDirection;
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

    if (mDirection < Horizontal || mDirection > Both)
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    register BGRA * bitsIn, * bitsOut;
    register int pixels;

    switch (mDirection)
    {
        case Horizontal:
        {
            pixels = i.width();
            for (int y = 0; y < i.height(); y++)
            {
                bitsIn = (BGRA *)inputImage.scanLine(y);
                bitsOut = (BGRA *)i.scanLine(y) + pixels - 1;
                for (int x = 0; x < pixels; x++)
                {
                    *bitsOut = *bitsIn;
                    bitsIn++;
                    bitsOut--;
                }
            }
            break;
        }
        case Vertical:
        {
            pixels = i.width() * 4;
            for (int y = 0; y < i.height(); y++)
            {
                bitsIn = (BGRA *)inputImage.scanLine(y);
                bitsOut = (BGRA *)i.scanLine(i.height() - y - 1);
                memcpy(bitsOut, bitsIn, pixels);
            }
            break;
        }
        case Both:
        {
            pixels = i.width() * i.height();
            bitsIn = (BGRA *)inputImage.bits();
            bitsOut = (BGRA *)i.bits() + pixels - 1;
            while (pixels--)
            {
                *bitsOut = *bitsIn;
                bitsIn++;
                bitsOut--;
            }
            break;
        }
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString directionStr;
    Direction direction;

    directionStr = s.value("direction", "horizontal").toString();
    if (directionStr == "horizontal")
        direction = Horizontal;
    else if (directionStr == "vertical")
        direction = Vertical;
    else if (directionStr == "both")
        direction = Both;
    else
        return false;

    setDirection(direction);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("direction", mDirection == Horizontal ? "horizontal" :
                            mDirection == Vertical ? "vertical" :
                                                     "both");
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setDirection(mDirection);
    connect(this, SIGNAL(directionChanged(Filter::Direction)), fw, SLOT(setDirection(Filter::Direction)));
    connect(fw, SIGNAL(directionChanged(Filter::Direction)), this, SLOT(setDirection(Filter::Direction)));
    return fw;
}

void Filter::setDirection(Filter::Direction d)
{
    if (d == mDirection)
        return;
    mDirection = d;
    emit directionChanged(d);
    emit parametersChanged();
}
