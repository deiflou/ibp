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
#include <tina/sys/sysPro.h>
#include <tina/sys/sysDef.h>
#include <tina/image/imgDef.h>
#include <tina/image/imgPro.h>
#include <tina/medical/medDef.h>
#include <tina/medical/medPro.h>
#undef E
#include <Eigen/Dense>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/lut.h>
#include <imgproc/types.h>
#include <imgproc/colorconversion.h>
#include <misc/util.h>

#define MAX_IMAGE_SIZE 256

Imrect * modified_xy_normf(Imrect *im, double constant, double sigma, double thresh)
{
    Q_UNUSED(thresh)
    Imrect *im1,*im2,*imdx,*imdy;
    Imrect *imxs,*imys,*corr;

    im1 = im_square(im);

    smooth_slopes(constant,sigma,im1,&imdx,&imdy,&corr,&imxs,&imys);
    im_free(im1);
    enf_integ(&imdx,&imdy,corr,imxs,imys);
    im_free(corr);
    im2 = im_integrate(imdx,imdy);

    im_free(imxs); im_free(imys);
    im_free(imdx);
    im_free(imdy);
    if (im2 == NULL)
    {
    error("imcalc_xy_norm:  image not in calculator ",warning);
        return(NULL);
    }

    return im2;
}

Filter::Filter() :
    mRefinement(10),
    mSmoothness(10),
    mOutputMode(CorrectedImageMode1)
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mRefinement = mRefinement;
    f->mSmoothness = mSmoothness;
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

    register int x, y, w = inputImage.width(), h = inputImage.height(), mean = 0, sw, sh;
    register HSL * bitsHSL = (HSL *)malloc(w * h * sizeof(HSL)), * bitsHSLsl;
    cv::Mat mlchannel(h, w, CV_8UC1);
    register unsigned char * mbits8;

    // Convert to HSL
    convertBGRToHSL(inputImage.bits(), (unsigned char *)bitsHSL, w * h);

    // Separate L channel
    for (y = 0; y < h; y++)
    {
        bitsHSLsl = bitsHSL + y * w;
        mbits8 = mlchannel.ptr(y);
        for (x = 0; x < w; x++)
        {
            *mbits8 = bitsHSLsl->l;
            bitsHSLsl++;
            mbits8++;
        }
    }

    // Resample
    cv::Mat mInitial;
    if (w != MAX_IMAGE_SIZE || h != MAX_IMAGE_SIZE)
    {
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
        cv::resize(mlchannel, mresized, cv::Size(sw, sh), 0, 0, cv::INTER_CUBIC);
        mInitial = mresized;
    }
    else
    {
        sw = w;
        sh = h;
        mInitial = mlchannel;
    }

    // ---------------------------
    // Tina Vision Bias Correction
    // ---------------------------

    Imrect * tvInitialImage = im_alloc(sh, sw, 0, uchar_v);
    for (y = 0; y < sh; y++)
        memcpy(((unsigned char **)tvInitialImage->data)[y], mInitial.ptr(y), sw);

    Imrect * tvBiasImage = modified_xy_normf(tvInitialImage, mRefinement, mSmoothness, 0);
    im_free(tvInitialImage);
    Imrect * tvExpImage = im_exp(tvBiasImage);
    im_free(tvBiasImage);

    float * tvImagePtr;
    Imrect * tvFinalImage = tvExpImage;
    int totalPixels = sw * sh;

    // Set up the matrix A and the vector b for least squares fitting with the initial image
    register int row = 0;
    Eigen::MatrixXf ls_A = Eigen::MatrixXf(totalPixels, 2);
    Eigen::VectorXf ls_b = Eigen::VectorXf(totalPixels);

    for (y = 0; y < sh; y++)
    {
        mbits8 = mInitial.ptr(y);
        tvImagePtr = ((float **)tvFinalImage->data)[y];
        for (x = 0; x < sw; x++)
        {
            ls_A(row, 0) = 1;
            ls_A(row, 1) = *tvImagePtr;
            ls_b(row) = *mbits8;

            tvImagePtr++;
            mbits8++;
            row++;
        }
    }
    // Solve...
    Eigen::VectorXf ls_x = (ls_A).jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(ls_b);

    for (y = 0; y < sh; y++)
    {
        tvImagePtr = ((float **)tvFinalImage->data)[y];
        for (x = 0; x < sw; x++)
        {
            *tvImagePtr = ls_x(0) + (*tvImagePtr) * ls_x(1);
            tvImagePtr++;
        }
    }

    // Remap due to out of range values and scale
    double minIIH = 10000, maxIIH = -10000, minIIHc, maxIIHc, range1, range2;

    for (y = 0; y < sh; y++)
    {
        tvImagePtr = ((float **)tvFinalImage->data)[y];
        for (x = 0; x < sw; x++)
        {
            if ((*tvImagePtr) > maxIIH)
                maxIIH = (*tvImagePtr);
            if ((*tvImagePtr) < minIIH)
                minIIH = (*tvImagePtr);
            tvImagePtr++;
        }
    }
    minIIHc = IBP_clamp(0., minIIH, 255.);
    maxIIHc = IBP_clamp(0., maxIIH, 255.);
    range1 = maxIIH - minIIH;
    range2 = maxIIHc - minIIHc;
    for (y = 0; y < sh; y++)
    {
        mbits8 = mInitial.ptr(y);
        tvImagePtr = ((float **)tvFinalImage->data)[y];
        for (x = 0; x < sw; x++)
        {
            *mbits8 = round((minIIHc + ((*tvImagePtr) - minIIH) * range2 / range1));
            mean += *mbits8;
            mbits8++;
            tvImagePtr++;
        }
    }
    mean /= totalPixels;

    im_free(tvFinalImage);

    // ----------------------------------
    // End of Tina Vision Bias Correction
    // ----------------------------------

    // Resample
    if (w != sw || h != sh)
        cv::resize(mInitial, mlchannel, cv::Size(w, h), 0, 0, cv::INTER_CUBIC);
    else
        mlchannel = mInitial;

    // Make output image
    if (mOutputMode == CorrectedImageMode1)
    {
        // Divide lightness channel
        for (y = 0; y < h; y++)
        {
            bitsHSLsl = bitsHSL + y * w;
            mbits8 = mlchannel.ptr(y);
            for (x = 0; x < w; x++)
            {
                bitsHSLsl->l = IBP_clamp(0, lut02[bitsHSLsl->l][IBP_clamp(1, *mbits8, 255)] * mean / 255, 255);
                bitsHSLsl++;
                mbits8++;
            }
        }
    }
    else if (mOutputMode == CorrectedImageMode2)
    {
        // Divide lightness channel
        for (y = 0; y < h; y++)
        {
            bitsHSLsl = bitsHSL + y * w;
            mbits8 = mlchannel.ptr(y);
            for (x = 0; x < w; x++)
            {
                bitsHSLsl->l = IBP_clamp(0, lut02[bitsHSLsl->l][IBP_clamp(1, *mbits8, 255)], 255);
                bitsHSLsl++;
                mbits8++;
            }
        }
    }
    else
    {
        for (y = 0; y < h; y++)
        {
            bitsHSLsl = bitsHSL + y * w;
            mbits8 = mlchannel.ptr(y);
            for (x = 0; x < w; x++)
            {
                bitsHSLsl->h = bitsHSLsl->s = 0;
                bitsHSLsl->l = *mbits8;
                bitsHSLsl++;
                mbits8++;
            }
        }
    }

    // Convert to RGB
    QImage finalImage = inputImage.copy();
    convertHSLToBGR((const unsigned char *)bitsHSL, finalImage.bits(), w * h);
    free(bitsHSL);

    return finalImage;
}

