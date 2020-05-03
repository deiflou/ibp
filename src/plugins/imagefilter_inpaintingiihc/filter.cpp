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

#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/photo.hpp>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/lut.h>
#include <imgproc/types.h>
#include <imgproc/colorconversion.h>
#include <imgproc/thresholding.h>
#include <misc/util.h>

#define MAX_IMAGE_SIZE 512

Filter::Filter() :
    mNoiseReduction(.0),
    mMaskExpansion(0),
    mOutputMode(CorrectedImageMode1)
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mNoiseReduction = mNoiseReduction;
    f->mMaskExpansion = mMaskExpansion;
    f->mOutputMode = mOutputMode;
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
    register int x, y, w = inputImage.width(), h = inputImage.height();
    register HSL * bitsHSL = (HSL *)malloc(w * h * sizeof(HSL)), * bitsHSLsl;
    cv::Mat mlchannel(h, w, CV_8UC1);
    cv::Mat mlmask(h, w, CV_8UC1);
    cv::Mat mliihc(h, w, CV_8UC1);
    register unsigned char * mlchannelsl, * mlmasksl, * mliihcsl;

    convertBGRToHSL(inputImage.bits(), (unsigned char *)bitsHSL, w * h);

    // Get lightness channel
    for (y = 0; y < h; y++)
    {
        bitsHSLsl = bitsHSL + y * w;
        mlchannelsl = mlchannel.ptr(y);
        mlmasksl = mlmask.ptr(y);
        for (x = 0; x < w; x++, bitsHSLsl++, mlchannelsl++, mlmasksl++)
            *mlchannelsl = *mlmasksl = bitsHSLsl->l;
    }

    // Get original size mask
    // blur
    if (!qFuzzyIsNull(mNoiseReduction))
    {
        cv::Mat mlblurred;
        double sigmaS = (mNoiseReduction + .5) / 2.45;
        cv::ximgproc::dtFilter(mlmask, mlmask, mlblurred, sigmaS, 50);
        mlmask = mlblurred;
    }
    // adaptive threshold
    cv::Mat mlthresholded;
    adaptiveThresholdIntegral(mlmask, mlthresholded, 21, .05);
    mlmask = mlthresholded;
    // erode
    if (mMaskExpansion > 0)
    {
        cv::Mat mleroded;
        int size = mMaskExpansion * 2 + 1;
        cv::erode(mlmask, mleroded,
                  cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size)));
        mlmask = mleroded;
    }

    if (mOutputMode == Mask)
    {
        free(bitsHSL);
        i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
        register BGRA * bits = (BGRA *)i.bits();
        for (y = 0; y < h; y++)
        {
            mlmasksl = mlmask.ptr(y);
            for (x = 0; x < w; x++, bits++, mlmasksl++)
            {
                bits->r = bits->g = bits->b = 255 - (*mlmasksl);
                bits->a = 255;
            }
        }
        return i;
    }

    // Invert the mask.
    // Also check number of mask pixels. It must be less than 80%.
    // Return the input image otherwise.
    const int totalPixels = inputImage.width() * inputImage.height();
    register int maskPixelsCount = 0;
    for (y = 0; y < h; y++)
    {
        mlmasksl = mlmask.ptr(y);
        for (x = 0; x < w; x++, mlmasksl++)
        {
            *mlmasksl = 255 - (*mlmasksl);
            if ((*mlmasksl) == 255)
                maskPixelsCount++;
        }
    }
    if (maskPixelsCount * 100 / totalPixels >= 80)
    {
        free(bitsHSL);
        return inputImage;
    }

    // Resize
    if (w > MAX_IMAGE_SIZE || h > MAX_IMAGE_SIZE)
    {
        int sw, sh;
        if (w > h)
        {
            sw = MAX_IMAGE_SIZE;
            sh = h * MAX_IMAGE_SIZE / w;
        }
        else
        {
            sh = MAX_IMAGE_SIZE;
            sw = w * MAX_IMAGE_SIZE / h;
        }
        sw = sw < 1 ? 1 : sw;
        sh = sh < 1 ? 1 : sh;

        cv::Mat mresized(sh, sw, CV_8UC1);
        cv::Mat mresized2(sh, sw, CV_8UC1);
        cv::Mat mmaskresized(sh, sw, CV_8UC1);
        cv::resize(mlchannel, mresized, cv::Size(sw, sh));
        cv::resize(mlmask, mmaskresized, cv::Size(sw, sh), 0, 0, cv::INTER_NEAREST);
        cv::inpaint(mresized, mmaskresized, mresized2, 1, cv::INPAINT_TELEA);
        cv::resize(mresized2, mliihc, cv::Size(w, h));

        // Combine iihc model with original lightness image
        for (y = 0; y < h; y++)
        {
            mlchannelsl = mlchannel.ptr(y);
            mliihcsl = mliihc.ptr(y);
            mlmasksl = mlmask.ptr(y);
            for (x = 0; x < w; x++, mlchannelsl++, mliihcsl++, mlmasksl++)
                if ((*mlmasksl) == 0)
                    *mliihcsl = *mlchannelsl;
        }
    }
    else
    {
        cv::inpaint(mlchannel, mlmask, mliihc, 1, cv::INPAINT_NS);
    }

    if (mOutputMode == CorrectedImageMode1)
    {
        // Compute mean
        register int mean = 0;
        for (y = 0; y < h; y++)
        {
            mliihcsl = mliihc.ptr(y);
            for (x = 0; x < w; x++, mliihcsl++)
                mean += *mliihcsl;
        }
        mean /= w * h;

        for (y = 0; y < h; y++)
        {
            bitsHSLsl = bitsHSL + y * w;
            mlchannelsl = mliihc.ptr(y);
            for (x = 0; x < w; x++)
            {
                bitsHSLsl->l = IBP_clamp(0, lut02[bitsHSLsl->l][IBP_clamp(1, *mlchannelsl, 255)] * mean / 255, 255);
                bitsHSLsl++;
                mlchannelsl++;
            }
        }
    }
    else if (mOutputMode == CorrectedImageMode2)
    {
        for (y = 0; y < h; y++)
        {
            bitsHSLsl = bitsHSL + y * w;
            mlchannelsl = mliihc.ptr(y);
            for (x = 0; x < w; x++)
            {
                bitsHSLsl->l = IBP_clamp(0, lut02[bitsHSLsl->l][IBP_clamp(1, *mlchannelsl, 255)], 255);
                bitsHSLsl++;
                mlchannelsl++;
            }
        }
    }
    else
    {
        for (y = 0; y < h; y++)
        {
            bitsHSLsl = bitsHSL + y * w;
            mlchannelsl = mliihc.ptr(y);
            for (x = 0; x < w; x++)
            {
                bitsHSLsl->h = bitsHSLsl->s = 0;
                bitsHSLsl->l = *mlchannelsl;
                bitsHSLsl++;
                mlchannelsl++;
            }
        }
    }

    i = inputImage.copy();
    convertHSLToBGR((const unsigned char *)bitsHSL, i.bits(), w * h);
    free(bitsHSL);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    double noiseReduction;
    int maskExpansion;
    QString outputModeStr;
    OutputMode outputMode;
    bool ok;

    noiseReduction = s.value("noisereduction", 0.).toDouble(&ok);
    if (!ok || noiseReduction > 100.)
        return false;

    maskExpansion = s.value("maskexpansion", 0).toUInt(&ok);
    if (!ok || maskExpansion > 100)
        return false;

    outputModeStr = s.value("outputmode", "correctedimage").toString();
    if (outputModeStr == "correctedimagemode1")
        outputMode = CorrectedImageMode1;
    else if (outputModeStr == "correctedimagemode2")
        outputMode = CorrectedImageMode2;
    else if (outputModeStr == "mask")
        outputMode = Mask;
    else if (outputModeStr == "iihcorrectionmodel")
        outputMode = IIHCorrectionModel;
    else
        return false;

    setNoiseReduction(noiseReduction);
    setMaskExpansion(maskExpansion);
    setOutputMode(outputMode);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("noisereduction", mNoiseReduction);
    s.setValue("maskexpansion", mMaskExpansion);
    s.setValue("outputmode", mOutputMode == CorrectedImageMode1 ? "correctedimagemode1" :
                             mOutputMode == CorrectedImageMode2 ? "correctedimagemode2" :
                             mOutputMode == Mask ? "mask" : "iihcorrectionmodel");
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setNoiseReduction(mNoiseReduction);
    fw->setMaskExpansion(mMaskExpansion);
    fw->setOutputMode(mOutputMode);
    connect(this, SIGNAL(noiseReductionChanged(double)), fw, SLOT(setNoiseReduction(double)));
    connect(this, SIGNAL(maskExpansionChanged(int)), fw, SLOT(setMaskExpansion(int)));
    connect(this, SIGNAL(outputModeChanged(Filter::OutputMode)), fw, SLOT(setOutputMode(Filter::OutputMode)));
    connect(fw, SIGNAL(noiseReductionChanged(double)), this, SLOT(setNoiseReduction(double)));
    connect(fw, SIGNAL(maskExpansionChanged(int)), this, SLOT(setMaskExpansion(int)));
    connect(fw, SIGNAL(outputModeChanged(Filter::OutputMode)), this, SLOT(setOutputMode(Filter::OutputMode)));
    return fw;
}

void Filter::setNoiseReduction(double v)
{
    if (v == mNoiseReduction)
        return;
    mNoiseReduction = v;
    emit noiseReductionChanged(v);
    emit parametersChanged();
}

void Filter::setMaskExpansion(int v)
{
    if (v == mMaskExpansion)
        return;
    mMaskExpansion = v;
    emit maskExpansionChanged(v);
    emit parametersChanged();
}

void Filter::setOutputMode(Filter::OutputMode v)
{
    if (v == mOutputMode)
        return;
    mOutputMode = v;
    emit outputModeChanged(v);
    emit parametersChanged();
}
