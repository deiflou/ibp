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
    mAngle(_90Clockwise)
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mAngle = mAngle;
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

    QImage i;
    if (mAngle == _180)
        i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    else
        i = QImage(inputImage.height(), inputImage.width(), QImage::Format_ARGB32);
    register BGRA * bitsIn, * bitsOut;
    register int pixels;

    switch (mAngle)
    {
        case _90Clockwise:
        {
            pixels = i.width();
            for (int y = 0; y < inputImage.height(); y++)
            {
                bitsIn = (BGRA *)inputImage.scanLine(y);
                bitsOut = (BGRA *)i.bits() + pixels - y - 1;
                for (int x = 0; x < inputImage.width(); x++)
                {
                    *bitsOut = *bitsIn;
                    bitsIn++;
                    bitsOut += pixels;
                }
            }
            break;
        }
        case _90CounterClockwise:
        {
            pixels = i.width();
            for (int y = 0; y < inputImage.height(); y++)
            {
                bitsIn = (BGRA *)inputImage.scanLine(y);
                bitsOut = (BGRA *)i.scanLine(i.height() - 1) + y;
                for (int x = 0; x < inputImage.width(); x++)
                {
                    *bitsOut = *bitsIn;
                    bitsIn++;
                    bitsOut -= pixels;
                }
            }
            break;
        }
        case _180:
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
    QString angleStr;
    Angle angle;

    angleStr = s.value("angle", "90clockwise").toString();
    if (angleStr == "90clockwise")
        angle = _90Clockwise;
    else if (angleStr == "90counterclockwise")
        angle = _90CounterClockwise;
    else if (angleStr == "180")
        angle = _180;
    else
        return false;

    setAngle(angle);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("angle", mAngle == _90Clockwise ? "90clockwise" :
                        mAngle == _90CounterClockwise ? "90counterclockwise" :
                                                        "180");
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setAngle(mAngle);
    connect(this, SIGNAL(angleChanged(Filter::Angle)), fw, SLOT(setAngle(Filter::Angle)));
    connect(fw, SIGNAL(angleChanged(Filter::Angle)), this, SLOT(setAngle(Filter::Angle)));
    return fw;
}

void Filter::setAngle(Filter::Angle a)
{
    if (a == mAngle)
        return;
    mAngle = a;
    emit angleChanged(a);
    emit parametersChanged();
}
