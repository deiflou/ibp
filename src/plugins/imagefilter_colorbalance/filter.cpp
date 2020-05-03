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
#include <imgproc/colorconversion.h>
#include <imgproc/lut.h>
#include <imgproc/intensitymapping.h>
#include <misc/util.h>

Filter::Filter() :
    mShadowsRed(0),
    mShadowsGreen(0),
    mShadowsBlue(0),
    mMidtonesRed(0),
    mMidtonesGreen(0),
    mMidtonesBlue(0),
    mHighlightsRed(0),
    mHighlightsGreen(0),
    mHighlightsBlue(0),
    mPreserveLuminosity(false)
{
    makeLUTs();
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mShadowsRed = mShadowsRed;
    f->mShadowsGreen = mShadowsGreen;
    f->mShadowsBlue = mShadowsBlue;
    f->mMidtonesRed = mMidtonesRed;
    f->mMidtonesGreen = mMidtonesGreen;
    f->mMidtonesBlue = mMidtonesBlue;
    f->mHighlightsRed = mHighlightsRed;
    f->mHighlightsGreen = mHighlightsGreen;
    f->mHighlightsBlue = mHighlightsBlue;
    f->mPreserveLuminosity = mPreserveLuminosity;
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
        bits2->r = mLuts[0][bits->r];
        bits2->g = mLuts[1][bits->g];
        bits2->b = mLuts[2][bits->b];
        bits2->a = bits->a;
        bits++;
        bits2++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    int shadowsRed, shadowsGreen, shadowsBlue;
    int midtonesRed, midtonesGreen, midtonesBlue;
    int highlightsRed, highlightsGreen, highlightsBlue;
    bool preserveLuminosity;
    bool ok;

    shadowsRed = s.value("shadowsred", 0).toInt(&ok);
    if (!ok || shadowsRed < -100 || shadowsRed > 100)
        return false;
    shadowsGreen = s.value("shadowsgreen", 0).toInt(&ok);
    if (!ok || shadowsGreen < -100 || shadowsGreen > 100)
        return false;
    shadowsBlue = s.value("shadowsblue", 0).toInt(&ok);
    if (!ok || shadowsBlue < -100 || shadowsBlue > 100)
        return false;
    midtonesRed = s.value("midtonesred", 0).toInt(&ok);
    if (!ok || midtonesRed < -100 || midtonesRed > 100)
        return false;
    midtonesGreen = s.value("midtonesgreen", 0).toInt(&ok);
    if (!ok || midtonesGreen < -100 || midtonesGreen > 100)
        return false;
    midtonesBlue = s.value("midtonesblue", 0).toInt(&ok);
    if (!ok || midtonesBlue < -100 || midtonesBlue > 100)
        return false;
    highlightsRed = s.value("highlightsred", 0).toInt(&ok);
    if (!ok || highlightsRed < -100 || highlightsRed > 100)
        return false;
    highlightsGreen = s.value("highlightsgreen", 0).toInt(&ok);
    if (!ok || highlightsGreen < -100 || highlightsGreen > 100)
        return false;
    highlightsBlue = s.value("highlightsblue", 0).toInt(&ok);
    if (!ok || highlightsBlue < -100 || highlightsBlue > 100)
        return false;

    preserveLuminosity = s.value("preserveluminosity", false).toBool();

    setShadowsRed(shadowsRed);
    setShadowsGreen(shadowsGreen);
    setShadowsBlue(shadowsBlue);
    setShadowsRed(shadowsRed);
    setMidtonesGreen(midtonesGreen);
    setMidtonesBlue(midtonesBlue);
    setHighlightsRed(highlightsRed);
    setHighlightsGreen(highlightsGreen);
    setHighlightsBlue(highlightsBlue);
    setPreserveLuminosity(preserveLuminosity);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("shadowsred", mShadowsRed);
    s.setValue("shadowsgreen", mShadowsGreen);
    s.setValue("shadowsblue", mShadowsBlue);
    s.setValue("midtonesred", mMidtonesRed);
    s.setValue("midtonesgreen", mMidtonesGreen);
    s.setValue("midtonesblue", mMidtonesBlue);
    s.setValue("highlightsred", mHighlightsRed);
    s.setValue("highlightsgreen", mHighlightsGreen);
    s.setValue("highlightsblue", mHighlightsBlue);
    s.setValue("preserveluminosity", mPreserveLuminosity);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setShadowsRed(mShadowsRed);
    fw->setShadowsGreen(mShadowsGreen);
    fw->setShadowsBlue(mShadowsBlue);
    fw->setMidtonesRed(mMidtonesRed);
    fw->setMidtonesGreen(mMidtonesGreen);
    fw->setMidtonesBlue(mMidtonesBlue);
    fw->setHighlightsRed(mHighlightsRed);
    fw->setHighlightsGreen(mHighlightsGreen);
    fw->setHighlightsBlue(mHighlightsBlue);
    fw->setPreserveLuminosity(mPreserveLuminosity);

    connect(this, SIGNAL(shadowsRedChanged(int)), fw, SLOT(setShadowsRed(int)));
    connect(this, SIGNAL(shadowsGreenChanged(int)), fw, SLOT(setShadowsGreen(int)));
    connect(this, SIGNAL(shadowsBlueChanged(int)), fw, SLOT(setShadowsBlue(int)));
    connect(this, SIGNAL(midtonesRedChanged(int)), fw, SLOT(setMidtonesRed(int)));
    connect(this, SIGNAL(midtonesGreenChanged(int)), fw, SLOT(setMidtonesGreen(int)));
    connect(this, SIGNAL(midtonesBlueChanged(int)), fw, SLOT(setMidtonesBlue(int)));
    connect(this, SIGNAL(highlightsRedChanged(int)), fw, SLOT(setHighlightsRed(int)));
    connect(this, SIGNAL(highlightsGreenChanged(int)), fw, SLOT(setHighlightsGreen(int)));
    connect(this, SIGNAL(highlightsBlueChanged(int)), fw, SLOT(setHighlightsBlue(int)));
    connect(this, SIGNAL(preserveLuminosityChanged(bool)), fw, SLOT(setPreserveLuminosity(bool)));

    connect(fw, SIGNAL(shadowsRedChanged(int)), this, SLOT(setShadowsRed(int)));
    connect(fw, SIGNAL(shadowsGreenChanged(int)), this, SLOT(setShadowsGreen(int)));
    connect(fw, SIGNAL(shadowsBlueChanged(int)), this, SLOT(setShadowsBlue(int)));
    connect(fw, SIGNAL(midtonesRedChanged(int)), this, SLOT(setMidtonesRed(int)));
    connect(fw, SIGNAL(midtonesGreenChanged(int)), this, SLOT(setMidtonesGreen(int)));
    connect(fw, SIGNAL(midtonesBlueChanged(int)), this, SLOT(setMidtonesBlue(int)));
    connect(fw, SIGNAL(highlightsRedChanged(int)), this, SLOT(setHighlightsRed(int)));
    connect(fw, SIGNAL(highlightsGreenChanged(int)), this, SLOT(setHighlightsGreen(int)));
    connect(fw, SIGNAL(highlightsBlueChanged(int)), this, SLOT(setHighlightsBlue(int)));
    connect(fw, SIGNAL(preserveLuminosityChanged(bool)), this, SLOT(setPreserveLuminosity(bool)));

    return fw;
}

