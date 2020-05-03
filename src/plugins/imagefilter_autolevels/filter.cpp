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
#include <QDebug>
#include "filter.h"
#include "filterwidget.h"
#include <imgproc/types.h>
#include <imgproc/imagehistogram.h>
#include <imgproc/intensitymapping.h>
#include <misc/util.h>

Filter::Filter() :
    mEnhanceChannelsSeparately(false),
    mAdjustMidtones(false),
    mTargetColorShadows(0, 0, 0),
    mTargetColorMidtones(128, 128, 128),
    mTargetColorHighlights(255, 255, 255),
    mClippingShadows(.1),
    mClippingHighlights(.1)
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mEnhanceChannelsSeparately = mEnhanceChannelsSeparately;
    f->mAdjustMidtones = mAdjustMidtones;
    f->mTargetColorShadows = mTargetColorShadows;
    f->mTargetColorMidtones = mTargetColorMidtones;
    f->mTargetColorHighlights = mTargetColorHighlights;
    f->mClippingShadows = mClippingShadows;
    f->mClippingHighlights = mClippingHighlights;
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

    ImageHistogram histogram;
    double inputBlackPoint[3], inputWhitePoint[3], inputGamma[3], outputBlackPoint[3], outputWhitePoint[3];
    unsigned char mLuts[3][256];
    double clippingShadows = mClippingShadows / 100., clippingHighlights = mClippingHighlights / 100.;
    double mean;

    if (mEnhanceChannelsSeparately)
    {
        histogram.computeHistogram(inputImage.bits(), inputImage.width(), inputImage.height(), ImageHistogram::RGB,
                                   inputImage.bytesPerLine());

        for (int i = 0; i < 3; i++)
        {
            const ImageHistogram::Channel channel = (ImageHistogram::Channel)(1 << i);

            if (qFuzzyIsNull(clippingShadows))
                inputBlackPoint[i] = histogram.limit(channel, ProbabilityMassFunction::A);
            else
                inputBlackPoint[i] = histogram.integrate2(channel, clippingShadows);

            if (qFuzzyIsNull(clippingHighlights))
                inputWhitePoint[i] = histogram.limit(channel, ProbabilityMassFunction::B);
            else
                inputWhitePoint[i] = histogram.integrate2(channel, -clippingHighlights);

            if (mAdjustMidtones)
            {
                mean = (histogram.mean(channel) - inputBlackPoint[i]) /
                       (inputWhitePoint[i] - inputBlackPoint[i]);
                inputGamma[i] = IBP_clamp(.1, 1. /
                                         (log(i == 0 ? mTargetColorMidtones.blueF() :
                                             i == 1 ? mTargetColorMidtones.greenF() : mTargetColorMidtones.redF()) /
                                         log(mean)),
                                         10.);
            }
            else
                inputGamma[i] = 1.;

            inputBlackPoint[i] /= 255.;
            inputWhitePoint[i] /= 255.;
        }
    }
    else
    {
        histogram.computeHistogram(inputImage.bits(), inputImage.width(), inputImage.height(), ImageHistogram::Luma,
                                   inputImage.bytesPerLine());

        if (qFuzzyIsNull(clippingShadows))
            inputBlackPoint[0] = inputBlackPoint[1] = inputBlackPoint[2] =
                    histogram.limit(ImageHistogram::Luma, ProbabilityMassFunction::A);
        else
            inputBlackPoint[0] = inputBlackPoint[1] = inputBlackPoint[2] =
                    histogram.integrate2(ImageHistogram::Luma, clippingShadows);

        if (qFuzzyIsNull(clippingHighlights))
            inputWhitePoint[0] = inputWhitePoint[1] = inputWhitePoint[2] =
                    histogram.limit(ImageHistogram::Luma, ProbabilityMassFunction::B);
        else
            inputWhitePoint[0] = inputWhitePoint[1] = inputWhitePoint[2] =
                    histogram.integrate2(ImageHistogram::Luma, -clippingHighlights);

        if (mAdjustMidtones)
        {
            mean = (histogram.mean(ImageHistogram::Luma) - inputBlackPoint[0]) /
                   (inputWhitePoint[0] - inputBlackPoint[0]);
            for (int i = 0; i < 3; i++)
            {
                inputGamma[i] = IBP_clamp(.1, 1. /
                                         (log(i == 0 ? mTargetColorMidtones.blueF() :
                                             i == 1 ? mTargetColorMidtones.greenF() : mTargetColorMidtones.redF()) /
                                         log(mean)),
                                         10.);
            }
        }
        else
            inputGamma[0] = inputGamma[1] = inputGamma[2] = 1.;

        inputBlackPoint[0] = inputBlackPoint[1] = inputBlackPoint[2] = inputBlackPoint[0] / 255.;
        inputWhitePoint[0] = inputWhitePoint[1] = inputWhitePoint[2] = inputWhitePoint[0] / 255.;
    }

    outputBlackPoint[0] = mTargetColorShadows.blueF();
    outputBlackPoint[1] = mTargetColorShadows.greenF();
    outputBlackPoint[2] = mTargetColorShadows.redF();
    outputWhitePoint[0] = mTargetColorHighlights.blueF();
    outputWhitePoint[1] = mTargetColorHighlights.greenF();
    outputWhitePoint[2] = mTargetColorHighlights.redF();

    generateLevelsLUT(mLuts[0], inputGamma[0], inputBlackPoint[0], inputWhitePoint[0],
                      outputBlackPoint[0], outputWhitePoint[0]);
    generateLevelsLUT(mLuts[1], inputGamma[1], inputBlackPoint[1], inputWhitePoint[1],
                      outputBlackPoint[1], outputWhitePoint[1]);
    generateLevelsLUT(mLuts[2], inputGamma[2], inputBlackPoint[2], inputWhitePoint[2],
                      outputBlackPoint[2], outputWhitePoint[2]);

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);

    register BGRA * bits = (BGRA*)inputImage.bits();
    register BGRA * bits2 = (BGRA*)i.bits();
    register int totalPixels = i.width() * i.height();

    while (totalPixels--)
    {
        bits2->b = mLuts[0][bits->b];
        bits2->g = mLuts[1][bits->g];
        bits2->r = mLuts[2][bits->r];
        bits2->a = bits->a;
        bits++;
        bits2++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    bool enhanceChannelsSeparately, adjustMidtones;
    QColor targetColorShadows, targetColorMidtones, targetColorHighlights;
    double clippingShadows, clippingHighlights;
    bool ok;

    enhanceChannelsSeparately = s.value("enhancechannelsseparately", false).toBool();

    adjustMidtones = s.value("adjustmidtones", false).toBool();

    targetColorShadows = QColor(s.value("targetcolorshadows", "#000000").toString());
    if (!targetColorShadows.isValid())
        return false;
    targetColorShadows.setAlpha(255);

    targetColorMidtones = QColor(s.value("targetcolormidtones", "#808080").toString());
    if (!targetColorMidtones.isValid())
        return false;
    targetColorMidtones.setAlpha(255);

    targetColorHighlights = QColor(s.value("targetcolorhighlights", "#FFFFFF").toString());
    if (!targetColorHighlights.isValid())
        return false;
    targetColorHighlights.setAlpha(255);

    clippingShadows = s.value("clippingshadows", .1).toDouble(&ok);
    if (!ok)
        return false;

    clippingHighlights = s.value("clippinghighlights", .1).toDouble(&ok);
    if (!ok)
        return false;

    setEnhanceChannelsSeparately(enhanceChannelsSeparately);
    setAdjustMidtones(adjustMidtones);
    setTargetColorShadows(targetColorShadows);
    setTargetColorMidtones(targetColorMidtones);
    setTargetColorHighlights(targetColorHighlights);
    setClippingShadows(clippingShadows);
    setClippingHighlights(clippingHighlights);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("enhancechannelsseparately", mEnhanceChannelsSeparately);
    s.setValue("adjustmidtones", mAdjustMidtones);
    s.setValue("targetcolorshadows", mTargetColorShadows.name(QColor::HexRgb));
    s.setValue("targetcolormidtones", mTargetColorMidtones.name(QColor::HexRgb));
    s.setValue("targetcolorhighlights", mTargetColorHighlights.name(QColor::HexRgb));
    s.setValue("clippingshadows", mClippingShadows);
    s.setValue("clippinghighlights", mClippingHighlights);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setEnhanceChannelsSeparately(mEnhanceChannelsSeparately);
    fw->setAdjustMidtones(mAdjustMidtones);
    fw->setTargetColorShadows(mTargetColorShadows);
    fw->setTargetColorMidtones(mTargetColorMidtones);
    fw->setTargetColorHighlights(mTargetColorHighlights);
    fw->setClippingShadows(mClippingShadows);
    fw->setClippingHighlights(mClippingHighlights);
    connect(this, SIGNAL(enhanceChannelsSeparatelyChanged(bool)), fw, SLOT(setEnhanceChannelsSeparately(bool)));
    connect(this, SIGNAL(adjustMidtonesChanged(bool)), fw, SLOT(setAdjustMidtones(bool)));
    connect(this, SIGNAL(targetColorShadowsChanged(QColor)), fw, SLOT(setTargetColorShadows(QColor)));
    connect(this, SIGNAL(targetColorMidtonesChanged(QColor)), fw, SLOT(setTargetColorMidtones(QColor)));
    connect(this, SIGNAL(targetColorHighlightsChanged(QColor)), fw, SLOT(setTargetColorHighlights(QColor)));
    connect(this, SIGNAL(clippingShadowsChanged(double)), fw, SLOT(setClippingShadows(double)));
    connect(this, SIGNAL(clippingHighlightsChanged(double)), fw, SLOT(setClippingHighlights(double)));
    connect(fw, SIGNAL(enhanceChannelsSeparatelyChanged(bool)), this, SLOT(setEnhanceChannelsSeparately(bool)));
    connect(fw, SIGNAL(adjustMidtonesChanged(bool)), this, SLOT(setAdjustMidtones(bool)));
    connect(fw, SIGNAL(targetColorShadowsChanged(QColor)), this, SLOT(setTargetColorShadows(QColor)));
    connect(fw, SIGNAL(targetColorMidtonesChanged(QColor)), this, SLOT(setTargetColorMidtones(QColor)));
    connect(fw, SIGNAL(targetColorHighlightsChanged(QColor)), this, SLOT(setTargetColorHighlights(QColor)));
    connect(fw, SIGNAL(clippingShadowsChanged(double)), this, SLOT(setClippingShadows(double)));
    connect(fw, SIGNAL(clippingHighlightsChanged(double)), this, SLOT(setClippingHighlights(double)));
    return fw;
}

