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

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QDebug>

#include "colorbox.h"
#include "../imgproc/lut.h"

namespace anitools {
namespace widgets {

ColorBox::ColorBox(QWidget *parent) :
    QWidget(parent),
    mColorModel(ColorModel_BGR),
    mColorChannel(ColorChannel_Red)
{
    setFocusPolicy(Qt::StrongFocus);

    mColor[ColorChannel_Red] = 255;
    mColor[ColorChannel_Green] = 0;
    mColor[ColorChannel_Blue] = 0;
    mColor[ColorChannel_Alpha] = 255;

    updateImageData();
}

QColor ColorBox::color() const
{
    if (mColorModel == ColorModel_BGR)
        return QColor(mColor[ColorChannel_Red], mColor[ColorChannel_Green],
                      mColor[ColorChannel_Blue], mColor[ColorChannel_Alpha]);
    else
    {
        BGRA bgra;
        convertColors[mColorModel][ColorModel_BGR](mColor, (unsigned char*)&bgra, 1);
        return QColor(bgra.r, bgra.g, bgra.b, 255);
    }

}

void ColorBox::color(unsigned char *x, unsigned char *y, unsigned char *z, unsigned char *w) const
{
    if (x)
        *x = mColor[0];
    if (y)
        *y = mColor[1];
    if (z)
        *z = mColor[2];
    if (w)
        *w = mColor[3];
}

void ColorBox::color(unsigned int *c) const
{
    if (!c)
        return;
    memcpy(c, mColor, 4 * sizeof(unsigned char));
}

unsigned char ColorBox::value() const
{
    return mColor[mColorChannel];
}

ColorModel ColorBox::colorModel() const
{
    return mColorModel;
}

ColorChannel ColorBox::colorChannel() const
{
    return mColorChannel;
}

void ColorBox::updateImageData()
{
    int x, y;
    BGRA * BGRAImageData = mBGRAImageData;
    unsigned char * realImageData = mRealImageData;
    switch (mColorModel)
    {
    case ColorModel_HSV:
        switch (mColorChannel)
        {
        case ColorChannel_Hue:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 3)
                {
                    realImageData[ColorChannel_Hue] = mColor[ColorChannel_Hue];
                    realImageData[ColorChannel_Saturation] = x;
                    realImageData[ColorChannel_Value] = 255 - y;
                }
            }
            break;
        case ColorChannel_Saturation:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 3)
                {
                    realImageData[ColorChannel_Hue] = x;
                    realImageData[ColorChannel_Saturation] = mColor[ColorChannel_Saturation];
                    realImageData[ColorChannel_Value] = 255 - y;
                }
            }
            break;
        default:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 3)
                {
                    realImageData[ColorChannel_Hue] = x;
                    realImageData[ColorChannel_Saturation] = 255 - y;
                    realImageData[ColorChannel_Value] = mColor[ColorChannel_Value];
                }
            }
            break;
        }
        convertHSVToBGR(mRealImageData, (unsigned char *)mBGRAImageData, 256 * 256);
        break;
    case ColorModel_HSL:
        switch (mColorChannel)
        {
        case ColorChannel_Hue:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 3)
                {
                    realImageData[ColorChannel_Hue] = mColor[ColorChannel_Hue];
                    realImageData[ColorChannel_Saturation] = x;
                    realImageData[ColorChannel_Lightness] = 255 - y;
                }
            }
            break;
        case ColorChannel_Saturation:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 3)
                {
                    realImageData[ColorChannel_Hue] = x;
                    realImageData[ColorChannel_Saturation] = mColor[ColorChannel_Saturation];
                    realImageData[ColorChannel_Lightness] = 255 - y;
                }
            }
            break;
        default:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 3)
                {
                    realImageData[ColorChannel_Hue] = x;
                    realImageData[ColorChannel_Saturation] = 255 - y;
                    realImageData[ColorChannel_Lightness] = mColor[ColorChannel_Lightness];
                }
            }
            break;
        }
        convertHSLToBGR(mRealImageData, (unsigned char *)mBGRAImageData, 256 * 256);
        break;
    case ColorModel_Lab:
        switch (mColorChannel)
        {
        case ColorChannel_CIEL:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 3)
                {
                    realImageData[ColorChannel_CIEL] = mColor[ColorChannel_CIEL];
                    realImageData[ColorChannel_CIEa] = x;
                    realImageData[ColorChannel_CIEb] = 255 - y;
                }
            }
            break;
        case ColorChannel_CIEa:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 3)
                {
                    realImageData[ColorChannel_CIEL] = 255 - y;
                    realImageData[ColorChannel_CIEa] = mColor[ColorChannel_CIEa];
                    realImageData[ColorChannel_CIEb] = x;
                }
            }
            break;
        default:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 3)
                {
                    realImageData[ColorChannel_CIEL] = 255 - y;
                    realImageData[ColorChannel_CIEa] = x;
                    realImageData[ColorChannel_CIEb] = mColor[ColorChannel_CIEb];
                }
            }
            break;
        }
        convertLabToBGR(mRealImageData, (unsigned char *)mBGRAImageData, 256 * 256);
        break;
    case ColorModel_CMYK:
        switch (mColorChannel)
        {
        case ColorChannel_Cyan:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 4)
                {
                    realImageData[ColorChannel_Cyan] = mColor[ColorChannel_Cyan];
                    realImageData[ColorChannel_Magenta] = x;
                    realImageData[ColorChannel_Yellow] = 255 - y;
                    realImageData[ColorChannel_Black] = mColor[ColorChannel_Black];
                }
            }
            break;
        case ColorChannel_Magenta:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 4)
                {
                    realImageData[ColorChannel_Cyan] = x;
                    realImageData[ColorChannel_Magenta] = mColor[ColorChannel_Magenta];
                    realImageData[ColorChannel_Yellow] = 255 - y;
                    realImageData[ColorChannel_Black] = mColor[ColorChannel_Black];
                }
            }
            break;
        default:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, realImageData += 4)
                {
                    realImageData[ColorChannel_Cyan] = x;
                    realImageData[ColorChannel_Magenta] = 255 - y;
                    realImageData[ColorChannel_Yellow] = mColor[ColorChannel_Yellow];
                    realImageData[ColorChannel_Black] = mColor[ColorChannel_Black];
                }
            }
            break;
        }
        convertCMYKToBGR(mRealImageData, (unsigned char *)mBGRAImageData, 256 * 256);
        break;
    default:
        switch (mColorChannel)
        {
        case ColorChannel_Red:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, BGRAImageData++)
                {
                    BGRAImageData->r = mColor[ColorChannel_Red];
                    BGRAImageData->g = 255 - y;
                    BGRAImageData->b = x;
                    BGRAImageData->a = 255;
                }
            }
            break;
        case ColorChannel_Green:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, BGRAImageData++)
                {
                    BGRAImageData->r = 255 - y;
                    BGRAImageData->g = mColor[ColorChannel_Green];
                    BGRAImageData->b = x;
                    BGRAImageData->a = 255;
                }
            }
            break;
        default:
            for (y = 0; y < 256; y++)
            {
                for (x = 0; x < 256; x++, BGRAImageData++)
                {
                    BGRAImageData->r = 255 - y;
                    BGRAImageData->g = x;
                    BGRAImageData->b = mColor[ColorChannel_Blue];
                    BGRAImageData->a = 255;
                }
            }
            break;
        }
        break;
    }
}