bool Filter::loadParameters(QSettings &s)
{
    int refinement, smoothness;
    QString outputModeStr;
    OutputMode outputMode;
    bool ok;

    refinement = s.value("refinement", 0).toUInt(&ok);
    if (!ok || refinement > 100 || refinement < 1)
        return false;

    smoothness = s.value("smoothness", 0).toUInt(&ok);
    if (!ok || smoothness > 100 || smoothness < 1)
        return false;

    outputModeStr = s.value("outputmode", "correctedimagemode1").toString();
    if (outputModeStr == "correctedimagemode1")
        outputMode = CorrectedImageMode2;
    else if (outputModeStr == "correctedimagemode2")
        outputMode = CorrectedImageMode1;
    else if (outputModeStr == "iihcorrectionmodel")
        outputMode = IIHCorrectionModel;
    else
        return false;

    setOutputMode(outputMode);
    setRefinement(refinement);
    setSmoothness(smoothness);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("refinement", mRefinement);
    s.setValue("smoothness", mSmoothness);
    s.setValue("outputmode", mOutputMode == CorrectedImageMode1 ? "correctedimagemode1" :
                             mOutputMode == CorrectedImageMode2 ? "correctedimagemode2" : "iihcorrectionmodel");
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setRefinement(mRefinement);
    fw->setSmoothness(mSmoothness);
    fw->setOutputMode(mOutputMode);
    connect(this, SIGNAL(refinementChanged(int)), fw, SLOT(setRefinement(int)));
    connect(this, SIGNAL(smoothnessChanged(int)), fw, SLOT(setSmoothness(int)));
    connect(this, SIGNAL(outputModeChanged(Filter::OutputMode)), fw, SLOT(setOutputMode(Filter::OutputMode)));
    connect(fw, SIGNAL(refinementChanged(int)), this, SLOT(setRefinement(int)));
    connect(fw, SIGNAL(smoothnessChanged(int)), this, SLOT(setSmoothness(int)));
    connect(fw, SIGNAL(outputModeChanged(Filter::OutputMode)), this, SLOT(setOutputMode(Filter::OutputMode)));
    return fw;
}

void Filter::setRefinement(int v)
{
    if (v == mRefinement)
        return;
    mRefinement = v;
    emit refinementChanged(v);
    emit parametersChanged();
}

void Filter::setSmoothness(int v)
{
    if (v == mSmoothness)
        return;
    mSmoothness = v;
    emit smoothnessChanged(v);
    emit parametersChanged();
}

void Filter::setOutputMode(Filter::OutputMode om)
{
    if (om == mOutputMode)
        return;
    mOutputMode = om;
    emit outputModeChanged(om);
    emit parametersChanged();
}
