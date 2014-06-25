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
#include <QMouseEvent>
#include <QGraphicsEffect>
#include <math.h>

#include "inputlevelsslider.h"

namespace anitools {
namespace widgets {

InputLevelsSlider::InputLevelsSlider(QWidget *parent) :
    QWidget(parent),
    mBlackPoint(0.0),
    mWhitePoint(1.0),
    mGammaCorrection(1.0),
    mFunction(256, 1, QImage::Format_Indexed8),
    mHandlePressed(0),
    mHandleSelected(1)
{
    QGraphicsDropShadowEffect * shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(0, 2);
    shadow->setBlurRadius(8);
    shadow->setColor(QColor(0, 0, 0, 128));
    this->setGraphicsEffect(shadow);

    setFocusPolicy(Qt::StrongFocus);

    QVector<QRgb> t;
    for (int i = 0; i < 256; i++)
        t.append(qRgb(i, i, i));
    mFunction.setColorTable(t);
    makeFunction();
}

void InputLevelsSlider::focusInEvent(QFocusEvent * e)
{
    repaint();
    QWidget::focusInEvent(e);
}
void InputLevelsSlider::focusOutEvent(QFocusEvent *e)
{
    repaint();
    QWidget::focusOutEvent(e);
}

void InputLevelsSlider::keyPressEvent(QKeyEvent * e)
{
    switch (e->key())
    {
    case Qt::Key_Up:
        mHandleSelected++;
        if (mHandleSelected > 3) mHandleSelected = 3;
        repaint();
        break;
    case Qt::Key_Down:
        mHandleSelected--;
        if (mHandleSelected < 1) mHandleSelected = 1;
        repaint();
        break;
    case Qt::Key_Right:
        if (mHandleSelected == 1)
            setBlackPoint(mBlackPoint + 0.01);
        else if (mHandleSelected == 2)
            setGammaCorrection(mGammaCorrection + 0.01);
        else
            setWhitePoint(mWhitePoint + 0.01);
        break;
    case Qt::Key_Left:
        if (mHandleSelected == 1)
            setBlackPoint(mBlackPoint - 0.01);
        else if (mHandleSelected == 2)
            setGammaCorrection(mGammaCorrection - 0.01);
        else
            setWhitePoint(mWhitePoint - 0.01);
        break;
    default:
        QWidget::keyPressEvent(e);
        return;
    }
}

void InputLevelsSlider::paintHandle(QPainter & p, const QPoint & pos, const QColor & c, bool selected)
{
    p.translate(pos);
    QPainterPath path(QPointF(0.0, 0.0));
    path.lineTo(4, 8);
    path.lineTo(-4, 8);
    path.closeSubpath();
    path.translate(0.5, 0.5);
    p.setBrush(c);
    p.setPen(QColor(0, 0, 0, 128));
    p.drawPath(path);
    if (selected && this->hasFocus())
    {
        QColor h = palette().highlight().color();
        h.setAlpha(192);
        p.setPen(Qt::transparent);
        p.setBrush(h);
        p.drawPath(path);
    }
    p.translate(-pos);
}

void InputLevelsSlider::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter p(this);

    QRect r = this->rect().adjusted(LEFT_MARGIN, TOP_MARGIN, -RIGHT_MARGIN, -BOTTOM_MARGIN);

    QLinearGradient grd(r.topLeft(), r.topRight());
    grd.setColorAt(0.0, QColor(0, 0, 0));
    grd.setColorAt(1.0, QColor(255, 255, 255));

    p.fillRect(r.adjusted(0, 0, 0, -r.center().y()), grd);
    p.drawImage(r.adjusted(0, r.center().y(), 0, 0), mFunction);

    p.setBrush(Qt::transparent);
    p.setPen(QColor(0, 0, 0, 128));
    p.drawRect(r.adjusted(0, 0, -1, -1));
    p.setPen(QColor(255, 255, 255, 128));
    p.drawRect(r.adjusted(1, 1, -2, -2));

    double y, xB, xW, xG;
    y = this->rect().bottom() - HANDLE_HEIGHT;
    xB = (r.width() - 1) * mBlackPoint + LEFT_MARGIN;
    xW = (r.width() - 1) * mWhitePoint + LEFT_MARGIN;
    xG = (xW - xB - 2) * pow(0.5, mGammaCorrection) + xB + 1;