void ColorBox::updateImageDataAndPaint()
{
    updateImageData();
    update();
}

void ColorBox::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::Antialiasing);

    // Color
    QImage img((unsigned char *)mBGRAImageData, 256, 256, QImage::Format_ARGB32);
    p.drawImage(this->rect().adjusted(1, 1, -1, -1), img);
    // Border
    p.setBrush(Qt::NoBrush);
    if (hasFocus())
    {
        p.setPen(QPen(palette().color(QPalette::Highlight), 2));
        p.drawRoundedRect(this->rect().adjusted(1, 1, -1, -1), 2, 2);
    }
    else
    {
        p.setPen(palette().color(QPalette::Dark));
        p.drawRoundedRect(this->rect(), 2, 2);
    }
    // Handle
    int x, y;
    if (mColorModel == ColorModel_Lab)
    {
        switch (mColorChannel)
        {
        case ColorChannel_CIEL:
            x = qRound(mColor[ColorChannel_CIEa] * (width() - 3) / 255.) + 1;
            y = qRound((255 - mColor[ColorChannel_CIEb]) * (height() - 3) / 255.) + 1;
            break;
        case ColorChannel_CIEa:
            x = qRound(mColor[ColorChannel_CIEb] * (width() - 3) / 255.) + 1;
            y = qRound((255 - mColor[ColorChannel_CIEL]) * (height() - 3) / 255.) + 1;
            break;
        case ColorChannel_CIEb:
            x = qRound(mColor[ColorChannel_CIEa] * (width() - 3) / 255.) + 1;
            y = qRound((255 - mColor[ColorChannel_CIEL]) * (height() - 3) / 255.) + 1;
            break;
        default:
            x = y = 0;
            break;
        }
    }
    else
    {
        switch (mColorChannel)
        {
        case ColorChannel_0:
            x = qRound(mColor[ColorChannel_1] * (width() - 3) / 255.) + 1;
            y = qRound((255 - mColor[ColorChannel_2]) * (height() - 3) / 255.) + 1;
            break;
        case ColorChannel_1:
            x = qRound(mColor[ColorChannel_0] * (width() - 3) / 255.) + 1;
            y = qRound((255 - mColor[ColorChannel_2]) * (height() - 3) / 255.) + 1;
            break;
        case ColorChannel_2:
            x = qRound(mColor[ColorChannel_0] * (width() - 3) / 255.) + 1;
            y = qRound((255 - mColor[ColorChannel_1]) * (height() - 3) / 255.) + 1;
            break;
        default:
            x = y = 0;
            break;
        }
    }
    p.setBrush(Qt::NoBrush);
    p.setPen(QColor(0, 0, 0, 128));
    p.drawEllipse(QPoint(x, y), 5, 5);
    p.setPen(QColor(255, 255, 255, 128));
    p.drawEllipse(QPoint(x, y), 4, 4);

    e->accept();
}

