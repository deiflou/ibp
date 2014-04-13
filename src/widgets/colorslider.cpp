#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QDebug>

#include "colorslider.h"
#include "../imgproc/lut.h"

namespace anitools {
namespace widgets {

ColorSlider::ColorSlider(QWidget *parent) :
    QWidget(parent),
    mColorModel(ColorModel_BGR),
    mColorChannel(ColorChannel_Red),
    mOrientation(Qt::Horizontal),
    mBackgroundImage(2, 2, QImage::Format_RGB888)
{
    setFocusPolicy(Qt::StrongFocus);

    mColor[ColorChannel_Red] = 255;
    mColor[ColorChannel_Green] = 0;
    mColor[ColorChannel_Blue] = 0;
    mColor[ColorChannel_Alpha] = 255;

    updateImageData();
}

QColor ColorSlider::color() const
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

void ColorSlider::color(unsigned char *x, unsigned char *y, unsigned char *z, unsigned char *w) const
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

void ColorSlider::color(unsigned int *c) const
{
    if (!c)
        return;
    memcpy(c, mColor, 4 * sizeof(unsigned char));
}

unsigned char ColorSlider::value() const
{
    return mColor[mColorChannel];
}

ColorModel ColorSlider::colorModel() const
{
    return mColorModel;
}

ColorChannel ColorSlider::colorChannel() const
{
    return mColorChannel;
}

Qt::Orientation ColorSlider::orientation() const
{
    return mOrientation;
}

void ColorSlider::updateImageData()
{
    int i;
    BGRA * BGRAImageData = mBGRAImageData;
    unsigned char * realImageData = mRealImageData;
    switch (mColorModel)
    {
    case ColorModel_HSV:
        switch (mColorChannel)
        {
        case ColorChannel_Hue:
            for (i = 0; i < 256; i++, realImageData += 3)
            {
                realImageData[ColorChannel_Hue] = i;
                realImageData[ColorChannel_Saturation] = mColor[ColorChannel_Saturation];
                realImageData[ColorChannel_Value] = mColor[ColorChannel_Value];
            }
            break;
        case ColorChannel_Saturation:
            for (i = 0; i < 256; i++, realImageData += 3)
            {
                realImageData[ColorChannel_Hue] = mColor[ColorChannel_Hue];
                realImageData[ColorChannel_Saturation] = i;
                realImageData[ColorChannel_Value] = mColor[ColorChannel_Value];
            }
            break;
        default:
            for (i = 0; i < 256; i++, realImageData += 3)
            {
                realImageData[ColorChannel_Hue] = mColor[ColorChannel_Hue];
                realImageData[ColorChannel_Saturation] = mColor[ColorChannel_Saturation];
                realImageData[ColorChannel_Value] = i;
            }
            break;
        }
        convertHSVToBGR(mRealImageData, (unsigned char *)mBGRAImageData, 256);
        break;
    case ColorModel_HSL:
        switch (mColorChannel)
        {
        case ColorChannel_Hue:
            for (i = 0; i < 256; i++, realImageData += 3)
            {
                realImageData[ColorChannel_Hue] = i;
                realImageData[ColorChannel_Saturation] = mColor[ColorChannel_Saturation];
                realImageData[ColorChannel_Lightness] = mColor[ColorChannel_Lightness];
            }
            break;
        case ColorChannel_Saturation:
            for (i = 0; i < 256; i++, realImageData += 3)
            {
                realImageData[ColorChannel_Hue] = mColor[ColorChannel_Hue];
                realImageData[ColorChannel_Saturation] = i;
                realImageData[ColorChannel_Lightness] = mColor[ColorChannel_Lightness];
            }
            break;
        default:
            for (i = 0; i < 256; i++, realImageData += 3)
            {
                realImageData[ColorChannel_Hue] = mColor[ColorChannel_Hue];
                realImageData[ColorChannel_Saturation] = mColor[ColorChannel_Saturation];
                realImageData[ColorChannel_Lightness] = i;
            }
            break;
        }
        convertHSLToBGR(mRealImageData, (unsigned char *)mBGRAImageData, 256);
        break;
    case ColorModel_Lab:
        switch (mColorChannel)
        {
        case ColorChannel_CIEL:
            for (i = 0; i < 256; i++, realImageData += 3)
            {
                realImageData[ColorChannel_CIEL] = i;
                realImageData[ColorChannel_CIEa] = mColor[ColorChannel_CIEa];
                realImageData[ColorChannel_CIEb] = mColor[ColorChannel_CIEb];
            }
            break;
        case ColorChannel_CIEa:
            for (i = 0; i < 256; i++, realImageData += 3)
            {
                realImageData[ColorChannel_CIEL] = mColor[ColorChannel_CIEL];
                realImageData[ColorChannel_CIEa] = i;
                realImageData[ColorChannel_CIEb] = mColor[ColorChannel_CIEb];
            }
            break;
        default:
            for (i = 0; i < 256; i++, realImageData += 3)
            {
                realImageData[ColorChannel_CIEL] = mColor[ColorChannel_CIEL];
                realImageData[ColorChannel_CIEa] = mColor[ColorChannel_CIEa];
                realImageData[ColorChannel_CIEb] = i;
            }
            break;
        }
        convertLabToBGR(mRealImageData, (unsigned char *)mBGRAImageData, 256);
        break;
    case ColorModel_CMYK:
        switch (mColorChannel)
        {
        case ColorChannel_Cyan:
            for (i = 0; i < 256; i++, realImageData += 4)
            {
                realImageData[ColorChannel_Cyan] = i;
                realImageData[ColorChannel_Magenta] = mColor[ColorChannel_Magenta];
                realImageData[ColorChannel_Yellow] = mColor[ColorChannel_Yellow];
                realImageData[ColorChannel_Black] = mColor[ColorChannel_Black];
            }
            break;
        case ColorChannel_Magenta:
            for (i = 0; i < 256; i++, realImageData += 4)
            {
                realImageData[ColorChannel_Cyan] = mColor[ColorChannel_Cyan];
                realImageData[ColorChannel_Magenta] = i;
                realImageData[ColorChannel_Yellow] = mColor[ColorChannel_Yellow];
                realImageData[ColorChannel_Black] = mColor[ColorChannel_Black];
            }
            break;
        case ColorChannel_Yellow:
            for (i = 0; i < 256; i++, realImageData += 4)
            {
                realImageData[ColorChannel_Cyan] = mColor[ColorChannel_Cyan];
                realImageData[ColorChannel_Magenta] = mColor[ColorChannel_Magenta];
                realImageData[ColorChannel_Yellow] = i;
                realImageData[ColorChannel_Black] = mColor[ColorChannel_Black];
            }
            break;
        default:
            for (i = 0; i < 256; i++, realImageData += 4)
            {
                realImageData[ColorChannel_Cyan] = mColor[ColorChannel_Cyan];
                realImageData[ColorChannel_Magenta] = mColor[ColorChannel_Magenta];
                realImageData[ColorChannel_Yellow] = mColor[ColorChannel_Yellow];
                realImageData[ColorChannel_Black] = i;
            }
            break;
        }
        convertCMYKToBGR(mRealImageData, (unsigned char *)mBGRAImageData, 256);
        break;
    default:
        switch (mColorChannel)
        {
        case ColorChannel_Red:
            for (i = 0; i < 256; i++, BGRAImageData++)
            {
                BGRAImageData->r = i;
                BGRAImageData->g = mColor[ColorChannel_Green];
                BGRAImageData->b = mColor[ColorChannel_Blue];
                BGRAImageData->a = 255;
            }
            break;
        case ColorChannel_Green:
            for (i = 0; i < 256; i++, BGRAImageData++)
            {
                BGRAImageData->r = mColor[ColorChannel_Red];
                BGRAImageData->g = i;
                BGRAImageData->b = mColor[ColorChannel_Blue];
                BGRAImageData->a = 255;
            }
            break;
        case ColorChannel_Blue:
            for (i = 0; i < 256; i++, BGRAImageData++)
            {
                BGRAImageData->r = mColor[ColorChannel_Red];
                BGRAImageData->g = mColor[ColorChannel_Green];
                BGRAImageData->b = i;
                BGRAImageData->a = 255;
            }
            break;
        default:
            for (i = 0; i < 256; i++, BGRAImageData++)
            {
                BGRAImageData->r = mColor[ColorChannel_Red];
                BGRAImageData->g = mColor[ColorChannel_Green];
                BGRAImageData->b = mColor[ColorChannel_Blue];
                BGRAImageData->a = i;
            }
            break;
        }
        break;
    }
}

void ColorSlider::updateImageDataAndPaint()
{
    updateImageData();
    repaint();
}

void ColorSlider::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::Antialiasing);

