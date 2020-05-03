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

#include <QPainter>
#include <QStyle>
#include <math.h>

#include "hslcolorreplacementcurvespaintdelegate.h"
#include "../imgproc/colorconversion.h"
#include "../imgproc/lut.h"
#include "../imgproc/pixelblending.h"
#include "../misc/util.h"

namespace ibp {
namespace widgets {

HSLColorReplacementCurvesPaintDelegate::HSLColorReplacementCurvesPaintDelegate(QObject *parent) :
    mCheckerboardImage(2, 2, QImage::Format_RGB888),
    mGradientImage(256, 1, QImage::Format_ARGB32),
    mCorrectedGradientImage(256, 1, QImage::Format_ARGB32),
    mCorrectedHue(0),
    mCorrectedSaturation(0),
    mCorrectedLightness(0),
    mChannel(ColorChannel_Hue),
    mIsInverted(false),
    mColorize(false)
{
    setParent(parent);

    HSL c;
    c.h = 0;
    c.s = 255;
    c.l = 128;

    BGRA * bits = (BGRA *)mGradientImage.bits();
    BGRA * bits2 = (BGRA *)mCorrectedGradientImage.bits();
    for (int x = 0; x < 256; x++, bits++, bits2++)
        bits->a = bits2->a = 255;

    setColor(c);
}

HSLColorReplacementCurvesPaintDelegate::~HSLColorReplacementCurvesPaintDelegate()
{

}

void HSLColorReplacementCurvesPaintDelegate::update(UpdateEvent e, const Curves *w, const QRect & r)
{
    Q_UNUSED(r)
    Q_UNUSED(w)

    if (e == PeriodicChanged || e == InterpolationModeChanged || e == KnotsChanged)
    {
        BGRA * bits = (BGRA *)mCorrectedGradientImage.bits();
        if (mIsInverted)
            for (int x = 0; x < 256; x++, bits++)
                bits->a = 255 - round(((Curves *)w)->valueAt(x / 255.) * 255.);
        else
            for (int x = 0; x < 256; x++, bits++)
                bits->a = round(((Curves *)w)->valueAt(x / 255.) * 255.);
    }
}

void HSLColorReplacementCurvesPaintDelegate::paint(QPainter &p, const Curves *w, const QRect &r,
                                                   QStyle::State widgetState, const QPolygonF & graph,
                                                   const QVector<QPointF> & knotPos,
                                                   const QVector<QStyle::State> & knotStates,
                                                   const QSize & knotSize)
{
    paintBackground(p, w, r, widgetState);
    paintGraph(graph, p, w, r, widgetState);
    paintKnots(knotPos, knotStates, knotSize, p, w, r, widgetState);
}

void HSLColorReplacementCurvesPaintDelegate::paintBackground(QPainter &p, const Curves *w,
                                                             const QRect & r, QStyle::State widgetState)
{
    Q_UNUSED(widgetState)

    QRect gr = graphRect(r);

    // checkerboard background
    int checkerboardColor1 = w->palette().color(QPalette::Light).rgb();
    int checkerboardColor2 = w->palette().color(QPalette::Midlight).rgb();
    mCheckerboardImage.setPixel(0, 0, checkerboardColor1);
    mCheckerboardImage.setPixel(1, 1, checkerboardColor1);
    mCheckerboardImage.setPixel(0, 1, checkerboardColor2);
    mCheckerboardImage.setPixel(1, 0, checkerboardColor2);
    QBrush checkerboardBrush = mCheckerboardImage;
    checkerboardBrush.setTransform(QTransform(6, 0, 0, 0, 6, 0, 0, 0, 1));
    p.setBrush(checkerboardBrush);
    p.setPen(Qt::NoPen);
    p.setBrushOrigin(-w->offset() + gr.left(), gr.top());
    p.drawRect(r);

    QRectF rr(gr.x() - w->offset(), r.y(), gr.width() * w->zoomFactor(), r.height());

    // color gradient
    p.drawImage(rr, mGradientImage);
    p.drawImage(rr, mCorrectedGradientImage);
    if (w->isPeriodic())
    {
        p.drawImage(rr.translated(-rr.width(), 0), mGradientImage);
        p.drawImage(rr.translated(rr.width(), 0), mGradientImage);
        p.drawImage(rr.translated(-rr.width(), 0), mCorrectedGradientImage);
        p.drawImage(rr.translated(rr.width(), 0), mCorrectedGradientImage);
    }
}

void HSLColorReplacementCurvesPaintDelegate::paintGraph(const QPolygonF &g,
                                                        QPainter &p, const Curves *w,
                                                        const QRect & r, QStyle::State widgetState)
{
    Q_UNUSED(w)
    Q_UNUSED(r)

    int opacity = (widgetState & QStyle::State_MouseOver || widgetState & QStyle::State_HasFocus) ? 64 : 32;
    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(QColor(0, 0, 0, opacity), 2));
    p.drawPolyline(g.translated(1, 1));
    p.setPen(QPen(QColor(255, 255, 255, opacity), 2));
    p.drawPolyline(g);
}

void HSLColorReplacementCurvesPaintDelegate::paintKnots(const QVector<QPointF> & pts,
                                                        const QVector<QStyle::State> & sts,
                                                        const QSize & s,
                                                        QPainter & p, const Curves * w,
                                                        const QRect & r, QStyle::State widgetState)
{
    Q_UNUSED(r)

    const double radiusX = s.width() / 2.;
    const double radiusY = s.height() / 2.;
    QColor fillColor = w->palette().color(QPalette::Highlight);
    int opacity, fillOpacity;

    for (int i = 0; i < pts.size(); i++)
    {
        if (widgetState & QStyle::State_MouseOver || widgetState & QStyle::State_HasFocus)
        {
            if (sts[i] & QStyle::State_MouseOver)
            {
                if (sts[i] & QStyle::State_Sunken)
                {
                    opacity = 200;
                    fillOpacity = (sts[i] & QStyle::State_Selected) ? 255 : 0;
                }
                else
                {
                    opacity = 128;
                    fillOpacity = (sts[i] & QStyle::State_Selected) ? 200 : 0;
                }
            }
            else
            {
                opacity = 64;
                fillOpacity = (sts[i] & QStyle::State_Selected) ? 128 : 0;
            }
        }
        else
        {
            opacity = 32;
            fillOpacity = (sts[i] & QStyle::State_Selected) ? 64 : 0;
        }

        fillColor.setAlpha(fillOpacity);
        p.setBrush(fillColor);
        p.setPen(QPen(QColor(0, 0, 0, opacity), 1));
        p.drawEllipse(pts[i], radiusX, radiusY);

        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(QColor(255, 255, 255, opacity), 1));
        p.drawEllipse(pts[i], radiusX - 1, radiusY - 1);
    }
}

