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

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"

Filter::Filter() :
    mReference(AlphaChannel),
    mThreshold(0),
    mMargins(0, 0, 0, 0)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mReference = mReference;
    f->mThreshold = mThreshold;
    f->mMargins = mMargins;
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

    register BGRA * bits = (BGRA *)inputImage.bits();
    register int x, y, t, l, r, b;
    const int w = inputImage.width();
    const int h = inputImage.height();
    const int threshold = mThreshold;
    t = h - 1;
    l = w - 1;
    b = r = 0;

    if (mReference == AlphaChannel)
    {
        for (y = 0; y < h; y++)
        {
            for (x = 0; x < w; x++, bits++)
            {
                if (bits->a > threshold)
                {
                    if (y < t)
                        t = y;
                    if (x < l)
                        l = x;
                    if (y > b)
                        b = y;
                    if (x > r)
                        r = x;
                }
            }
        }
    }
    else
    {
        const unsigned int gR = .2126 * 0x10000;
        const unsigned int gG = .7152 * 0x10000;
        const unsigned int gB = .0722 * 0x10000;
        register int g;
        for (y = 0; y < h; y++)
        {
            for (x = 0; x < w; x++, bits++)
            {
                g = 255 - ((bits->r * gR >> 16) + (bits->g * gG >> 16) + (bits->b * gB >> 16));
                if (g > threshold)
                {
                    if (y < t)
                        t = y;
                    if (x < l)
                        l = x;
                    if (y > b)
                        b = y;
                    if (x > r)
                        r = x;
                }
            }
        }
    }

    QRect rect;
    if (b - t < 0 || r - l < 0)
        rect = QRect(0, 0, 1, 1);
    else
        rect = QRect(l, t, r - l + 1, b - t + 1);
    rect.adjust(-mMargins.left(), -mMargins.top(), mMargins.right(), mMargins.bottom());

    return inputImage.copy(rect.intersected(inputImage.rect()));
}

bool Filter::loadParameters(QSettings &s)
{
    QString referenceStr;
    Reference reference;
    int threshold;
    int marginTop, marginLeft, marginBottom, marginRight;
    bool ok;

    referenceStr = s.value("reference", "alphachannel").toString();
    if (referenceStr == "alphachannel")
        reference = AlphaChannel;
    else if (referenceStr == "luma")
        reference = Luma;
    else
        return false;

    threshold = s.value("threshold", 0).toInt(&ok);
    if (!ok || threshold < 0 || threshold > 254)
        return false;

    marginTop = s.value("margintop", 0).toInt(&ok);
    if (!ok || marginTop < 0 || marginTop > 500)
        return false;
    marginLeft = s.value("marginleft", 0).toInt(&ok);
    if (!ok || marginLeft < 0 || marginLeft > 500)
        return false;
    marginBottom = s.value("marginbottom", 0).toInt(&ok);
    if (!ok || marginBottom < 0 || marginBottom > 500)
        return false;
    marginRight = s.value("marginright", 0).toInt(&ok);
    if (!ok || marginRight < 0 || marginRight > 500)
        return false;

    setReference(reference);
    setThreshold(threshold);
    setMargins(QMargins(marginLeft, marginTop, marginRight, marginBottom));
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("reference", mReference == AlphaChannel ? "alphachannel" : "luma");
    s.setValue("threshold", mThreshold);
    s.setValue("margintop", mMargins.top());
    s.setValue("marginleft", mMargins.left());
    s.setValue("marginbottom", mMargins.bottom());
    s.setValue("marginright", mMargins.right());
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setReference(mReference);
    fw->setThreshold(mThreshold);
    fw->setMargins(mMargins);
    connect(this, SIGNAL(referenceChanged(Filter::Reference)), fw, SLOT(setReference(Filter::Reference)));
    connect(this, SIGNAL(thresholdChanged(int)), fw, SLOT(setThreshold(int)));
    connect(this, SIGNAL(marginsChanged(QMargins)), fw, SLOT(setMargins(QMargins)));
    connect(fw, SIGNAL(referenceChanged(Filter::Reference)), this, SLOT(setReference(Filter::Reference)));
    connect(fw, SIGNAL(thresholdChanged(int)), this, SLOT(setThreshold(int)));
    connect(fw, SIGNAL(marginsChanged(QMargins)), this, SLOT(setMargins(QMargins)));
    return fw;
}

void Filter::setReference(Filter::Reference v)
{
    if (v == mReference)
        return;
    mReference = v;
    emit referenceChanged(v);
    emit parametersChanged();
}

void Filter::setThreshold(int v)
{
    if (v == mThreshold)
        return;
    mThreshold = v;
    emit thresholdChanged(v);
    emit parametersChanged();
}

void Filter::setMargins(const QMargins &v)
{
    if (v == mMargins)
        return;
    mMargins = v;
    emit marginsChanged(v);
    emit parametersChanged();
}