void Filter::setShadowsRed(int v)
{
    if (v == mShadowsRed)
        return;
    mShadowsRed = v;
    makeLUTs();
    emit shadowsRedChanged(v);
    emit parametersChanged();
}

void Filter::setShadowsGreen(int v)
{
    if (v == mShadowsGreen)
        return;
    mShadowsGreen = v;
    makeLUTs();
    emit shadowsGreenChanged(v);
    emit parametersChanged();
}

void Filter::setShadowsBlue(int v)
{
    if (v == mShadowsBlue)
        return;
    mShadowsBlue = v;
    makeLUTs();
    emit shadowsBlueChanged(v);
    emit parametersChanged();
}

void Filter::setMidtonesRed(int v)
{
    if (v == mMidtonesRed)
        return;
    mMidtonesRed = v;
    makeLUTs();
    emit midtonesRedChanged(v);
    emit parametersChanged();
}

void Filter::setMidtonesGreen(int v)
{
    if (v == mMidtonesGreen)
        return;
    mMidtonesGreen = v;
    makeLUTs();
    emit midtonesGreenChanged(v);
    emit parametersChanged();
}

void Filter::setMidtonesBlue(int v)
{
    if (v == mMidtonesBlue)
        return;
    mMidtonesBlue = v;
    makeLUTs();
    emit midtonesBlueChanged(v);
    emit parametersChanged();
}

