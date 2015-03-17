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

#include <QDebug>

#include "filter.h"
#include "filterwidget.h"

Filter::Filter() :
    mModifyRGB(true),
    mModifyAlpha(true),
    mMorphologyOp(Dilation),
    mKernelShape(Ellipse),
    mVRadius(0),
    mHRadius(0),
    mLockRadius(true)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mModifyRGB = mModifyRGB;
    f->mModifyAlpha = mModifyAlpha;
    f->mMorphologyOp = mMorphologyOp;
    f->mKernelShape = mKernelShape;
    f->mVRadius = mVRadius;
    f->mHRadius = mHRadius;
    f->mLockRadius = mLockRadius;
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

    if (mHRadius == 0 || mVRadius == 0)
        return inputImage;

    if (!mModifyRGB && !mModifyAlpha)
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    int hsize = mHRadius * 2;
    hsize += hsize % 2 == 0 ? 1 : 0;
    int vsize = mVRadius * 2;
    vsize += vsize % 2 == 0 ? 1 : 0;
    cv::Mat kernel;
    cv::Mat mSrc(inputImage.height(), inputImage.width(), CV_8UC4, (void *)inputImage.bits());
    cv::Mat mDst(i.height(), i.width(), CV_8UC4, i.bits());

    switch (mKernelShape)
    {
    case Ellipse:
        kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(hsize, vsize));
        break;
    case Rectangle:
        kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(hsize, vsize));
        break;
    case Diamond:
        kernel = getDiamondStructuringElement(cv::Size(hsize, vsize));
        break;
    case Octagon:
        kernel = getOctagonStructuringElement(cv::Size(hsize, vsize));
        break;
    case Plus:
        kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(hsize, vsize));
        break;
    case Cross:
        kernel = getCrossStructuringElement(cv::Size(hsize, vsize));
        break;
    case Ring:
        kernel = getRingStructuringElement(cv::Size(hsize, vsize));
        break;
    default:
        kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(hsize, vsize));
        break;
    }

    // split the image channels
    cv::Mat mRGB(inputImage.height(), inputImage.width(), CV_8UC3);
    cv::Mat mAlpha(inputImage.height(), inputImage.width(), CV_8UC1);
    cv::Mat mOutSplit[] = { mRGB, mAlpha };
    int fromTo[] = { 0, 0, 1, 1, 2, 2, 3, 3 };
    cv::mixChannels(&mSrc, 1, mOutSplit, 2, fromTo, 4);

    // apply gaussian blur
    cv::Mat mRGBMorph;
    cv::Mat mAlphaMorph;
    if (mModifyRGB)
    {
        mRGBMorph = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC3);
        switch (mMorphologyOp)
        {
        case Dilation:
            cv::dilate(mRGB, mRGBMorph, kernel);
            break;
        case Erosion:
            cv::erode(mRGB, mRGBMorph, kernel);
            break;
        case Closing:
            cv::morphologyEx(mRGB, mRGBMorph, cv::MORPH_CLOSE, kernel);
            break;
        case Opening:
            cv::morphologyEx(mRGB, mRGBMorph, cv::MORPH_OPEN, kernel);
            break;
        }
    }
    else
        mRGBMorph = mRGB;
    if (mModifyAlpha)
    {
        mAlphaMorph = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC1);
        switch (mMorphologyOp)
        {
        case Dilation:
            cv::dilate(mAlpha, mAlphaMorph, kernel);
            break;
        case Erosion:
            cv::erode(mAlpha, mAlphaMorph, kernel);
            break;
        case Closing:
            cv::morphologyEx(mAlpha, mAlphaMorph, cv::MORPH_CLOSE, kernel);
            break;
        case Opening:
            cv::morphologyEx(mAlpha, mAlphaMorph, cv::MORPH_OPEN, kernel);
            break;
        }
    }
    else
        mAlphaMorph = mAlpha;

    // merge image channels
    cv::Mat mOutMerge[] = { mRGBMorph, mAlphaMorph };
    cv::mixChannels(mOutMerge, 2, &mDst, 1, fromTo, 4);

    return i;
}

