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

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/types.h>

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

extern "C" QHash<QString, QString> getIBPPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getIBPPluginInfo();
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
