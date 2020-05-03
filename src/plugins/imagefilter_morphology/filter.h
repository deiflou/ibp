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

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>
#include <opencv2/imgproc.hpp>

#include <imgproc/imagefilter.h>

using namespace ibp::imgproc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    enum MorphologyOp
    {
        Dilation,
        Erosion,
        Closing,
        Opening
    };
    enum KernelShape
    {
        Ellipse,
        Rectangle,
        Diamond,
        Octagon,
        Plus,
        Cross,
        Ring
    };

    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    bool mModifyRGB, mModifyAlpha;
    MorphologyOp mMorphologyOp;
    KernelShape mKernelShape;
    int mVRadius, mHRadius;
    bool mLockRadius;

    cv::Mat getDiamondStructuringElement(cv::Size size);
    cv::Mat getOctagonStructuringElement(cv::Size size);
    cv::Mat getCrossStructuringElement(cv::Size size);
    cv::Mat getRingStructuringElement(cv::Size size);

signals:
    void modifyRGBChanged(bool v);
    void modifyAlphaChanged(bool v);
    void morphologyOpChanged(Filter::MorphologyOp mop);
    void kernelShapeChanged(Filter::KernelShape shape);
    void vRadiusChanged(int r);
    void hRadiusChanged(int r);
    void lockRadiusChanged(bool l);

public slots:
    void setModifyRGB(bool v);
    void setModifyAlpha(bool v);
    void setMorphologyOp(Filter::MorphologyOp mop);
    void setKernelShape(Filter::KernelShape shape);
    void setVRadius(int r);
    void setHRadius(int r);
    void setLockRadius(bool l);
};

#endif // FILTER_H