QRect HSLColorReplacementCurvesPaintDelegate::graphRect(const QRect &r) const
{
    return r.adjusted(0, 5, 0, -5);
}

HSL HSLColorReplacementCurvesPaintDelegate::color() const
{
    return mColor;
}

int HSLColorReplacementCurvesPaintDelegate::correctedValue(ColorChannel channel) const
{
    if (channel == ColorChannel_Hue)
        return mCorrectedHue;
    else if (channel == ColorChannel_Saturation)
        return mCorrectedSaturation;
    return mCorrectedLightness;
}

ColorChannel HSLColorReplacementCurvesPaintDelegate::channel() const
{
    return mChannel;
}

bool HSLColorReplacementCurvesPaintDelegate::isInverted() const
{
    return mIsInverted;
}

bool HSLColorReplacementCurvesPaintDelegate::colorize() const
{
    return mColorize;
}

void HSLColorReplacementCurvesPaintDelegate::setColor(const HSL &color)
{
    if (mColor.h == color.h && mColor.s == color.s && mColor.l == color.l)
        return;
    mColor = color;
    makeGradientImage();
    emit updateRequired();
}

void HSLColorReplacementCurvesPaintDelegate::setCorrectedValues(int h, int s, int l)
{
    if (h == mCorrectedHue && s == mCorrectedSaturation && l == mCorrectedLightness)
        return;
    if (!mColorize)
    {
        if (h < -180 || h > 180 || s < -100 || s > 100)
            return;
    }
    else
        if (h < 0 || h > 360 || s < 0 || s > 100)
            return;
    if (l < -100 || l > 100)
        return;
    mCorrectedHue = h;
    mCorrectedSaturation = s;
    mCorrectedLightness = l;
    makeGradientImage();
    emit updateRequired();
}

void HSLColorReplacementCurvesPaintDelegate::setCorrectedValue(ColorChannel channel, int v)
{
    if ((channel == ColorChannel_Hue && v == mCorrectedHue) ||
        (channel == ColorChannel_Saturation && v == mCorrectedSaturation) ||
        (channel == ColorChannel_Lightness && v == mCorrectedLightness))
        return;
    if (channel == ColorChannel_Hue)
    {
        if ((!mColorize && (v < -180 || v > 180)) || (mColorize && (v < 0 || v > 360)))
            return;
        mCorrectedHue = v;
    }
    else if (channel == ColorChannel_Saturation)
    {
        if ((!mColorize && (v < -100 || v > 100)) || (mColorize && (v < 0 || v > 100)))
            return;
        mCorrectedSaturation = v;
    }
    else
    {
        if (v < -100 || v > 100)
            return;
        mCorrectedLightness = v;
    }
    makeGradientImage();
    emit updateRequired();
}