void Filter::setHighlightsRed(int v)
{
    if (v == mHighlightsRed)
        return;
    mHighlightsRed = v;
    makeLUTs();
    emit highlightsRedChanged(v);
    emit parametersChanged();
}

void Filter::setHighlightsGreen(int v)
{
    if (v == mHighlightsGreen)
        return;
    mHighlightsGreen = v;
    makeLUTs();
    emit highlightsGreenChanged(v);
    emit parametersChanged();
}

void Filter::setHighlightsBlue(int v)
{
    if (v == mHighlightsBlue)
        return;
    mHighlightsBlue = v;
    makeLUTs();
    emit highlightsBlueChanged(v);
    emit parametersChanged();
}

void Filter::setPreserveLuminosity(bool v)
{
    if (v == mPreserveLuminosity)
        return;
    mPreserveLuminosity = v;
    emit preserveLuminosityChanged(v);
    emit parametersChanged();
}

void Filter::makeLUTs()
{
    double gamma[3] = { 1., 1., 1. };
    double inputBlackPoint[3] = { 0., 0., 0. }, inputWhitePoint[3] = { 1., 1., 1. };

    // translate the color balance parameters to levels parameters
    if (mPreserveLuminosity)
    {
        // shadows
        if (mShadowsRed < 0)
            inputBlackPoint[0] += -mShadowsRed / 255.;
        else if (mShadowsRed > 0)
        {
            inputBlackPoint[1] += mShadowsRed / 255.;
            inputBlackPoint[2] += mShadowsRed / 255.;
        }
        if (mShadowsGreen < 0)
            inputBlackPoint[1] += -mShadowsGreen / 255.;
        else if (mShadowsGreen > 0)
        {
            if (inputBlackPoint[1] > 0.)
            {
                inputBlackPoint[1] -= mShadowsGreen / 255.;
                if (inputBlackPoint[1] < 0.)
                {
                    inputBlackPoint[0] += -inputBlackPoint[1];
                    inputBlackPoint[2] += -inputBlackPoint[1];
                    inputBlackPoint[1] = 0.;
                }
            }
            else
            {
                inputBlackPoint[0] += mShadowsGreen / 255.;
                inputBlackPoint[2] += mShadowsGreen / 255.;
            }
        }
        if (mShadowsBlue < 0)
            inputBlackPoint[2] += -mShadowsBlue / 255.;
        else if (mShadowsBlue > 0)
        {
            if (inputBlackPoint[2] > 0.)
            {
                inputBlackPoint[2] -= mShadowsBlue / 255.;
                if (inputBlackPoint[2] < 0.)
                {
                    inputBlackPoint[0] += -inputBlackPoint[2];
                    inputBlackPoint[1] += -inputBlackPoint[2];
                    inputBlackPoint[2] = 0.;
                }
            }
            else
            {
                inputBlackPoint[0] += mShadowsBlue / 255.;
                inputBlackPoint[1] += mShadowsBlue / 255.;
            }
        }
        // midtones
        gamma[0] *= pow(2., mMidtonesRed / 200.);
        gamma[1] *= pow(2., -mMidtonesRed / 200.);
        gamma[2] *= pow(2., -mMidtonesRed / 200.);
        gamma[0] *= pow(2., -mMidtonesGreen / 200.);
        gamma[1] *= pow(2., mMidtonesGreen / 200.);
        gamma[2] *= pow(2., -mMidtonesGreen / 200.);
        gamma[0] *= pow(2., -mMidtonesBlue / 200.);
        gamma[1] *= pow(2., -mMidtonesBlue / 200.);
        gamma[2] *= pow(2., mMidtonesBlue / 200.);
        // highlights
        if (mHighlightsRed < 0)
        {
            inputWhitePoint[1] -= -mHighlightsRed / 255.;
            inputWhitePoint[2] -= -mHighlightsRed / 255.;
        }
        else if (mHighlightsRed > 0)
            inputWhitePoint[0] -= mHighlightsRed / 255.;
        if (mHighlightsGreen < 0)
        {
            if (inputWhitePoint[1] < 1.)
            {
                inputWhitePoint[1] += -mHighlightsGreen / 255.;
                if (inputWhitePoint[1] > 1.)
                {
                    inputWhitePoint[0] -= inputWhitePoint[1] - 1.;
                    inputWhitePoint[2] -= inputWhitePoint[1] - 1.;
                    inputWhitePoint[1] = 1.;
                }
            }
            else
            {
                inputWhitePoint[0] -= -mHighlightsGreen / 255.;
                inputWhitePoint[2] -= -mHighlightsGreen / 255.;
            }
        }
        else if (mHighlightsGreen > 0)
            inputWhitePoint[1] -= mHighlightsGreen / 255.;
        if (mHighlightsBlue < 0)
        {
            if (inputWhitePoint[2] < 1.)
            {
                inputWhitePoint[2] += -mHighlightsBlue / 255.;
                if (inputWhitePoint[2] > 1.)
                {
                    inputWhitePoint[0] -= inputWhitePoint[2] - 1.;
                    inputWhitePoint[1] -= inputWhitePoint[2] - 1.;
                    inputWhitePoint[2] = 1.;
                }
            }
            else
            {
                inputWhitePoint[0] -= -mHighlightsBlue / 255.;
                inputWhitePoint[1] -= -mHighlightsBlue / 255.;
            }
        }
        else if (mHighlightsBlue > 0)
            inputWhitePoint[2] -= mHighlightsBlue / 255.;
        // correct
        if (inputBlackPoint[0] >= inputWhitePoint[0])
            inputBlackPoint[0] = inputWhitePoint[0] - IBP_1OVER255;
        if (inputBlackPoint[1] >= inputWhitePoint[1])
            inputBlackPoint[1] = inputWhitePoint[1] - IBP_1OVER255;
        if (inputBlackPoint[2] >= inputWhitePoint[2])
            inputBlackPoint[2] = inputWhitePoint[2] - IBP_1OVER255;
    }
    else
    {
        // shadows
        if (mShadowsRed < 0)
            inputBlackPoint[0] = -mShadowsRed / 255.;
        gamma[0] *= pow(2., mShadowsRed / 200.);
        if (mShadowsGreen < 0)
            inputBlackPoint[1] = -mShadowsGreen / 255.;
        gamma[1] *= pow(2., mShadowsGreen / 200.);
        if (mShadowsBlue < 0)
            inputBlackPoint[2] = -mShadowsBlue / 255.;
        gamma[2] *= pow(2., mShadowsBlue / 200.);
        // midtones
        gamma[0] *= pow(2., mMidtonesRed / 100.);
        gamma[1] *= pow(2., mMidtonesGreen / 100.);
        gamma[2] *= pow(2., mMidtonesBlue / 100.);
        // highlights
        if (mHighlightsRed > 0)
            inputWhitePoint[0] = 1. - (mHighlightsRed / 255.);
        gamma[0] *= pow(2., mHighlightsRed / 200.);
        if (mHighlightsGreen > 0)
            inputWhitePoint[1] = 1. - (mHighlightsGreen / 255.);
        gamma[1] *= pow(2., mHighlightsGreen / 200.);
        if (mHighlightsBlue > 0)
            inputWhitePoint[2] = 1. - (mHighlightsBlue / 255.);
        gamma[2] *= pow(2., mHighlightsBlue / 200.);
    }
    // make luts
    generateLevelsLUT(mLuts[0], gamma[0], inputBlackPoint[0], inputWhitePoint[0], 0., 1.);
    generateLevelsLUT(mLuts[1], gamma[1], inputBlackPoint[1], inputWhitePoint[1], 0., 1.);
    generateLevelsLUT(mLuts[2], gamma[2], inputBlackPoint[2], inputWhitePoint[2], 0., 1.);
}