    // Background
    if (mColorModel == ColorModel_BGR)
    {
        p.setBrushOrigin(1, 1);
        int checkerboardColor1 = palette().color(QPalette::Light).rgb();
        int checkerboardColor2 = palette().color(QPalette::Midlight).rgb();
        mBackgroundImage.setPixel(0, 0, checkerboardColor1);
        mBackgroundImage.setPixel(1, 1, checkerboardColor1);
        mBackgroundImage.setPixel(0, 1, checkerboardColor2);
        mBackgroundImage.setPixel(1, 0, checkerboardColor2);
        QBrush checkerboardBrush = QBrush(mBackgroundImage);
        checkerboardBrush.setTransform(QTransform(4, 0, 0, 0, 4, 0, 0, 0, 1));
        p.fillRect(this->rect().adjusted(1, 1, -1, -1), checkerboardBrush);
    }
    // Color
    QImage img((unsigned char *)mBGRAImageData,
               mOrientation == Qt::Horizontal ? 256 : 1,
               mOrientation == Qt::Horizontal ? 1 : 256,
               QImage::Format_ARGB32);
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
    int hPos;
    QPolygon poly;
    p.setBrush(palette().color(QPalette::Dark));
    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing, false);
    if (mOrientation == Qt::Horizontal)
    {
        hPos = qRound(mColor[mColorChannel] * (width() - 3) / 255.0) + 1;
        poly.append(QPoint(hPos, height() - 4));
        poly.append(QPoint(hPos - 4, height()));
        poly.append(QPoint(hPos + 4, height()));
        p.drawPolygon(poly);
    }
    else
    {
        hPos = qRound(mColor[mColorChannel] * (height() - 3) / 255.0) + 1;
        poly.append(QPoint(4, hPos));
        poly.append(QPoint(0, hPos - 4));
        poly.append(QPoint(0, hPos + 4));
        p.drawPolygon(poly);
    }

    e->accept();
}

