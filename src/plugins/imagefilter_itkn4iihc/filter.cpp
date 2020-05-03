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
#include <itkImage.h>
#include <itkImportImageFilter.h>
#include <itkN4BiasFieldCorrectionImageFilter.h>
#include <itkBSplineControlPointImageFilter.h>
#include <Eigen/Dense>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/lut.h>
#include <imgproc/types.h>
#include <imgproc/colorconversion.h>
#include <misc/util.h>

#define MAX_IMAGE_SIZE 128

Filter::Filter() :
    mGridSize(3),
    mOutputMode(CorrectedImageMode1)
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mGridSize = mGridSize;
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

    // -----------------------
    // ITK N4 Bias Correction
    // -----------------------

    // typedef declaration
    typedef itk::Image<float, 2> ImageType;
    typedef ImageType::Pointer ImagePointer;
    typedef itk::Image<unsigned char, 2> MaskImageType;
    typedef itk::N4BiasFieldCorrectionImageFilter<ImageType, MaskImageType, ImageType> CorrecterType;
    typedef itk::BSplineControlPointImageFilter <CorrecterType::BiasFieldControlPointLatticeType,
                                                 CorrecterType::ScalarImageType> BSplinerType;

    // initial image
    ImageType::SizeType initialImageSize;
    initialImageSize[0] = mInitial.cols;
    initialImageSize[1] = mInitial.rows;
    ImageType::IndexType initialImageStart;
    initialImageStart.Fill(0);
    ImageType::RegionType initialImageRegion;
    initialImageRegion.SetIndex(initialImageStart);
    initialImageRegion.SetSize(initialImageSize);

    ImagePointer initialImage = ImageType::New();
    initialImage->SetRegions(initialImageRegion);
    initialImage->Allocate();

    float * initialImagePtr = initialImage->GetBufferPointer();
    for (y = 0; y < sh; y++)
    {
        mbits8 = mInitial.ptr(y);
        for (x = 0; x < sw; x++)
        {
            *initialImagePtr = ((*mbits8) + 1) / 255.;
            mbits8++;
            initialImagePtr++;
        }
    }

    // correcter
    CorrecterType::Pointer correcter = CorrecterType::New();
    CorrecterType::ArrayType numberOfControlPoints;
    numberOfControlPoints.Fill(mGridSize + 1);
    correcter->SetNumberOfControlPoints(numberOfControlPoints);
    correcter->SetSplineOrder(IBP_minimum(mGridSize, 3));
    correcter->SetInput(initialImage);

    try
    {
        correcter->Update();
    }
    catch (itk::ExceptionObject &excep)
    {
        Q_UNUSED(excep)
        return inputImage;
    }

    // spline
    BSplinerType::Pointer bspliner = BSplinerType::New();
    bspliner->SetInput(correcter->GetLogBiasFieldControlPointLattice());
    bspliner->SetSplineOrder(correcter->GetSplineOrder());
    bspliner->SetSize(initialImage->GetLargestPossibleRegion().GetSize());
    bspliner->SetOrigin(initialImage->GetOrigin());
    bspliner->SetDirection(initialImage->GetDirection());
    bspliner->SetSpacing(initialImage->GetSpacing());
    bspliner->Update();

    itk::ImageRegionIterator<CorrecterType::ScalarImageType> ItB(bspliner->GetOutput(),
                                                                 bspliner->GetOutput()->GetLargestPossibleRegion());
    itk::ImageRegionIterator<ImageType> ItF(initialImage, initialImage->GetLargestPossibleRegion());
    for (ItB.GoToBegin(), ItF.GoToBegin(); !ItB.IsAtEnd(); ++ItB, ++ItF)
        ItF.Set(exp(ItB.Get()[0]));

    // ------------------------------
    // End of ITK N4 Bias Correction
    // ------------------------------

    // Set up the matrix A and the vector b for least squares fitting with the initial image
    register int row = 0, totalPixels = sw * sh;
    Eigen::MatrixXf ls_A = Eigen::MatrixXf(totalPixels, 2);
    Eigen::VectorXf ls_b = Eigen::VectorXf(totalPixels);

    initialImagePtr = initialImage->GetBufferPointer();
    for (y = 0; y < sh; y++)
    {
        mbits8 = mInitial.ptr(y);
        for (x = 0; x < sw; x++)
        {
            ls_A(row, 0) = 1;
            ls_A(row, 1) = *initialImagePtr;
            ls_b(row) = *mbits8;

            initialImagePtr++;
            mbits8++;
            row++;
        }
    }
    // Solve...
    Eigen::VectorXf ls_x = (ls_A).jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(ls_b);

    initialImagePtr = initialImage->GetBufferPointer();
    for (y = 0; y < sh; y++)
    {
        for (x = 0; x < sw; x++)
        {
            *initialImagePtr = ls_x(0) + (*initialImagePtr) * ls_x(1);
            initialImagePtr++;
        }
    }

    // Remap due to out of range values and scale
    double minIIH = 10000, maxIIH = -10000, minIIHc, maxIIHc, range1, range2;

    initialImagePtr = initialImage->GetBufferPointer();
    for (y = 0; y < sh; y++)
    {
        for (x = 0; x < sw; x++)
        {
            if ((*initialImagePtr) > maxIIH)
                maxIIH = (*initialImagePtr);
            if ((*initialImagePtr) < minIIH)
                minIIH = (*initialImagePtr);
            initialImagePtr++;
        }
    }
    minIIHc = IBP_clamp(0., minIIH, 255.);
    maxIIHc = IBP_clamp(0., maxIIH, 255.);
    range1 = maxIIH - minIIH;
    range2 = maxIIHc - minIIHc;
    initialImagePtr = initialImage->GetBufferPointer();
    for (y = 0; y < sh; y++)
    {
        mbits8 = mInitial.ptr(y);
        for (x = 0; x < sw; x++)
        {
            *mbits8 = round((minIIHc + ((*initialImagePtr) - minIIH) * range2 / range1));
            mean += *mbits8;
            mbits8++;
            initialImagePtr++;
        }
    }
    mean /= totalPixels;

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
    int gridSize;
    QString outputModeStr;
    OutputMode outputMode;
    bool ok;

    gridSize = s.value("gridsize", 3).toUInt(&ok);
    if (!ok || gridSize > 10 || gridSize < 1)
        return false;

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
    setGridSize(gridSize);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("gridsize", mGridSize);
    s.setValue("outputmode", mOutputMode == CorrectedImageMode1 ? "correctedimagemode1" :
                             mOutputMode == CorrectedImageMode2 ? "correctedimagemode2" : "iihcorrectionmodel");
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setGridSize(mGridSize);
    fw->setOutputMode(mOutputMode);
    connect(this, SIGNAL(gridSizeChanged(int)), fw, SLOT(setGridSize(int)));
    connect(this, SIGNAL(outputModeChanged(Filter::OutputMode)), fw, SLOT(setOutputMode(Filter::OutputMode)));
    connect(fw, SIGNAL(gridSizeChanged(int)), this, SLOT(setGridSize(int)));
    connect(fw, SIGNAL(outputModeChanged(Filter::OutputMode)), this, SLOT(setOutputMode(Filter::OutputMode)));
    return fw;
}

void Filter::setGridSize(int gs)
{
    if (gs == mGridSize)
        return;
    mGridSize = gs;
    emit gridSizeChanged(gs);
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
