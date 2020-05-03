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

#include <QRegularExpression>
#include <QDebug>
#include <math.h>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/types.h>
#include <imgproc/intensitymapping.h>

Filter::Filter() :
    mWorkingChannel(Luma)
{
    for (int i = 0; i < 5; i++)
    {
        mInputLevels[i][InputBlackPoint] = 0;
        mInputLevels[i][InputGamma] = 1;
        mInputLevels[i][InputWhitePoint] = 1;
        mOutputLevels[i][OutputBlackPoint] = 0;
        mOutputLevels[i][OutputWhitePoint] = 1;
        makeLUT((WorkingChannel)i);
    }
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mWorkingChannel = mWorkingChannel;
    for (int i = 0; i < 5; i++)
    {
        f->mInputLevels[i][InputBlackPoint] = mInputLevels[i][InputBlackPoint];
        f->mInputLevels[i][InputGamma] = mInputLevels[i][InputGamma];
        f->mInputLevels[i][InputWhitePoint] = mInputLevels[i][InputWhitePoint];
        f->mOutputLevels[i][OutputBlackPoint] = mOutputLevels[i][OutputBlackPoint];
        f->mOutputLevels[i][OutputWhitePoint] = mOutputLevels[i][OutputWhitePoint];
        f->makeLUT((WorkingChannel)i);
    }
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
        bits2->r = mLuts[Luma][mLuts[Red][bits->r]];
        bits2->g = mLuts[Luma][mLuts[Green][bits->g]];
        bits2->b = mLuts[Luma][mLuts[Blue][bits->b]];
        bits2->a = mLuts[Alpha][bits->a];
        bits++;
        bits2++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString workingChannelStr;
    WorkingChannel workingChannel;
    QString levelsStr[5];
    QStringList levelsList;
    double inputLevels[5][3];
    double outputLevels[5][2];
    bool ok;

    workingChannelStr = s.value("workingchannel", 0).toString();
    if (workingChannelStr == "luma")
        workingChannel = Luma;
    else if (workingChannelStr == "red")
        workingChannel = Red;
    else if (workingChannelStr == "green")
        workingChannel = Green;
    else if (workingChannelStr == "blue")
        workingChannel = Blue;
    else if (workingChannelStr == "alpha")
        workingChannel = Alpha;
    else
        return false;

    levelsStr[Luma] = s.value("lumalevels", 0).toString();
    levelsStr[Red] = s.value("redlevels", 0).toString();
    levelsStr[Green] = s.value("greenlevels", 0).toString();
    levelsStr[Blue] = s.value("bluelevels", 0).toString();
    levelsStr[Alpha] = s.value("alphalevels", 0).toString();
    for (int i = 0; i < 5; i++)
    {
        levelsList = levelsStr[i].split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        if (levelsList.size() != 5)
            return false;
        inputLevels[i][InputBlackPoint] = levelsList.at(0).toDouble(&ok);
        if (!ok)
            return false;
        inputLevels[i][InputGamma] = levelsList.at(1).toDouble(&ok);
        if (!ok)
            return false;
        inputLevels[i][InputWhitePoint] = levelsList.at(2).toDouble(&ok);
        if (!ok)
            return false;
        outputLevels[i][OutputBlackPoint] = levelsList.at(3).toDouble(&ok);
        if (!ok)
            return false;
        outputLevels[i][OutputWhitePoint] = levelsList.at(4).toDouble(&ok);
        if (!ok)
            return false;
        if (inputLevels[i][InputBlackPoint] < 0 || inputLevels[i][InputBlackPoint] > 1 ||
            inputLevels[i][InputGamma] < 0.1 || inputLevels[i][InputGamma] > 10 ||
            inputLevels[i][InputWhitePoint] < 0 || inputLevels[i][InputWhitePoint] > 1 ||
            outputLevels[i][OutputBlackPoint] < 0 || outputLevels[i][OutputBlackPoint] > 1 ||
            outputLevels[i][OutputWhitePoint] < 0 || outputLevels[i][OutputWhitePoint] > 1)
            return false;
    }

    mWorkingChannel = workingChannel;
    for(int i = 0; i < 5; i++)
    {
        mInputLevels[i][InputBlackPoint] = inputLevels[i][InputBlackPoint];
        mInputLevels[i][InputGamma] = inputLevels[i][InputGamma];
        mInputLevels[i][InputWhitePoint] = inputLevels[i][InputWhitePoint];
        mOutputLevels[i][OutputBlackPoint] = outputLevels[i][OutputBlackPoint];
        mOutputLevels[i][OutputWhitePoint] = outputLevels[i][OutputWhitePoint];
        makeLUT((WorkingChannel)i);
    }
    emit workingChannelChanged(mWorkingChannel);
    emit inputBlackPointChanged(mInputLevels[mWorkingChannel][InputBlackPoint]);
    emit inputGammaChanged(mInputLevels[mWorkingChannel][InputGamma]);
    emit inputWhitePointChanged(mInputLevels[mWorkingChannel][InputWhitePoint]);
    emit outputBlackPointChanged(mOutputLevels[mWorkingChannel][OutputBlackPoint]);
    emit outputWhitePointChanged(mOutputLevels[mWorkingChannel][OutputWhitePoint]);
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("workingchannel", mWorkingChannel == Luma ? "luma" :
                                 mWorkingChannel == Red ? "red" :
                                 mWorkingChannel == Green ? "green" :
                                 mWorkingChannel == Blue ? "blue" :
                                 "alpha");
    QString levelsNameStr[5] = { "rgblevels", "redlevels", "greenlevels", "bluelevels", "alphalevels" };
    QString levelsStr;
    for(int i = 0; i < 5; i++)
    {
        levelsStr = QString::number(mInputLevels[i][InputBlackPoint], 'f', 2) + " " +
                    QString::number(mInputLevels[i][InputGamma], 'f', 2) + " " +
                    QString::number(mInputLevels[i][InputWhitePoint], 'f', 2) + " " +
                    QString::number(mOutputLevels[i][OutputBlackPoint], 'f', 2) + " " +
                    QString::number(mOutputLevels[i][OutputWhitePoint], 'f', 2);
        s.setValue(levelsNameStr[i], levelsStr);
    }

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setWorkingChannel(mWorkingChannel);
    fw->setInputBlackPoint(mInputLevels[mWorkingChannel][InputBlackPoint]);
    fw->setInputGamma(mInputLevels[mWorkingChannel][InputGamma]);
    fw->setInputWhitePoint(mInputLevels[mWorkingChannel][InputWhitePoint]);
    fw->setOutputBlackPoint(mOutputLevels[mWorkingChannel][OutputBlackPoint]);
    fw->setOutputWhitePoint(mOutputLevels[mWorkingChannel][OutputWhitePoint]);

    connect(this, SIGNAL(workingChannelChanged(Filter::WorkingChannel)),
            fw, SLOT(setWorkingChannel(Filter::WorkingChannel)));
    connect(this, SIGNAL(inputBlackPointChanged(double)),
            fw, SLOT(setInputBlackPoint(double)));
    connect(this, SIGNAL(inputGammaChanged(double)),
            fw, SLOT(setInputGamma(double)));
    connect(this, SIGNAL(inputWhitePointChanged(double)),
            fw, SLOT(setInputWhitePoint(double)));
    connect(this, SIGNAL(outputBlackPointChanged(double)),
            fw, SLOT(setOutputBlackPoint(double)));
    connect(this, SIGNAL(outputWhitePointChanged(double)),
            fw, SLOT(setOutputWhitePoint(double)));

    connect(fw, SIGNAL(workingChannelChanged(Filter::WorkingChannel)),
            this, SLOT(setWorkingChannel(Filter::WorkingChannel)));
    connect(fw, SIGNAL(inputBlackPointChanged(double)),
            this, SLOT(setInputBlackPoint(double)));
    connect(fw, SIGNAL(inputGammaChanged(double)),
            this, SLOT(setInputGamma(double)));
    connect(fw, SIGNAL(inputWhitePointChanged(double)),
            this, SLOT(setInputWhitePoint(double)));
    connect(fw, SIGNAL(outputBlackPointChanged(double)),
            this, SLOT(setOutputBlackPoint(double)));
    connect(fw, SIGNAL(outputWhitePointChanged(double)),
            this, SLOT(setOutputWhitePoint(double)));

    return fw;
}

