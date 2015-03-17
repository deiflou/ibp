/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
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

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>
#include <opencv2/imgproc.hpp>

#include "../imgproc/imagefilter.h"

using namespace anitools::imgproc;

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
