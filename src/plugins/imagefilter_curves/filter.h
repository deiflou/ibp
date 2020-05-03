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

#include <imgproc/imagefilter.h>
#include "../misc/interpolator1D.h"

using namespace ibp::imgproc;
using namespace ibp::misc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    enum WorkingChannel
    {
        Luma, Red, Green, Blue, Alpha
    };
    enum InterpolationMode
    {
        Flat,
        Linear,
        Smooth
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
    WorkingChannel mWorkingChannel;
    Interpolator1D * mSplineInterpolator[5];
    InterpolationMode mInterpolationMode[5];
    unsigned char mLuts[5][256];

    void makeLUT(WorkingChannel c);

signals:
    void workingChannelChanged(Filter::WorkingChannel s);
    void knotsChanged(const Interpolator1DKnots & k);
    void interpolationModeChanged(Filter::InterpolationMode im);

public slots:
    void setWorkingChannel(Filter::WorkingChannel s);
    void setKnots(const Interpolator1DKnots & k);
    void setInterpolationMode(Filter::InterpolationMode im);
};

#endif // FILTER_H
