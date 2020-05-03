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
#include <QMouseEvent>
#include <QGraphicsEffect>
#include <math.h>

#include "outputlevelsslider.h"

namespace ibp {
namespace widgets {

OutputLevelsSlider::OutputLevelsSlider(QWidget *parent) :
    QWidget(parent),
    mBlackPoint(0.0),
    mWhitePoint(1.0),
    mFunction(256, 1, QImage::Format_Indexed8),
    mHandlePressed(0),
    mHandleSelected(1)
{
//    QGraphicsDropShadowEffect * shadow = new QGraphicsDropShadowEffect();
//    shadow->setOffset(0, 2);
//    shadow->setBlurRadius(12);
//    shadow->setColor(QColor(0, 0, 0, 128));
//    this->setGraphicsEffect(shadow);

    setFocusPolicy(Qt::StrongFocus);

    QVector<QRgb> t;
    for (int i = 0; i < 256; i++)
        t.append(qRgb(i, i, i));
    mFunction.setColorTable(t);
    makeFunction();
}

void OutputLevelsSlider::focusInEvent(QFocusEvent * e)
{
    update();
    QWidget::focusInEvent(e);
}
void OutputLevelsSlider::focusOutEvent(QFocusEvent *e)
{
    update();
    QWidget::focusOutEvent(e);
}

void OutputLevelsSlider::keyPressEvent(QKeyEvent * e)
{
    switch (e->key())
    {
    case Qt::Key_Up:
        mHandleSelected++;
        if (mHandleSelected > 2) mHandleSelected = 2;
        update();
        break;
    case Qt::Key_Down:
        mHandleSelected--;
        if (mHandleSelected < 1) mHandleSelected = 1;
        update();
        break;
    case Qt::Key_Right:
        if (mHandleSelected == 1)
            setBlackPoint(mBlackPoint + 0.01);
        else
            setWhitePoint(mWhitePoint + 0.01);
        break;
    case Qt::Key_Left:
        if (mHandleSelected == 1)
            setBlackPoint(mBlackPoint - 0.01);
        else
            setWhitePoint(mWhitePoint - 0.01);
        break;
    default:
        QWidget::keyPressEvent(e);
        return;
    }
}

void OutputLevelsSlider::paintHandle(QPainter & p, const QPoint & pos, const QColor & c, bool selected)
{
    p.translate(pos);
    QPainterPath path(QPointF(0.0, 0.0));
    path.lineTo(4, 8);
    path.lineTo(-4, 8);
    path.closeSubpath();
    path.translate(0.5, 0.5);
    // shadow
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 0, 0, 32));
    p.drawPath(path.translated(3., 1.));
    p.setBrush(QColor(0, 0, 0, 64));
    p.drawPath(path.translated(1., 1.));
    // handle
    p.setBrush(c);
    p.setPen(QColor(0, 0, 0, 128));
    p.drawPath(path);
    // selection
    if (selected && this->hasFocus())
    {
        QColor h = palette().highlight().color();
        h.setAlpha(192);
        p.setPen(Qt::NoPen);
        p.setBrush(h);
        p.drawPath(path);
    }
    p.translate(-pos);
}

void OutputLevelsSlider::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter p(this);
    QRect r = this->rect().adjusted(kLeftMargin, kTopMargin, -kRightMargin, -kBottomMargin);

    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setPen(Qt::NoPen);
    // paint back shadow
    p.setBrush(QColor(0, 0, 0, 16));
    p.drawRoundedRect(r.adjusted(-2, -1, 2, 3), 3, 3);
    p.drawRoundedRect(r.adjusted(-1, 0, 1, 2), 2, 2);
    p.drawRoundedRect(r.adjusted(0, 1, 0, 1), 1, 1);
    // clipping
    QPainterPath clippingPath;
    clippingPath.addRoundedRect(r, 1, 1);
    p.setClipPath(clippingPath);
    // paint identity function
    QLinearGradient grd(r.topLeft(), r.topRight());
    grd.setColorAt(0.0, QColor(0, 0, 0));
    grd.setColorAt(1.0, QColor(255, 255, 255));
    p.setBrush(grd);
    p.drawRect(r.adjusted(0, 0, 0, -1));
    // paint function
    p.drawImage(r.adjusted(0, r.center().y(), 0, 0), mFunction);
    //handles
    p.setClipping(false);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setRenderHint(QPainter::SmoothPixmapTransform, false);

    double y, xB, xW;
    y = r.bottom() - 2;
    xB = (r.width() - 1) * mBlackPoint + kLeftMargin;
    xW = (r.width() - 1) * mWhitePoint + kLeftMargin;

    p.setRenderHint(QPainter::Antialiasing);
    paintHandle(p, QPoint(xB, y), QColor(64, 64, 64), mHandleSelected == 1);
    paintHandle(p, QPoint(xW, y), QColor(255, 255, 255), mHandleSelected == 2);
}