void ColorBox::mousePressEvent(QMouseEvent *e)
{
    mouseMoveEvent(e);

    e->accept();
}

void ColorBox::mouseMoveEvent(QMouseEvent *e)
{
    if (!(e->buttons() & Qt::LeftButton))
        return;

    int x, y;
    x = qBound<int>(0, qRound((e->x() - 1) * 255.0 / (width() - 3)), 255);
    y = qBound<int>(0, qRound((e->y() - 1) * 255.0 / (height() - 3)), 255);

    if (mColorModel == ColorModel_Lab)
    {
        switch (mColorChannel)
        {
        case ColorChannel_CIEL:
            setColor(mColor[ColorChannel_CIEL], x, 255 - y, mColor[ColorChannel_3]);
            break;
        case ColorChannel_CIEa:
            setColor(255 - y, mColor[ColorChannel_CIEa], x, mColor[ColorChannel_3]);
            break;
        case ColorChannel_CIEb:
            setColor(255 - y, x, mColor[ColorChannel_CIEb], mColor[ColorChannel_3]);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (mColorChannel)
        {
        case ColorChannel_0:
            setColor(mColor[ColorChannel_0], x, 255 - y, mColor[ColorChannel_3]);
            break;
        case ColorChannel_1:
            setColor(x, mColor[ColorChannel_1], 255 - y, mColor[ColorChannel_3]);
            break;
        case ColorChannel_2:
            setColor(x, 255 - y, mColor[ColorChannel_2], mColor[ColorChannel_3]);
            break;
        default:
            break;
        }
    }

    e->accept();
}

void ColorBox::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_Left && e->key() != Qt::Key_Right &&
        e->key() != Qt::Key_Up && e->key() != Qt::Key_Down)
        return;

    e->accept();
}

void ColorBox::wheelEvent(QWheelEvent *e)
{/*
    QPoint numPixels = e->pixelDelta();
    QPoint numDegrees = e->angleDelta() / 8;
    if (!numPixels.isNull())
    {
        setValue(qBound<int>(0, value() + numPixels.y(), 255));
    }
    else if (!numDegrees.isNull())
    {
        QPoint numSteps = numDegrees / 15;
        setValue(qBound<int>(0, value() + numSteps.y(), 255));
    }*/
    e->accept();
}

void ColorBox::focusInEvent(QFocusEvent *e)
{
    Q_UNUSED(e)
    update();
}

void ColorBox::focusOutEvent(QFocusEvent *e)
{
    Q_UNUSED(e)
    update();
}

void ColorBox::setColor(unsigned char x, unsigned char y, unsigned char z, unsigned char w)
{
    mColor[0] = x;
    mColor[1] = y;
    mColor[2] = z;
    mColor[3] = w;
    updateImageDataAndPaint();
    emit colorChanged();
}

void ColorBox::setColor(unsigned int c)
{
    memcpy(mColor, &c, 4 * sizeof(unsigned char));
    updateImageDataAndPaint();
    emit colorChanged();
}

void ColorBox::setColor(const QColor &c)
{
    unsigned int color = c.rgba();
    convertColors[ColorModel_BGR][mColorModel]((unsigned char *)&color, mColor, 1);
    updateImageDataAndPaint();
    emit colorChanged();
}

void ColorBox::setValue(int v)
{
    if (v == mColor[mColorChannel])
        return;
    if (v < 0 || v > 255)
        return;
    mColor[mColorChannel] = v;
    update();
    emit valueChanged(v);
    emit colorChanged();
}

void ColorBox::setColorModel(ColorModel cm)
{
    if (cm == mColorModel)
        return;
    if (cm < ColorModel_BGR || cm > ColorModel_CMYK)
        return;

    convertColors[mColorModel][cm](mColor, mColor, 1);
    mColorModel = cm;
    mColorChannel = (ColorChannel)0;
    updateImageDataAndPaint();
    emit valueChanged(mColor[0]);
    emit colorChanged();
}

void ColorBox::setColorChannel(ColorChannel cc)
{
    if (cc == mColorChannel)
        return;
    if (cc < 0 || cc > 2)
        return;

    mColorChannel = cc;
    updateImageDataAndPaint();
}

}}
