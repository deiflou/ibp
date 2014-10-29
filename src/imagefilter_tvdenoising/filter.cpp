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

#include <opencv2/photo.hpp>
#include <vector>

#include "filter.h"
#include "filterwidget.h"
#include "../misc/util.h"

Filter::Filter() :
    mStrength(0.),
    mIterations(30)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mStrength = mStrength;
    f->mIterations = mIterations;
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

    if (qFuzzyCompare(mStrength, 0.))
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    cv::Mat mSrc(inputImage.height(), inputImage.width(), CV_8UC4, (void *)inputImage.bits());
    cv::Mat mDst(i.height(), i.width(), CV_8UC4, i.bits());
    cv::Mat mRed(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mGreen(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mBlue(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mAlpha(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mRedDenoised(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mGreenDenoised(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mBlueDenoised(inputImage.height(), inputImage.width(), CV_8UC1);;
    double lambda = pow(AT_maximum(100. - mStrength, .001) / 100., 3) * 10.;
    cv::Mat mOutSplit[] = { mBlue, mGreen, mRed, mAlpha };
    cv::Mat mOutMerge[] = { mBlueDenoised, mGreenDenoised, mRedDenoised, mAlpha };
    int fromTo[] = { 0, 0, 1, 1, 2, 2, 3, 3 };

    // split the image channels
    cv::mixChannels(&mSrc, 1, mOutSplit, 4, fromTo, 4);

    // denoise
    std::vector<cv::Mat> observations;
    observations.push_back(mBlue);
    cv::denoise_TVL1(observations, mBlueDenoised, lambda, mIterations);

    observations.clear();
    observations.push_back(mGreen);
    cv::denoise_TVL1(observations, mGreenDenoised, lambda, mIterations);

    observations.clear();
    observations.push_back(mRed);
    cv::denoise_TVL1(observations, mRedDenoised, lambda, mIterations);

    // merge image channels
    cv::mixChannels(mOutMerge, 4, &mDst, 1, fromTo, 4);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    double strength;
    int iterations;
    bool ok;
    strength = s.value("strength", 0.).toDouble(&ok);
    if (!ok || strength < 0 || strength > 100)
        return false;
    iterations = s.value("iterations", 30).toInt(&ok);
    if (!ok || iterations < 1 || iterations > 100)
        return false;
    setStrength(strength);
    setIterations(iterations);
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("strength", mStrength);
    s.setValue("iterations", mIterations);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setStrength(mStrength);
    fw->setIterations(mIterations);
    connect(this, SIGNAL(strengthChanged(double)), fw, SLOT(setStrength(double)));
    connect(this, SIGNAL(iterationsChanged(int)), fw, SLOT(setIterations(int)));
    connect(fw, SIGNAL(strengthChanged(double)), this, SLOT(setStrength(double)));
    connect(fw, SIGNAL(iterationsChanged(int)), this, SLOT(setIterations(int)));
    return fw;
}

void Filter::setStrength(double s)
{
    if (s == mStrength)
        return;
    mStrength = s;
    emit strengthChanged(s);
    emit parametersChanged();
}

void Filter::setIterations(int i)
{
    if (i == mIterations)
        return;
    mIterations = i;
    emit iterationsChanged(i);
    emit parametersChanged();
}
