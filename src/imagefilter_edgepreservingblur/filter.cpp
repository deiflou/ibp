/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
**
** This file is part of Anitools
**
** Anitools is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <opencv2/imgproc.hpp>

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"

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

extern "C" QHash<QString, QString> getAnitoolsPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getAnitoolsPluginInfo();
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
    cv::Mat mdstbgr(msrc.rows, msrc.cols, CV_8UC3);

    cv::Mat out[] = { msrcbgr, msrcalpha };
    int from_to[] = { 0,0, 1,1, 2,2, 3,3 };
    cv::mixChannels(&msrc, 1, out, 2, from_to, 4);

    cv::bilateralFilter(msrcbgr, mdstbgr, 0, sigmaR, sigmaS);

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