void OutputLevelsSlider::mousePressEvent(QMouseEvent *e)
{
    if (mHandlePressed != 0 || !(e->buttons() & Qt::LeftButton))
        return;

    QRect r = this->rect().adjusted(kLeftMargin, kTopMargin, -kRightMargin, -kBottomMargin);
    double xB, xW, xE;
    xB = (r.width() - 1) * mBlackPoint + kLeftMargin;
    xW = (r.width() - 1) * mWhitePoint + kLeftMargin;
    xE = e->x();

    double dB = fabs(xE - xB);
    double dW = fabs(xE - xW);

    int handle = dB < dW ? 1 : 2;
    if (handle == 1)
    {
        setBlackPoint((xE - kLeftMargin) / (r.width() - 1));
    }
    else
    {
        setWhitePoint((xE - kLeftMargin) / (r.width() - 1));
    }

    mHandlePressed = handle;
    if (mHandlePressed != mHandleSelected)
    {
        mHandleSelected = mHandlePressed;
        update();
    }
}
void OutputLevelsSlider::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
        return;

    mHandlePressed = 0;
}
void OutputLevelsSlider::mouseMoveEvent(QMouseEvent *e)
{
    if (mHandlePressed == 0 || !(e->buttons() & Qt::LeftButton))
        return;

    QRect r = this->rect().adjusted(kLeftMargin, kTopMargin, -kRightMargin, -kBottomMargin);
    double xE;
    xE = e->x();

    if (mHandlePressed == 1)
    {
        setBlackPoint((xE - kLeftMargin) / (r.width() - 1));
    }
    else
    {
        setWhitePoint((xE - kLeftMargin) / (r.width() - 1));
    }
}

double OutputLevelsSlider::blackPoint()
{
    return mBlackPoint;
}
double OutputLevelsSlider::whitePoint()
{
    return mWhitePoint;
}

void OutputLevelsSlider::setBlackPoint(double v)
{
    if (v == mBlackPoint) return;
    if (v < 0.0) v = 0.0;
    if (v > 1.0) v = 1.0;
    mBlackPoint = v;
    makeFunction();
    update();
    emit blackPointChanged(v);
}
void OutputLevelsSlider::setWhitePoint(double v)
{
    if (v == mWhitePoint) return;
    if (v < 0.0) v = 0.0;
    if (v > 1.0) v = 1.0;
    mWhitePoint = v;
    makeFunction();
    update();
    emit whitePointChanged(v);
}

void OutputLevelsSlider::setValues(double b, double w)
{
    if (b == mBlackPoint && w == mWhitePoint) return;
    if (b < 0.0) b = 0.0;
    if (b > 1.0) b = 1.0;
    if (w < 0.0) w = 0.0;
    if (w > 1.0) w = 1.0;
    mBlackPoint = b;
    mWhitePoint = w;
    makeFunction();
    update();
    emit blackPointChanged(b);
    emit whitePointChanged(w);
}

void OutputLevelsSlider::makeFunction()
{
    double m = mWhitePoint - mBlackPoint;
    double a = mBlackPoint;
    double value;
    for (int i = 0; i < 256; i++)
    {
        value = m * (i / 255.0) + a;
        value = value < 0.0 ? 0.0 : value > 1.0 ? 1.0 : value;
        mFunction.setPixel(i, 0, (unsigned char)round(value * 255.0));
    }
}

}}
