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

#include <math.h>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/types.h>

Filter::Filter() :
    mRedChannel(true),
    mGreenChannel(true),
    mBlueChannel(true),
    mAlphaChannel(false)
{
    makeLUTs();
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mRedChannel = mRedChannel;
    f->mGreenChannel = mGreenChannel;
    f->mBlueChannel = mBlueChannel;
    f->mAlphaChannel = mAlphaChannel;
    f->makeLUTs();
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

    register BGRA * bits = (BGRA*)inputImage.bits();
    register BGRA * bits2 = (BGRA*)i.bits();
    register int totalPixels = i.width() * i.height();

    while (totalPixels--)
    {
        bits2->r = mLuts[Red][bits->r];
        bits2->g = mLuts[Green][bits->g];
        bits2->b = mLuts[Blue][bits->b];
        bits2->a = mLuts[Alpha][bits->a];
        bits++;
        bits2++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    bool redChannel, greenChannel, blueChannel, alphaChannel;

    redChannel = s.value("redchannel", false).toBool();
    greenChannel = s.value("greenchannel", false).toBool();
    blueChannel = s.value("bluechannel", false).toBool();
    alphaChannel = s.value("alphachannel", false).toBool();

    setRedChannel(redChannel);
    setGreenChannel(greenChannel);
    setBlueChannel(blueChannel);
    setAlphaChannel(alphaChannel);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("redchannel", mRedChannel);
    s.setValue("greenchannel", mGreenChannel);
    s.setValue("bluechannel", mBlueChannel);
    s.setValue("alphachannel", mAlphaChannel);

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setRedChannel(mRedChannel);
    fw->setGreenChannel(mGreenChannel);
    fw->setBlueChannel(mBlueChannel);
    fw->setAlphaChannel(mAlphaChannel);

    connect(this, SIGNAL(redChannelToggled(bool)), fw, SLOT(setRedChannel(bool)));
    connect(this, SIGNAL(greenChannelToggled(bool)), fw, SLOT(setGreenChannel(bool)));
    connect(this, SIGNAL(blueChannelToggled(bool)), fw, SLOT(setBlueChannel(bool)));
    connect(this, SIGNAL(alphaChannelToggled(bool)), fw, SLOT(setAlphaChannel(bool)));

    connect(fw, SIGNAL(redChannelToggled(bool)), this, SLOT(setRedChannel(bool)));
    connect(fw, SIGNAL(greenChannelToggled(bool)), this, SLOT(setGreenChannel(bool)));
    connect(fw, SIGNAL(blueChannelToggled(bool)), this, SLOT(setBlueChannel(bool)));
    connect(fw, SIGNAL(alphaChannelToggled(bool)), this, SLOT(setAlphaChannel(bool)));

    return fw;
}

void Filter::setRedChannel(bool v)
{
    if (v == mRedChannel)
        return;
    mRedChannel = v;
    makeLUTs();
    emit redChannelToggled(v);
    emit parametersChanged();
}

void Filter::setGreenChannel(bool v)
{
    if (v == mGreenChannel)
        return;
    mGreenChannel = v;
    makeLUTs();
    emit greenChannelToggled(v);
    emit parametersChanged();
}

void Filter::setBlueChannel(bool v)
{
    if (v == mBlueChannel)
        return;
    mBlueChannel = v;
    makeLUTs();
    emit blueChannelToggled(v);
    emit parametersChanged();
}

void Filter::setAlphaChannel(bool v)
{
    if (v == mAlphaChannel)
        return;
    mAlphaChannel = v;
    makeLUTs();
    emit alphaChannelToggled(v);
    emit parametersChanged();
}

void Filter::makeLUTs()
{
    for (int i = 0; i < 256; i++)
    {
        mLuts[Red][i] = mRedChannel ? 255 - i : i;
        mLuts[Green][i] = mGreenChannel ? 255 - i : i;
        mLuts[Blue][i] = mBlueChannel ? 255 - i : i;
        mLuts[Alpha][i] = mAlphaChannel ? 255 - i : i;
    }
}

