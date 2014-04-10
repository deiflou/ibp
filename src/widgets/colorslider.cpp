#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QDebug>

#include "colorslider.h"

namespace anitools {
namespace widgets {

ColorSlider::ColorSlider(QWidget *parent) :
    QWidget(parent),
    mColorModel(ColorModel_BGR),
    mColorchannel(ColorChannel_Red),
    mOrientation(Qt::Horizontal)
{
    mColor[ColorChannel_Red] = 255;
    mColor[ColorChannel_Green] = 0;
    mColor[ColorChannel_Blue] = 0;
    mColor[ColorChannel_Alpha] = 255;

    updateImageData();
}

QColor ColorSlider::color() const
{
    BGRA bgra;
    convertColor[mColorModel][ColorModel_BGR](mColor, (unsigned char*)&bgra);
    if (mColorModel != ColorModel_BGR)
        bgra.a = 255;
    return QColor(bgra.r, bgra.g, bgra.b, bgra.a);
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
    return mColor[mColorchannel];
}

ColorModel ColorSlider::colorModel() const
{
    return mColorModel;
}

ColorChannel ColorSlider::Colorchannel() const
{
    return mColorchannel;
}

Qt::Orientation ColorSlider::orientation() const
{
    return mOrientation;
}

void ColorSlider::updateImageData()
{
    int i;
    BGRA * imageData = mBGRAImageData;
    switch (mColorModel)
    {
    case ColorModel_HSV:
        switch (mColorchannel)
        {
        case ColorChannel_Hue:
            break;
        case ColorChannel_Saturation:
            break;
        default:
            break;
        }
        break;
    case ColorModel_HSL:
        switch (mColorchannel)
        {
        case ColorChannel_Hue:
            break;
        case ColorChannel_Saturation:
            break;
        default:
            break;
        }
        break;
    case ColorModel_Lab:
        switch (mColorchannel)
        {
        case ColorChannel_CIEL:
            break;
        case ColorChannel_CIEa:
            break;
        default:
            break;
        }
        break;
    case ColorModel_CMYK:
        switch (mColorchannel)
        {
        case ColorChannel_Cyan:
            break;
        case ColorChannel_Magenta:
            break;
        case ColorChannel_Yellow:
            break;
        default:
            break;
        }
        break;
    default:
        switch (mColorchannel)
        {
        case ColorChannel_Red:
            for (i = 0; i < 256; i++, imageData++)
            {
                imageData->r = i;
                imageData->g = mColor[ColorChannel_Green];
                imageData->b = mColor[ColorChannel_Blue];
                imageData->a = 255;
            }
            break;
        case ColorChannel_Green:
            for (i = 0; i < 256; i++, imageData++)
            {
                imageData->r = mColor[ColorChannel_Red];
                imageData->g = i;
                imageData->b = mColor[ColorChannel_Blue];
                imageData->a = 255;
            }
            break;
        case ColorChannel_Blue:
            for (i = 0; i < 256; i++, imageData++)
            {
                imageData->r = mColor[ColorChannel_Red];
                imageData->g = mColor[ColorChannel_Green];
                imageData->b = i;
                imageData->a = 255;
            }
            break;
        default:
            for (i = 0; i < 256; i++, imageData++)
            {
                imageData->r = mColor[ColorChannel_Red];
                imageData->g = mColor[ColorChannel_Green];
                imageData->b = mColor[ColorChannel_Blue];
                imageData->a = i;
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

    // Color
    QImage img((unsigned char *)mBGRAImageData,
               mOrientation == Qt::Horizontal ? 256 : 1,
               mOrientation == Qt::Horizontal ? 1 : 256,
               QImage::Format_ARGB32);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.drawImage(this->rect(), img);

    e->accept();
}

void ColorSlider::mousePressEvent(QMouseEvent *e)
{
    e->accept();
}

void ColorSlider::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
}

void ColorSlider::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();
}

void ColorSlider::keyPressEvent(QKeyEvent *e)
{
    e->accept();
}

void ColorSlider::focusInEvent(QFocusEvent *e)
{
    e->accept();
}

void ColorSlider::focusOutEvent(QFocusEvent *e)
{
    e->accept();
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
    convertColor[ColorModel_BGR][mColorModel]((unsigned char *)&color, mColor);
    updateImageDataAndPaint();
}

void ColorSlider::setValue(unsigned char v)
{
    mColor[mColorchannel] = v;
    updateImageDataAndPaint();
}

void ColorSlider::setColorModel(ColorModel cm)
{
    if (cm == mColorModel)
        return;
    if (cm < ColorModel_BGR || cm > ColorModel_CMYK)
        return;

    convertColor[mColorModel][cm](mColor, mColor);
    updateImageDataAndPaint();
}

void ColorSlider::setColorChannel(ColorChannel cc)
{
    if (cc == mColorchannel)
        return;
    if (cc < 0 || cc > 3)
        return;
    if (cc == 3 && (mColorModel != ColorModel_BGR && mColorModel != ColorModel_CMYK))
        return;

    mColorchannel = cc;
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