cv::Mat Filter::getDiamondStructuringElement(cv::Size size)
{
    cv::Mat m = cv::Mat::ones(size, CV_8UC1);
    register int d, x, y, s;
    if (size.height > size.width)
    {
        d = (size.width << 8) / size.height;
        for (x = 0, s = (size.width << 8 >> 1); x < (size.height >> 1); x++, s -= d)
        {
            for (y = 0; y < (s >> 8); y++)
            {
                m.at<unsigned char>(x, y) = 0;
                m.at<unsigned char>(size.height - 1 - x, y) = 0;
                m.at<unsigned char>(x, size.width - 1 - y) = 0;
                m.at<unsigned char>(size.height - 1 - x, size.width - 1 - y) = 0;
            }
        }
    }
    else
    {
        d = (size.height << 8) / size.width;
        for (y = 0, s = (size.height << 8 >> 1); y < (size.width >> 1); y++, s -= d)
        {
            for (x = 0; x < (s >> 8); x++)
            {
                m.at<unsigned char>(x, y) = 0;
                m.at<unsigned char>(size.height - 1 - x, y) = 0;
                m.at<unsigned char>(x, size.width - 1 - y) = 0;
                m.at<unsigned char>(size.height - 1 - x, size.width - 1 - y) = 0;
            }
        }
    }
    return m;
}

cv::Mat Filter::getOctagonStructuringElement(cv::Size size)
{
    cv::Mat m = cv::Mat::ones(size, CV_8UC1);
    register int d, x, y, s;
    if (size.height > size.width)
    {
        d = (size.width << 8) / size.height;
        for (x = 0, s = (size.width << 8 >> 2); x < (size.height >> 2); x++, s -= d)
        {
            for (y = 0; y < (s >> 8); y++)
            {
                m.at<unsigned char>(x, y) = 0;
                m.at<unsigned char>(size.height - 1 - x, y) = 0;
                m.at<unsigned char>(x, size.width - 1 - y) = 0;
                m.at<unsigned char>(size.height - 1 - x, size.width - 1 - y) = 0;
            }
        }
    }
    else
    {
        d = (size.height << 8) / size.width;
        for (y = 0, s = (size.height << 8 >> 2); y < (size.width >> 2); y++, s -= d)
        {
            for (x = 0; x < (s >> 8); x++)
            {
                m.at<unsigned char>(x, y) = 0;
                m.at<unsigned char>(size.height - 1 - x, y) = 0;
                m.at<unsigned char>(x, size.width - 1 - y) = 0;
                m.at<unsigned char>(size.height - 1 - x, size.width - 1 - y) = 0;
            }
        }
    }
    return m;
}

cv::Mat Filter::getCrossStructuringElement(cv::Size size)
{
    cv::Mat m = cv::Mat::zeros(size, CV_8UC1);
    register int d, x, y, s;
    if (size.width > size.height)
    {
        d = (size.height << 8) / size.width;
        for (x = 0, s = 0; x < (size.width >> 1); x++, s += d)
        {
            y = s >> 8;
            m.at<unsigned char>(y, x) = 1;
            m.at<unsigned char>(y, size.width - 1 - x) = 1;
            m.at<unsigned char>(size.height - 1 - y, x) = 1;
            m.at<unsigned char>(size.height - 1 - y, size.width - 1 - x) = 1;
        }
    }
    else
    {
        d = (size.width << 8) / size.height;
        for (y = 0, s = 0; y < (size.height >> 1); y++, s += d)
        {
            x = s >> 8;
            m.at<unsigned char>(y, x) = 1;
            m.at<unsigned char>(y, size.width - 1 - x) = 1;
            m.at<unsigned char>(size.height - 1 - y, x) = 1;
            m.at<unsigned char>(size.height - 1 - y, size.width - 1 - x) = 1;
        }
    }
    m.at<unsigned char>(size.height >> 1, size.width >> 1) = 1;
    return m;;
}