    p.setRenderHint(QPainter::Antialiasing);
    paintHandle(p, QPoint(xG, y), QColor(128, 128, 128), mHandleSelected == 2);
    paintHandle(p, QPoint(xB, y), QColor(64, 64, 64), mHandleSelected == 1);
    paintHandle(p, QPoint(xW, y), QColor(255, 255, 255), mHandleSelected == 3);
}

void InputLevelsSlider::mousePressEvent(QMouseEvent *e)
{
    if (mHandlePressed != 0 || !(e->buttons() & Qt::LeftButton))
        return;

    QRect r = this->rect().adjusted(LEFT_MARGIN, TOP_MARGIN, -RIGHT_MARGIN, -BOTTOM_MARGIN);
    double xB, xW, xG, xE;
    xB = (r.width() - 1) * mBlackPoint + LEFT_MARGIN;
    xW = (r.width() - 1) * mWhitePoint + LEFT_MARGIN;
    xG = (xW - xB - 2) * pow(0.5, mGammaCorrection) + xB + 1;
    xE = e->x();

    double dB = fabs(xE - xB);
    double dW = fabs(xE - xW);
    double dG = fabs(xE - xG);

    int handle = dB < dW ? (dB < dG ? 1 : 2) : (dW < dG ? 3 : 2);
    if (handle == 1)
    {
        setBlackPoint((xE - LEFT_MARGIN) / (r.width() - 1));
    }
    else if (handle == 2)
    {
        xG = (xE - xB - 1) / (xW - xB - 2);
        if (xG <= POW_1OVER2_TEN) xG = POW_1OVER2_TEN;
        setGammaCorrection(log(xG) / LOG_1OVER2);
    }
    else
    {
        setWhitePoint((xE - LEFT_MARGIN) / (r.width() - 1));
    }

    mHandlePressed = handle;
    if (mHandlePressed != mHandleSelected)
    {
        mHandleSelected = mHandlePressed;
        repaint();
    }
}
void InputLevelsSlider::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
        return;

    mHandlePressed = 0;
}
void InputLevelsSlider::mouseMoveEvent(QMouseEvent *e)
{
    if (mHandlePressed == 0 || !(e->buttons() & Qt::LeftButton))
        return;

    QRect r = this->rect().adjusted(LEFT_MARGIN, TOP_MARGIN, -RIGHT_MARGIN, -BOTTOM_MARGIN);
    double xB, xW, xG, xE;
    xB = (r.width() - 1) * mBlackPoint + LEFT_MARGIN;
    xW = (r.width() - 1) * mWhitePoint + LEFT_MARGIN;
    xE = e->x();

    if (mHandlePressed == 1)
    {
        setBlackPoint((xE - LEFT_MARGIN) / (r.width() - 1));
    }
    else if (mHandlePressed == 2)
    {
        xG = (xE - xB - 1) / (xW - xB - 2);
        if (xG <= POW_1OVER2_TEN) xG = POW_1OVER2_TEN;
        setGammaCorrection(log(xG) / LOG_1OVER2);
    }
    else
    {
        setWhitePoint((xE - LEFT_MARGIN) / (r.width() - 1));
    }
}

double InputLevelsSlider::blackPoint()
{
    return mBlackPoint;
}
double InputLevelsSlider::whitePoint()
{
    return mWhitePoint;
}
double InputLevelsSlider::gammaCorrection()
{
    return mGammaCorrection;
}

void InputLevelsSlider::setBlackPoint(double v)
{
    if (v == mBlackPoint) return;
    if (v > mWhitePoint - 0.01) v = mWhitePoint - 0.01;
    if (v < 0.0) v = 0.0;
    mBlackPoint = v;
    makeFunction();
    repaint();
    emit blackPointChanged(v);
}
void InputLevelsSlider::setWhitePoint(double v)
{
    if (v == mWhitePoint) return;
    if (v < mBlackPoint + 0.01) v = mBlackPoint + 0.01;
    if (v > 1.0) v = 1.0;
    mWhitePoint = v;
    makeFunction();
    repaint();
    emit whitePointChanged(v);
}
void InputLevelsSlider::setGammaCorrection(double v)
{
    if (v == mGammaCorrection) return;
    if (v < 0.1) v = 0.1;
    if (v > 10.0) v = 10.0;
    mGammaCorrection = v;
    makeFunction();
    repaint();
    emit gammaCorrectionChanged(v);
}

void InputLevelsSlider::setValues(double b, double w, double g)
{
    if (b == mBlackPoint && w == mWhitePoint && g == mGammaCorrection) return;
    if (b > w - 0.01) b = w - 0.01;
    if (b < 0.0) b = 0.0;
    if (w < b + 0.01) w = b + 0.01;
    if (w > 1.0) w = 1.0;
    if (g < 0.1) g = 0.1;
    if (g > 10.0) g = 10.0;
    mBlackPoint = b;
    mWhitePoint = w;
    mGammaCorrection = g;
    makeFunction();
    repaint();
    emit blackPointChanged(b);
    emit whitePointChanged(w);
    emit gammaCorrectionChanged(g);
}

void InputLevelsSlider::makeFunction()
{
    double m = 1.0 / (mWhitePoint - mBlackPoint);
    double a = m * mBlackPoint;
    double gamma = 1.0 / mGammaCorrection;
    double value;
    for (int i = 0; i < 256; i++)
    {
        value = m * (i / 255.0) - a;
        value = value < 0.0 ? 0.0 : value > 1.0 ? 1.0 : value;
        mFunction.setPixel(i, 0, (unsigned char)round(pow(value, gamma) * 255.0));
    }
}

}}
