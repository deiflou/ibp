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

#include <QRegularExpression>
#include <QPainter>

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/util.h"
#include "../imgproc/pixelblending.h"
#include "../imgproc/lut.h"

Filter::Filter() :
    mImage(),
    mPosition(Front),
    mColorCompositionMode(ColorCompositionMode_Normal),
    mOpacity(100)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mImage = mImage;
    f->mPosition = mPosition;
    f->mColorCompositionMode = mColorCompositionMode;
    f->mOpacity = mOpacity;
    f->mTransformations = mTransformations;
    f->mBypasses = mBypasses;
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

    // Create texture
    QImage texture(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    QBrush brush(mImage);
    QTransform tfm;
    QPainter p(&texture);
    for (int i = mTransformations.size() - 1; i >= 0; i--)
    {
        if (mTransformations.at(i).type == Translation)
            tfm.translate(mTransformations.at(i).x, mTransformations.at(i).y);
        else if (mTransformations.at(i).type == Scaling)
            tfm.scale(mTransformations.at(i).x / 100., mTransformations.at(i).y / 100.);
        else if (mTransformations.at(i).type == Rotation)
            tfm.rotate(-mTransformations.at(i).z);
        else
            tfm.shear(mTransformations.at(i).x / 100., mTransformations.at(i).y / 100.);
    }
    tfm.translate(-mImage.width() / 2, -mImage.height() / 2);
    brush.setTransform(tfm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setBrushOrigin(inputImage.width() >> 1, inputImage.height() >> 1);
    p.fillRect(texture.rect(), brush);

    // Paint Texture
    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);
    register BGRA * src = (BGRA *)texture.bits(), * dst = (BGRA *)inputImage.bits(), * blend = (BGRA *)i.bits();
    register int totalPixels = i.width() * i.height();
    const int opacity = qRound(mOpacity * 255 / 100.);
    if (mPosition == Front)
    {
        while (totalPixels--)
        {
            src->a = lut01[src->a][opacity];
            blendColors[mColorCompositionMode](*src, *dst, *blend);
            src++;
            dst++;
            blend++;
        }
    }
    else if (mPosition == Inside)
    {
        if (mColorCompositionMode == ColorCompositionMode_Normal)
        {
            while (totalPixels--)
            {
                src->a = lut01[src->a][opacity];
                blendSourceAtopDestination(*src, *dst, *blend);
                src++;
                dst++;
                blend++;
            }
        }
        else
        {
            while (totalPixels--)
            {
                src->a = lut01[src->a][opacity];
                blendColors[mColorCompositionMode](*src, *dst, *blend);
                blendSourceAtopDestination(*blend, *dst, *blend);
                src++;
                dst++;
                blend++;
            }
        }
    }
    else
    {
        while (totalPixels--)
        {
            src->a = lut01[src->a][opacity];
            blendDestinationOverSource(*src, *dst, *blend);
            src++;
            dst++;
            blend++;
        }
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QVariant v;
    QString positionStr;
    QString colorCompositionModeStr;
    QImage image;
    Position position;
    ColorCompositionMode colorCompositionMode;
    int opacity;
    QString tfmStr;
    QStringList tfmList1, tfmList2;
    QList<AffineTransformation> transformations;
    QList<bool> bypasses;
    AffineTransformation transformation;
    bool bypass;
    bool ok;

    v = s.value("image", QImage());
    if (!v.isValid() || !v.canConvert<QImage>())
        image = QImage();
    else
        image = v.value<QImage>();

    positionStr = s.value("position", "front").toString();
    if (positionStr == "front")
        position = Front;
    else if (positionStr == "behind")
        position = Behind;
    else if (positionStr == "inside")
        position = Inside;
    else
        return false;

    colorCompositionModeStr = s.value("colorcompositionmode", "normal").toString();
    colorCompositionMode = colorCompositionModeStringToEnum(colorCompositionModeStr);
    if (colorCompositionMode == ColorCompositionMode_Unsupported)
      return false;

    opacity = s.value("opacity", 100).toInt(&ok);
    if (!ok || opacity < 0 || opacity > 100)
        return false;

    tfmStr = s.value("geometrictransformations").toString();
    tfmList1 = tfmStr.split(QRegularExpression("\\s*,\\s*"), QString::SkipEmptyParts);
    for (int i = 0; i < tfmList1.size(); i++)
    {
        tfmList2 = tfmList1.at(i).split(QRegularExpression("\\s+"), QString::SkipEmptyParts);

        if (tfmList2.size() != 3 && tfmList2.size() != 4)
            return false;

        if (tfmList2.at(0) == "translation")
            transformation.type = Translation;
        else if (tfmList2.at(0) == "scaling")
            transformation.type = Scaling;
        else if (tfmList2.at(0) == "rotation")
            transformation.type = Rotation;
        else if (tfmList2.at(0) == "shearing")
            transformation.type = Shearing;
        else
            return false;

        if (tfmList2.at(1) == "false")
            bypass = false;
        else if (tfmList2.at(1) == "true")
            bypass = true;
        else
            return false;

        if (tfmList2.size() == 3)
        {
            transformation.z = tfmList2.at(2).toDouble(&ok);
            if (!ok)
                return false;
            transformation.x = transformation.y = 0;
        }
        else
        {
            transformation.x = tfmList2.at(2).toDouble(&ok);
            if (!ok)
                return false;
            transformation.y = tfmList2.at(3).toDouble(&ok);
            if (!ok)
                return false;
            transformation.z = 0;
        }

        transformations.append(transformation);
        bypasses.append(bypass);
    }

    setImage(image);
    setPosition(position);
    setColorCompositionMode(colorCompositionMode);
    setOpacity(opacity);
    setTransformations(transformations, bypasses);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    QStringList tfms;
    if (mTransformations.size() > 0)
    {
        AffineTransformation t;
        for (int i = 0; i < mTransformations.size(); i++)
        {
            t = mTransformations.at(i);
            if (t.type == Translation)
                tfms.append(QString("translation ") +
                            (mBypasses.at(i) ? "true " : "false ") +
                            QString::number(t.x, 'f', 2) + " " +
                            QString::number(t.y, 'f', 2));
            else if (t.type == Scaling)
                tfms.append(QString("scaling ") +
                            (mBypasses.at(i) ? "true " : "false ") +
                            QString::number(t.x, 'f', 2) + " " +
                            QString::number(t.y, 'f', 2));
            else if (t.type == Rotation)
                tfms.append(QString("rotation ") +
                            (mBypasses.at(i) ? "true " : "false ") +
                            QString::number(t.z, 'f', 2));
            else
                tfms.append(QString("shearing ") +
                            (mBypasses.at(i) ? "true " : "false ") +
                            QString::number(t.x, 'f', 2) + " " +
                            QString::number(t.y, 'f', 2));
        }
    }

    s.setValue("image", mImage);
    s.setValue("position", mPosition == Front ? "front" : (mPosition == Behind ? "behind" : "inside"));
    s.setValue("colorcompositionmode", colorCompositionModeEnumToString(mColorCompositionMode));
    s.setValue("opacity", mOpacity);
    s.setValue("geometrictransformations", tfms.join(", "));
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setImage(mImage);
    fw->setPosition(mPosition);
    fw->setColorCompositionMode(mColorCompositionMode);
    fw->setOpacity(mOpacity);
    fw->setTransformations(mTransformations, mBypasses);
    connect(this, SIGNAL(imageChanged(QImage)), fw, SLOT(setImage(QImage)));
    connect(this, SIGNAL(positionChanged(Filter::Position)), fw, SLOT(setPosition(Filter::Position)));
    connect(this, SIGNAL(colorCompositionModeChanged(ColorCompositionMode)),
            fw, SLOT(setColorCompositionMode(ColorCompositionMode)));
    connect(this, SIGNAL(opacityChanged(int)), fw, SLOT(setOpacity(int)));
    connect(this, SIGNAL(transformationsChanged(QList<AffineTransformation>,QList<bool>)),
            fw, SLOT(setTransformations(QList<AffineTransformation>,QList<bool>)));
    connect(fw, SIGNAL(imageChanged(QImage)), this, SLOT(setImage(QImage)));
    connect(fw, SIGNAL(positionChanged(Filter::Position)), this, SLOT(setPosition(Filter::Position)));
    connect(fw, SIGNAL(colorCompositionModeChanged(ColorCompositionMode)),
            this, SLOT(setColorCompositionMode(ColorCompositionMode)));
    connect(fw, SIGNAL(opacityChanged(int)), this, SLOT(setOpacity(int)));
    connect(fw, SIGNAL(transformationsChanged(QList<AffineTransformation>,QList<bool>)),
            this, SLOT(setTransformations(QList<AffineTransformation>,QList<bool>)));
    return fw;
}

void Filter::setImage(const QImage &i)
{
    if (i == mImage)
        return;
    mImage = i;
    emit imageChanged(i);
    emit parametersChanged();
}

void Filter::setPosition(Filter::Position v)
{
    if (v == mPosition)
        return;
    mPosition = v;
    emit positionChanged(v);
    emit parametersChanged();
}

void Filter::setColorCompositionMode(ColorCompositionMode v)
{
    if (v == mColorCompositionMode)
        return;
    mColorCompositionMode = v;
    emit colorCompositionModeChanged(v);
    emit parametersChanged();
}

void Filter::setOpacity(int v)
{
    if (v == mOpacity)
        return;
    mOpacity = v;
    emit opacityChanged(v);
    emit parametersChanged();
}

void Filter::setTransformations(const QList<AffineTransformation> &t, const QList<bool> &b)
{
    if (t == mTransformations && b == mBypasses)
        return;
    mTransformations = t;
    mBypasses = b;
    emit transformationsChanged(t, b);
    emit parametersChanged();
}