cv::Mat Filter::getRingStructuringElement(cv::Size size)
{
    cv::Mat m = cv::Mat::zeros(size, CV_8UC1);
    register int xc = size.width >> 1, yc = size.height >> 1;
    int rx = size.width >> 1, ry = size.height >> 1;
    register int rxsq = rx * rx, rysq = ry * ry;
    register int rxsqtimes2 = rxsq * 2, rysqtimes2 = rysq * 2;
    register int x = 0, y = ry;
    register int p = (rysq) - (rxsq * ry) + (rxsq >> 2);

    while (x * rysqtimes2 < y * rxsqtimes2)
    {
         m.at<unsigned char>(yc + y, xc - x) = 1;
         m.at<unsigned char>(yc - y, xc + x) = 1;
         m.at<unsigned char>(yc + y, xc + x) = 1;
         m.at<unsigned char>(yc - y, xc - x) = 1;

         if (p < 0)
         {
             x++;
             p = p + (rysqtimes2 * x) + rysq;
         }
         else
         {
             x++;
             y--;
             p = p + (rysqtimes2 * x + rysq) - (rxsqtimes2 * y);
         }
    }
    p = ((float)x + 0.5) * ((float)x + 0.5) * rysq + (y - 1) * (y - 1) * rxsq - rxsq * rysq;

    while (y >= 0)
    {
        m.at<unsigned char>(yc + y, xc - x) = 1;
        m.at<unsigned char>(yc - y, xc + x) = 1;
        m.at<unsigned char>(yc + y, xc + x) = 1;
        m.at<unsigned char>(yc - y, xc - x) = 1;

        if (p > 0)
        {
            y--;
            p = p - (rxsqtimes2 * y) + rxsq;
        }
        else
        {
            y--;
            x++;
            p = p + (rysqtimes2 * x) - (rxsqtimes2 * y) - (rxsq);
        }
    }
    return m;
}

