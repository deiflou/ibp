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

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"
#include "../imgproc/util.h"

Filter::Filter() :
    mColorMode(0)
{
    for (int i = 0; i < 5; i++)
    {
        mAffectedChannel[i] = false;
        mThreshold[i] = 128;
        memset(&mLUT[i][0], 0, 128);
        memset(&mLUT[i][128], 255, 128);
    }
    for (int i = 0; i < 256; i++)
        mIdLUT[i] = i;
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mColorMode = mColorMode;
    for (int i = 0; i < 5; i++)
    {
        f->mAffectedChannel[i] = mAffectedChannel[i];
        f->mThreshold[i] = mThreshold[i];
    }
    memcpy(f->mLUT, mLUT, sizeof(mLUT));
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

    QImage i = inputImage.copy();
    register BGRA * bits = (BGRA *)i.bits();
    unsigned char * mLuts[5];
    register int totalPixels = i.width() * i.height();

    if (mColorMode == 0)
    {
        if (!mAffectedChannel[0] && !mAffectedChannel[4])
            return i;
        mLuts[4] = mAffectedChannel[4] ? mLUT[4] : mIdLUT;
        if (mAffectedChannel[0])
        {
            while (totalPixels--)
            {
                bits->r = bits->g = bits->b = mLUT[0][AT_pixelIntensity4(bits->r, bits->g, bits->b)];
                bits->a = mLuts[4][bits->a];
                bits++;
            }
        }
        else
        {
            while (totalPixels--)
            {
                bits->a = mLuts[4][bits->a];
                bits++;
            }
        }

    }
    else
    {
        if (!mAffectedChannel[1] && !mAffectedChannel[2] &&
            !mAffectedChannel[3] && !mAffectedChannel[4])
            return i;
        mLuts[1] = mAffectedChannel[1] ? mLUT[1] : mIdLUT;
        mLuts[2] = mAffectedChannel[2] ? mLUT[2] : mIdLUT;
        mLuts[3] = mAffectedChannel[3] ? mLUT[3] : mIdLUT;
        mLuts[4] = mAffectedChannel[4] ? mLUT[4] : mIdLUT;
        while (totalPixels--)
        {
            bits->r = mLuts[1][bits->r];
            bits->g = mLuts[2][bits->g];
            bits->b = mLuts[3][bits->b];
            bits->a = mLuts[4][bits->a];
            bits++;
        }
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString colorModeStr, affectedChannelStr;
    int colorMode;
    QStringList affectedChannelList;
    bool affectedChannel[5] =  { false };
    int threshold[5] = { 128 };
    bool ok;

    colorModeStr = s.value("colormode", "luma").toString();
    if (colorModeStr == "luma")
        colorMode = 0;
    else if (colorModeStr == "rgb")
        colorMode = 1;
    else
        return false;

    affectedChannelStr = s.value("affectedchannels", "").toString();
    affectedChannelList = affectedChannelStr.split(" ", QString::SkipEmptyParts);
    for (int i = 0; i < affectedChannelList.size(); i++)
    {
        affectedChannelStr = affectedChannelList.at(i);
        if (affectedChannelList.at(i) == "luma")
            affectedChannel[0] = true;
        else if (affectedChannelList.at(i) == "red")
            affectedChannel[1] = true;
        else if (affectedChannelList.at(i) == "green")
            affectedChannel[2] = true;
        else if (affectedChannelList.at(i) == "blue")
            affectedChannel[3] = true;
        else if (affectedChannelList.at(i) == "alpha")
            affectedChannel[4] = true;
        else
            return false;
    }

    threshold[0] = s.value("lumathreshold", 128).toInt(&ok);
    if (!ok || threshold[0] < 0 || threshold[0] > 255)
        return false;
    threshold[1] = s.value("redthreshold", 128).toInt(&ok);
    if (!ok || threshold[1] < 0 || threshold[1] > 255)
        return false;
    threshold[2] = s.value("greenthreshold", 128).toInt(&ok);
    if (!ok || threshold[2] < 0 || threshold[2] > 255)
        return false;
    threshold[3] = s.value("bluethreshold", 128).toInt(&ok);
    if (!ok || threshold[3] < 0 || threshold[3] > 255)
        return false;
    threshold[4] = s.value("alphathreshold", 128).toInt(&ok);
    if (!ok || threshold[4] < 0 || threshold[4] > 255)
        return false;

    setColorMode(colorMode);
    for (int i = 0; i < 5; i++)
    {
        setAffectedChannel(i, affectedChannel[i]);
        setThreshold(i, threshold[i]);
    }

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("colormode", mColorMode == 0 ? "luma" : "rgb");

    QStringList affectedChannelList;
    if (mAffectedChannel[0])
        affectedChannelList.append("luma");
    if (mAffectedChannel[1])
        affectedChannelList.append("red");
    if (mAffectedChannel[2])
        affectedChannelList.append("green");
    if (mAffectedChannel[3])
        affectedChannelList.append("blue");
    if (mAffectedChannel[4])
        affectedChannelList.append("alpha");
    s.setValue("affectedchannels", affectedChannelList.join(" "));

    s.setValue("lumathreshold", mThreshold[0]);
    s.setValue("redthreshold", mThreshold[1]);
    s.setValue("greenthreshold", mThreshold[2]);
    s.setValue("bluethreshold", mThreshold[3]);
    s.setValue("alphathreshold", mThreshold[4]);

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setColorMode(mColorMode);
    for (int i = 0; i < 5; i++)
    {
        fw->setAffectedChannel(i, mAffectedChannel[i]);
        fw->setThreshold(i, mThreshold[i]);
    }
    connect(this, SIGNAL(colorModeChanged(int)), fw, SLOT(setColorMode(int)));
    connect(this, SIGNAL(affectedChannelChanged(int,bool)), fw, SLOT(setAffectedChannel(int,bool)));
    connect(this, SIGNAL(thresholdChanged(int,int)), fw, SLOT(setThreshold(int,int)));
    connect(fw, SIGNAL(colorModeChanged(int)), this, SLOT(setColorMode(int)));
    connect(fw, SIGNAL(affectedChannelChanged(int,bool)), this, SLOT(setAffectedChannel(int,bool)));
    connect(fw, SIGNAL(thresholdChanged(int,int)), this, SLOT(setThreshold(int,int)));
    return fw;
}

void Filter::setColorMode(int m)
{
    if (m == mColorMode)
        return;
    mColorMode = m;
    emit colorModeChanged(m);
    emit parametersChanged();
}

void Filter::setAffectedChannel(int c, bool a)
{
    if (a == mAffectedChannel[c])
        return;
    mAffectedChannel[c] = a;
    emit affectedChannelChanged(c, a);
    emit parametersChanged();
}

void Filter::setThreshold(int c, int t)
{
    if (t == mThreshold[c])
        return;
    mThreshold[c] = t;
    for (int i = 0; i < 256; i++)
        mLUT[c][i] = i > t ? 255 : 0;
    emit thresholdChanged(c, t);
    emit parametersChanged();
}

