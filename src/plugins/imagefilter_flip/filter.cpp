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

extern "C" QHash<QString, QString> getIBPPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getIBPPluginInfo();
}

QImage Filter::process(const QImage &inputImage)
{
    if (inputImage.isNull() || inputImage.format() != QImage::Format_ARGB32)
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