void ColorSlider::mousePressEvent(QMouseEvent *e)
{
    mouseMoveEvent(e);

    e->accept();
}

void ColorSlider::mouseMoveEvent(QMouseEvent *e)
{
    if (!(e->buttons() & Qt::LeftButton))
        return;

    if (mOrientation == Qt::Horizontal)
        setValue(qBound<int>(0, qRound((e->x() - 1) * 255.0 / (width() - 3)), 255));
    else
        setValue(qBound<int>(0, qRound((e->y() - 1) * 255.0 / (height() - 3)), 255));

    e->accept();
}

void ColorSlider::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_Left && e->key() != Qt::Key_Right &&
        e->key() != Qt::Key_Up && e->key() != Qt::Key_Down)
        return;

    if (e->key() == Qt::Key_Left || e->key() == Qt::Key_Down)
        setValue(qBound<int>(0, value() - 1, 255));
    else
        setValue(qBound<int>(0, value() + 1, 255));
    e->accept();
}

void ColorSlider::wheelEvent(QWheelEvent *e)
{
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
    }
    e->accept();
}

void ColorSlider::focusInEvent(QFocusEvent *e)
{
    Q_UNUSED(e)
    update();
}

void ColorSlider::focusOutEvent(QFocusEvent *e)
{
    Q_UNUSED(e)
    update();
}

void ColorSlider::setColor(unsigned char x, unsigned char y, unsigned char z, unsigned char w)
{
    mColor[0] = x;
    mColor[1] = y;
    mColor[2] = z;
    mColor[3] = w;
    updateImageDataAndPaint();
}

void ColorSlider::setColor(unsigned int c)
{
    memcpy(mColor, &c, 4 * sizeof(unsigned char));
    updateImageDataAndPaint();
}

void ColorSlider::setColor(const QColor &c)
{
    unsigned int color = c.rgba();
    convertColors[ColorModel_BGR][mColorModel]((unsigned char *)&color, mColor, 1);
    updateImageDataAndPaint();
}

void ColorSlider::setValue(int v)
{
    if (v == mColor[mColorChannel])
        return;
    if (v < 0 || v > 255)
        return;
    mColor[mColorChannel] = v;
    update();
    emit valueChanged(v);
}

void ColorSlider::setColorModel(ColorModel cm)
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
}

void ColorSlider::setColorChannel(ColorChannel cc)
{
    if (cc == mColorChannel)
        return;
    if (cc < 0 || cc > 3)
        return;
    if (cc == 3 && (mColorModel != ColorModel_BGR && mColorModel != ColorModel_CMYK))
        return;

    mColorChannel = cc;
    updateImageDataAndPaint();
}

void ColorSlider::setOrientation(Qt::Orientation o)
{
    if (o == mOrientation)
        return;
    mOrientation = o;
    updateImageDataAndPaint();
}

}}