void Filter::setEnhanceChannelsSeparately(bool v)
{
    if (v == mEnhanceChannelsSeparately)
        return;
    mEnhanceChannelsSeparately = v;
    emit enhanceChannelsSeparatelyChanged(v);
    emit parametersChanged();
}

void Filter::setAdjustMidtones(bool v)
{
    if (v == mAdjustMidtones)
        return;
    mAdjustMidtones = v;
    emit adjustMidtonesChanged(v);
    emit parametersChanged();
}

void Filter::setTargetColorShadows(const QColor & v)
{
    if (v == mTargetColorShadows)
        return;
    mTargetColorShadows = v;
    emit targetColorShadowsChanged(v);
    emit parametersChanged();
}

void Filter::setTargetColorMidtones(const QColor & v)
{
    if (v == mTargetColorMidtones)
        return;
    mTargetColorMidtones = v;
    emit targetColorMidtonesChanged(v);
    emit parametersChanged();
}

void Filter::setTargetColorHighlights(const QColor & v)
{
    if (v == mTargetColorHighlights)
        return;
    mTargetColorHighlights = v;
    emit targetColorHighlightsChanged(v);
    emit parametersChanged();
}

void Filter::setClippingShadows(double v)
{
    if (v == mClippingShadows)
        return;
    mClippingShadows = v;
    emit clippingShadowsChanged(v);
    emit parametersChanged();
}

void Filter::setClippingHighlights(double v)
{
    if (v == mClippingHighlights)
        return;
    mClippingHighlights = v;
    emit clippingHighlightsChanged(v);
    emit parametersChanged();
}

