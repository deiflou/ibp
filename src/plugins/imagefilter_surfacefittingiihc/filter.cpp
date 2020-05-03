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
#include <Eigen/Dense>
#include <limits>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/lut.h>
#include <imgproc/types.h>
#include <imgproc/colorconversion.h>
#include <misc/util.h>

#define MAX_IMAGE_SIZE 128
#define DEGREE 3
#define MIU .5
#define MIUSQR (MIU * MIU)
#define MATRIXCOLUMNS ((DEGREE + 1) * (DEGREE + 2) / 2 - 1)
#define BLURKERNELSIZE 5

Filter::Filter() :
    mOutputMode(CorrectedImageMode1)
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
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

    register int x, y, w = inputImage.width(), h = inputImage.height(), mean = 0, sw, sh, i, j;
    register HSL * bitsHSL = (HSL *)malloc(w * h * sizeof(HSL)), * bitsHSLsl;
    cv::Mat mlchannel(h, w, CV_8UC1);
    register unsigned char * mbits8;
    register double * mbits321, * mbits322;
    register double weight;

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

    // Remove noise
    cv::Mat mMatUChar(sh, sw, CV_8UC1);
    cv::GaussianBlur(mInitial, mMatUChar, cv::Size(BLURKERNELSIZE, BLURKERNELSIZE), 0);

    // Convert to float and scale
    cv::Mat mMatDouble(sh, sw, CV_64FC1);
    for (y = 0; y < sh; y++)
    {
        mbits8 = mMatUChar.ptr(y);
        mbits321 = (double *)mMatDouble.ptr(y);
        for (x = 0; x < sw; x++)
        {
            *mbits321 = (*mbits8) / 255.;
            mbits8++;
            mbits321++;
        }
    }

    // Get the gradient of the blurred image
    cv::Mat mGradientX(sh, sw, CV_64FC1);
    cv::Mat mGradientY(sh, sw, CV_64FC1);
    cv::Sobel(mMatDouble, mGradientX, -1, 1, 0);
    cv::Sobel(mMatDouble, mGradientY, -1, 0, 1);

    // Calculate weights
    register int row = 0, column = 0, totalPixels = sw * sh;
    Eigen::DiagonalMatrix<float, Eigen::Dynamic> ls_W(totalPixels * 2);
    for (y = 0; y < sh; y++)
    {
        mbits321 = (double *)mGradientX.ptr(y);
        mbits322 = (double *)mGradientY.ptr(y);
        for (x = 0; x < sw; x++)
        {
            weight = sqrt(exp(-sqrt((*mbits321) * (*mbits321) + (*mbits322) * (*mbits322)) / MIUSQR));
            ls_W.diagonal()[row] = ls_W.diagonal()[row + totalPixels] = weight;
            row++;
            mbits321++;
            mbits322++;
        }
    }

    // Surface fitting using eigen
    // First set up the matrix A and the vector b for least squares fitting with the gradient image
    row = 0;
    column = 0;
    Eigen::MatrixXf ls_A(totalPixels * 2, MATRIXCOLUMNS);
    Eigen::VectorXf ls_b(totalPixels * 2);
    Eigen::VectorXf ls_x;

    for (y = 0; y < sh; y++)
    {
        mbits321 = (double *)mGradientX.ptr(y);
        mbits322 = (double *)mGradientY.ptr(y);
        for (x = 0; x < sw; x++)
        {
            column = 0;
            // Fill matrix A with the monomial terms of the polynomial surface
            for (i = 1; i <= DEGREE; i++)
            {
                for (j = 0; j <= i; j++)
                {
                    ls_A(row, column) = (i - j) * (x == 0 ? 1 : pow(x, i - j - 1)) * (y == 0 ? 1 : pow(y, j));
                    ls_A(row + totalPixels, column) = j * (x == 0 ? 1 : pow(x, i - j)) * (y == 0 ? 1 : pow(y, j - 1));

                    column++;
                }
            }
            // Fill vector b with the image gradient values
            ls_b(row) = *mbits321;
            ls_b(row + totalPixels) = *mbits322;

            mbits321++;
            mbits322++;
            row++;
        }
    }
    // Solve...
    ls_x = (ls_W * ls_A).jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(ls_W * ls_b);

    // Set up the matrix A and the vector b for least squares fitting with the blurred image
    ls_A = Eigen::MatrixXf(totalPixels, 2);
    ls_b = Eigen::VectorXf(totalPixels);
    ls_W.diagonal().conservativeResize(totalPixels);
    register double value;
    row = 0;
    for (y = 0; y < sh; y++)
    {
        mbits321 = (double *)mMatDouble.ptr(y);
        for (x = 0; x < sw; x++)
        {
            column = 0;
            value = 0.;
            for (i = 1; i <= DEGREE; i++)
            {
                for (j = 0; j <= i; j++)
                {
                    value += ls_x(column) * (x == 0 ? 1 : pow(x, i - j)) * (y == 0 ? 1 : pow(y, j));
                    column++;
                }
            }
            ls_A(row, 0) = value;
            ls_A(row, 1) = 1;
            ls_b(row) = *mbits321;

            mbits321++;
            row++;
        }
    }
    // Solve...
    ls_x = (ls_W * ls_A).jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(ls_W * ls_b);

    // Create the IIH model image
    row = 0;
    for (y = 0; y < sh; y++)
    {
        mbits321 = (double *)mMatDouble.ptr(y);
        for (x = 0; x < sw; x++)
        {
            *mbits321 = ls_A(row, 0) * ls_x(0) + ls_x(1);

            mbits321++;
            row++;
        }
    }

    // Remap due to out of range values and scale
    double minIIH, maxIIH, minIIHc, maxIIHc;
    cv::minMaxLoc(mMatDouble, &minIIH, &maxIIH);
    minIIHc = IBP_clamp(0., minIIH, 1.);
    maxIIHc = IBP_clamp(0., maxIIH, 1.);
    for (y = 0; y < sh; y++)
    {
        mbits321 = (double *)mMatDouble.ptr(y);
        mbits8 = mMatUChar.ptr(y);
        for (x = 0; x < sw; x++)
        {
            *mbits8 = round((minIIHc + ((*mbits321) - minIIH) * (maxIIHc - minIIHc) / (maxIIH - minIIH)) * 255.);
            mean += *mbits8;
            mbits8++;
            mbits321++;
        }
    }
    mean /= totalPixels;

    // Resample
    if (w != sw || h != sh)
        cv::resize(mMatUChar, mlchannel, cv::Size(w, h), 0, 0, cv::INTER_CUBIC);
    else
        mlchannel = mMatUChar;

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
    QString outputModeStr;
    OutputMode outputMode;

    outputModeStr = s.value("outputmode", "correctedimagemode1").toString();
    if (outputModeStr == "correctedimagemode1")
        outputMode = CorrectedImageMode1;
    else if (outputModeStr == "correctedimagemode2")
        outputMode = CorrectedImageMode2;
    else if (outputModeStr == "iihcorrectionmodel")
        outputMode = IIHCorrectionModel;
    else
        return false;

    setOutputMode(outputMode);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("outputmode", mOutputMode == CorrectedImageMode1 ? "correctedimagemode1" :
                             mOutputMode == CorrectedImageMode2 ? "correctedimagemode2" : "iihcorrectionmodel");
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setOutputMode(mOutputMode);
    connect(this, SIGNAL(outputModeChanged(Filter::OutputMode)), fw, SLOT(setOutputMode(Filter::OutputMode)));
    connect(fw, SIGNAL(outputModeChanged(Filter::OutputMode)), this, SLOT(setOutputMode(Filter::OutputMode)));
    return fw;
}

void Filter::setOutputMode(Filter::OutputMode om)
{
    if (om == mOutputMode)
        return;
    mOutputMode = om;
    emit outputModeChanged(om);
    emit parametersChanged();
}