bool Filter::loadParameters(QSettings &s)
{
    bool modifyRGB, modifyAlpha;
    QString mopstr;
    MorphologyOp mop;
    QString shapestr;
    KernelShape shape;
    int hradius, vradius;
    bool lock, ok;

    modifyRGB = s.value("modifyrgb", true).toBool();
    modifyAlpha = s.value("modifyalpha", true).toBool();

    mopstr = s.value("morphologicalop", "dilation").toString();
    if (mopstr == "dilation")
        mop = Dilation;
    else if (mopstr == "erosion")
        mop = Erosion;
    else if (mopstr == "closing")
        mop = Closing;
    else if (mopstr == "opening")
        mop = Opening;
    else
        mop = Dilation;

    shapestr = s.value("kernelshape", "ellipse").toString();
    if (shapestr == "ellipse")
        shape = Ellipse;
    else if (shapestr == "rectangle")
        shape = Rectangle;
    else if (shapestr == "diamond")
        shape = Diamond;
    else if (shapestr == "octagon")
        shape = Octagon;
    else if (shapestr == "plus")
        shape = Plus;
    else if (shapestr == "cross")
        shape = Cross;
    else if (shapestr == "ring")
        shape = Ring;
    else
        shape = Ellipse;

    hradius = s.value("hradius", 0).toInt(&ok);
    if (!ok)
        hradius = 0;
    vradius = s.value("vradius", 0).toInt(&ok);
    if (!ok)
        vradius = 0;

    lock = s.value("lockRadius", true).toBool();
    if (lock && hradius != vradius)
        vradius = hradius;

    setModifyRGB(modifyRGB);
    setModifyAlpha(modifyAlpha);
    setMorphologyOp(mop);
    setKernelShape(shape);
    setLockRadius(lock);
    setHRadius(hradius);
    setVRadius(vradius);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    QString mopstr;
    QString shapestr;

    if (mMorphologyOp == Dilation)
        mopstr = "dilation";
    else if (mMorphologyOp == Erosion)
        mopstr = "erosion";
    else if (mMorphologyOp == Closing)
        mopstr = "closing";
    else if (mMorphologyOp == Opening)
        mopstr = "opening";

    if (mKernelShape == Ellipse)
        shapestr = "ellipse";
    else if (mKernelShape == Rectangle)
        shapestr = "rectangle";
    else if (mKernelShape == Diamond)
        shapestr = "diamond";
    else if (mKernelShape == Octagon)
        shapestr = "octagon";
    else if (mKernelShape == Plus)
        shapestr = "plus";
    else if (mKernelShape == Cross)
        shapestr = "cross";
    else if (mKernelShape == Ring)
        shapestr = "ring";

    s.setValue("modifyrgb", mModifyRGB);
    s.setValue("modifyalpha", mModifyAlpha);
    s.setValue("morphologicalop", mopstr);
    s.setValue("kernelshape", shapestr);
    s.setValue("hradius", mHRadius);
    s.setValue("vradius", mVRadius);
    s.setValue("lockRadius", mLockRadius);

    return false;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setModifyRGB(mModifyRGB);
    fw->setModifyAlpha(mModifyAlpha);
    fw->setMorphologyOp(mMorphologyOp);
    fw->setKernelShape(mKernelShape);
    fw->setLockRadius(mLockRadius);
    fw->setHRadius(mHRadius);
    fw->setVRadius(mVRadius);
    connect(this, SIGNAL(modifyRGBChanged(bool)), fw, SLOT(setModifyRGB(bool)));
    connect(this, SIGNAL(modifyAlphaChanged(bool)), fw, SLOT(setModifyAlpha(bool)));
    connect(this, SIGNAL(morphologyOpChanged(Filter::MorphologyOp)), fw, SLOT(setMorphologyOp(Filter::MorphologyOp)));
    connect(this, SIGNAL(kernelShapeChanged(Filter::KernelShape)), fw, SLOT(setKernelShape(Filter::KernelShape)));
    connect(this, SIGNAL(hRadiusChanged(int)), fw, SLOT(setHRadius(int)));
    connect(this, SIGNAL(vRadiusChanged(int)), fw, SLOT(setVRadius(int)));
    connect(this, SIGNAL(lockRadiusChanged(bool)), fw, SLOT(setLockRadius(bool)));
    connect(fw, SIGNAL(modifyRGBChanged(bool)), this, SLOT(setModifyRGB(bool)));
    connect(fw, SIGNAL(modifyAlphaChanged(bool)), this, SLOT(setModifyAlpha(bool)));
    connect(fw, SIGNAL(morphologyOpChanged(Filter::MorphologyOp)), this, SLOT(setMorphologyOp(Filter::MorphologyOp)));
    connect(fw, SIGNAL(kernelShapeChanged(Filter::KernelShape)), this, SLOT(setKernelShape(Filter::KernelShape)));
    connect(fw, SIGNAL(hRadiusChanged(int)), this, SLOT(setHRadius(int)));
    connect(fw, SIGNAL(vRadiusChanged(int)), this, SLOT(setVRadius(int)));
    connect(fw, SIGNAL(lockRadiusChanged(bool)), this, SLOT(setLockRadius(bool)));
    return fw;
}

void Filter::setModifyRGB(bool v)
{
    if (v == mModifyRGB)
        return;
    mModifyRGB = v;
    emit modifyRGBChanged(v);
    emit parametersChanged();
}

void Filter::setModifyAlpha(bool v)
{
    if (v == mModifyAlpha)
        return;
    mModifyAlpha = v;
    emit modifyAlphaChanged(v);
    emit parametersChanged();
}

void Filter::setMorphologyOp(Filter::MorphologyOp mop)
{
    if (mop == mMorphologyOp)
        return;
    mMorphologyOp = mop;
    emit morphologyOpChanged(mop);
    emit parametersChanged();
}

void Filter::setKernelShape(Filter::KernelShape shape)
{
    if (shape == mKernelShape)
        return;
    mKernelShape = shape;
    emit kernelShapeChanged(shape);
    emit parametersChanged();
}

void Filter::setVRadius(int r)
{
    if (r == mVRadius)
        return;
    mVRadius = r;
    emit vRadiusChanged(r);
    emit parametersChanged();
}

void Filter::setHRadius(int r)
{
    if (r == mHRadius)
        return;
    mHRadius = r;
    emit hRadiusChanged(r);
    emit parametersChanged();
}

void Filter::setLockRadius(bool l)
{
    if (l == mLockRadius)
        return;
    mLockRadius = l;
    emit lockRadiusChanged(l);
    emit parametersChanged();
}