void HSLColorReplacementCurvesPaintDelegate::setChannel(ColorChannel channel)
{
    if (mChannel == channel)
        return;
    mChannel = channel;
    makeGradientImage();
    emit updateRequired();
}

void HSLColorReplacementCurvesPaintDelegate::setInverted(bool v)
{
    if (mIsInverted == v)
        return;
    mIsInverted = v;

    BGRA * bits = (BGRA *)mCorrectedGradientImage.bits();
    for (int x = 0; x < 256; x++, bits++)
        bits->a = 255 - bits->a;

    emit updateRequired();
}

void HSLColorReplacementCurvesPaintDelegate::setColorize(bool v)
{
    if (mColorize == v)
        return;
    mColorize = v;
    makeGradientImage();
    emit updateRequired();
}

void HSLColorReplacementCurvesPaintDelegate::makeGradientImage()
{
    HSL hsl[256];
    register HSL * bitshsl = hsl;
    register BGRA * bits = (BGRA *)mGradientImage.bits();
    register BGRA * bits2 = (BGRA *)mCorrectedGradientImage.bits();
    register int h, s, l, x;

    // create untransformed hsl image
    if (mChannel == ColorChannel_Hue)
        for (x = 0; x < 256; x++, bitshsl++)
        {
            bitshsl->h = x;
            bitshsl->s = mColor.s;
            bitshsl->l = mColor.l;
        }
    else if (mChannel == ColorChannel_Saturation)
        for (x = 0; x < 256; x++, bitshsl++)
        {
            bitshsl->s = x;
            bitshsl->h = mColor.h;
            bitshsl->l = mColor.l;
        }
    else
        for (x = 0; x < 256; x++, bitshsl++)
        {
            bitshsl->l = x;
            bitshsl->h = mColor.h;
            bitshsl->s = mColor.s;
        }
    convertHSLToBGR((const unsigned char *)hsl, (unsigned char *)bits, 256);

    // create transformed image
    if (mCorrectedLightness != 0)
    {
        l = mCorrectedLightness * 255 / 100;
        if (mCorrectedLightness < 0)
        {
            l += 255;
            for (x = 0; x < 256; x++, bits++, bits2++)
            {
                bits2->r = lut01[bits->r][l];
                bits2->g = lut01[bits->g][l];
                bits2->b = lut01[bits->b][l];
            }
        }
        else
        {
            for (x = 0; x < 256; x++, bits++, bits2++)
            {
                bits2->r = bits->r + lut01[255 - bits->r][l];
                bits2->g = bits->g + lut01[255 - bits->g][l];
                bits2->b = bits->b + lut01[255 - bits->b][l];
            }
        }

    }
    else
    {
        for (x = 0; x < 256; x++, bits++, bits2++)
        {
            bits2->r = bits->r;
            bits2->g = bits->g;
            bits2->b = bits->b;
        }
    }

    if (mCorrectedLightness != 0)
        convertBGRToHSL((const unsigned char *)mCorrectedGradientImage.bits(), (unsigned char *)hsl, 256);
    bitshsl = hsl;

    if (!mColorize)
    {
        if (mCorrectedHue != 0 || mCorrectedSaturation != 0)
        {
            h = mCorrectedHue * 127 / 180;
            s = mCorrectedSaturation * 255 / 100;
            for (x = 0; x < 256; x++, bitshsl++)
            {
                bitshsl->h = (bitshsl->h + h + 256) % 256;
                if (bitshsl->s > 0)
                    bitshsl->s = s < 0 ?
                              lut01[bitshsl->s][s + 255] :
                              s == 255 ? 255 : IBP_minimum(lut02[bitshsl->s][255 - s], 255);
            }

            convertHSLToBGR((const unsigned char *)hsl, (unsigned char *)mCorrectedGradientImage.bits(), 256);
        }
    }
    else
    {
        h = mCorrectedHue * 255 / 360;
        s = mCorrectedSaturation * 255 / 100;
        for (x = 0; x < 256; x++, bitshsl++)
        {
            bitshsl->h = h;
            bitshsl->s = s;
        }
        convertHSLToBGR((const unsigned char *)hsl, (unsigned char *)mCorrectedGradientImage.bits(), 256);
    }
}

}}