void Filter::setWorkingChannel(Filter::WorkingChannel s)
{
    if (s == mWorkingChannel)
        return;
    mWorkingChannel = s;
    emit workingChannelChanged(s);
    emit inputBlackPointChanged(mInputLevels[mWorkingChannel][InputBlackPoint]);
    emit inputGammaChanged(mInputLevels[mWorkingChannel][InputGamma]);
    emit inputWhitePointChanged(mInputLevels[mWorkingChannel][InputWhitePoint]);
    emit outputBlackPointChanged(mOutputLevels[mWorkingChannel][OutputBlackPoint]);
    emit outputWhitePointChanged(mOutputLevels[mWorkingChannel][OutputWhitePoint]);
    emit parametersChanged();
}

void Filter::setInputBlackPoint(double v)
{
    if (v == mInputLevels[mWorkingChannel][InputBlackPoint])
        return;
    mInputLevels[mWorkingChannel][InputBlackPoint] = v;
    makeLUT(mWorkingChannel);
    emit inputBlackPointChanged(v);
    emit parametersChanged();
}

void Filter::setInputGamma(double v)
{
    if (v == mInputLevels[mWorkingChannel][InputGamma])
        return;
    mInputLevels[mWorkingChannel][InputGamma] = v;
    makeLUT(mWorkingChannel);
    emit inputGammaChanged(v);
    emit parametersChanged();
}

void Filter::setInputWhitePoint(double v)
{
    if (v == mInputLevels[mWorkingChannel][InputWhitePoint])
        return;
    mInputLevels[mWorkingChannel][InputWhitePoint] = v;
    makeLUT(mWorkingChannel);
    emit inputWhitePointChanged(v);
    emit parametersChanged();
}

void Filter::setOutputBlackPoint(double v)
{
    if (v == mOutputLevels[mWorkingChannel][OutputBlackPoint])
        return;
    mOutputLevels[mWorkingChannel][OutputBlackPoint] = v;
    makeLUT(mWorkingChannel);
    emit outputBlackPointChanged(v);
    emit parametersChanged();
}

void Filter::setOutputWhitePoint(double v)
{
    if (v == mOutputLevels[mWorkingChannel][OutputWhitePoint])
        return;
    mOutputLevels[mWorkingChannel][OutputWhitePoint] = v;
    makeLUT(mWorkingChannel);
    emit outputWhitePointChanged(v);
    emit parametersChanged();
}

void Filter::makeLUT(WorkingChannel c)
{
    generateLevelsLUT(mLuts[c],
                      mInputLevels[c][InputGamma], mInputLevels[c][InputBlackPoint], mInputLevels[c][InputWhitePoint],
                      mOutputLevels[c][OutputBlackPoint], mOutputLevels[c][OutputWhitePoint]);
}

