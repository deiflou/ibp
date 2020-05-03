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

#include <opencv2/ximgproc.hpp>

#include "filter.h"
#include "filterwidget.h"
#include <imgproc/types.h>

Filter::Filter() :
    mRadius(0.0),
    mEdgePreservation(50)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mRadius = mRadius;
    f->mEdgePreservation = mEdgePreservation;
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

    if (qFuzzyIsNull(mRadius) || mEdgePreservation == 100)
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    double sigmaS = (mRadius + .5) / 2.45;
    double sigmaR = (100 - mEdgePreservation) * 255. / 100. * 2;

    cv::Mat msrc(inputImage.height(), inputImage.width(), CV_8UC4, (void *)inputImage.bits());
    cv::Mat msrcbgr(msrc.rows, msrc.cols, CV_8UC3);
    cv::Mat msrcalpha(msrc.rows, msrc.cols, CV_8UC1);
    cv::Mat mdst(i.height(), i.width(), CV_8UC4, i.bits());
    cv::Mat mdstbgr;

    cv::Mat out[] = { msrcbgr, msrcalpha };
    int from_to[] = { 0,0, 1,1, 2,2, 3,3 };
    cv::mixChannels(&msrc, 1, out, 2, from_to, 4);

    cv::ximgproc::dtFilter(msrcbgr, msrcbgr, mdstbgr, sigmaS, sigmaR);

    cv::Mat out2[] = { mdstbgr, msrcalpha };
    cv::mixChannels(out2, 2, &mdst, 1, from_to, 4);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    double radius;
    int edgePreservation;
    bool ok;
    radius = s.value("radius", 0.0).toDouble(&ok);
    if (!ok || radius < 0. || radius > 100.)
        return false;
    edgePreservation = s.value("edgepreservation", 95).toInt(&ok);
    if (!ok || edgePreservation < 0 || edgePreservation > 100)
        return false;
    setRadius(radius);
    setEdgePreservation(edgePreservation);
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("radius", mRadius);
    s.setValue("edgepreservation", mEdgePreservation);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setRadius(mRadius);
    fw->setEdgePreservation(mEdgePreservation);
    connect(this, SIGNAL(radiusChanged(double)), fw, SLOT(setRadius(double)));
    connect(this, SIGNAL(edgePreservationChanged(int)), fw, SLOT(setEdgePreservation(int)));
    connect(fw, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
    connect(fw, SIGNAL(edgePreservationChanged(int)), this, SLOT(setEdgePreservation(int)));
    return fw;
}

void Filter::setRadius(double s)
{
    if (s == mRadius)
        return;
    mRadius = s;
    emit radiusChanged(s);
    emit parametersChanged();
}

void Filter::setEdgePreservation(int v)
{
    if (v == mEdgePreservation)
        return;
    mEdgePreservation = v;
    emit edgePreservationChanged(v);
    emit parametersChanged();
}
