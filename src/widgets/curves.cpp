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
#include <QKeyEvent>
#include <math.h>

#include "curves.h"
#include "../misc/nearestneighborsplineinterpolator.h"
#include "../misc/linearsplineinterpolator.h"
#include "../misc/cubicsplineinterpolator.h"

namespace anitools {
namespace widgets {

Curves::Curves(QWidget *parent) :
    QWidget(parent),
    mSplineInterpolator(0),
    mInterpolationMode(Cubic),
    mKnotSelected(1),
    mKnotPressed(0),
    mBar1(256, 1, QImage::Format_ARGB32),
    mBar2(256, 1, QImage::Format_ARGB32),
    mFunctionValuesCalculated(false),
    mShowBars(true)
{
    setFocusPolicy(Qt::StrongFocus);
    for (int i = 0; i < 256; i++)
    {
        mBar1.setPixel(i, 0, qRgb(i, i, i));
        mBar2.setPixel(i, 0, qRgb(i, i, i));
    }
    mSplineInterpolator = new CubicSplineInterpolator();
}

SplineInterpolatorKnots Curves::knots() const
{
    return mSplineInterpolator->knots();
}

void Curves::setKnots(const SplineInterpolatorKnots &k)
{
    if (mSplineInterpolator->knots() == k)
        return;

    mSplineInterpolator->setKnots(k);
    mKnotSelected = 1;
    mFunctionValuesCalculated = false;
    repaint();
    emit knotsChanged(k);
    emit selectedKnotChanged(mKnotSelected - 1);
}

Curves::InterpolationMode Curves::interpolationMode() const
{
    return mInterpolationMode;
}

void Curves::setInterpolationMode(InterpolationMode m)
{
    if (mInterpolationMode == m)
        return;

    mInterpolationMode = m;

    SplineInterpolatorKnots k = mSplineInterpolator->knots();
    delete mSplineInterpolator;

    if (mInterpolationMode == NearestNeighbor)
        mSplineInterpolator = new NearestNeighborSplineInterpolator();
    else if (mInterpolationMode == Linear)
        mSplineInterpolator = new LinearSplineInterpolator();
    else
        mSplineInterpolator = new CubicSplineInterpolator();

    mSplineInterpolator->setKnots(k);

    mFunctionValuesCalculated = false;
    repaint();

    emit interpolationModeChanged(m);
}

void Curves::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter p(this);
    QRect r = this->rect();
    QSize plotSize = r.size();
    if (mShowBars) plotSize.setHeight(plotSize.height() - TOTAL_BAR_SIZE - SEPARATOR_SIZE);

    if (mSplineInterpolator && !mFunctionValuesCalculated) calculateFunctionValues();

    // overall background
    p.fillRect(QRect(QPoint(0, 0), plotSize), palette().color(QPalette::Button));

    // plot
    if (plotSize.isValid())
    {
        p.setClipRect(QRect(QPoint(0, 0), plotSize));
        // background
        p.fillRect(QRect(QPoint(0, 0), plotSize), palette().color(QPalette::Dark));
        // grid lines
        p.setPen(QPen(palette().color(QPalette::Mid), 1, Qt::DotLine));
        p.drawLine(plotSize.width() * 0.25, 0, plotSize.width() * 0.25, plotSize.height());
        p.drawLine(plotSize.width() * 0.50, 0, plotSize.width() * 0.50, plotSize.height());
        p.drawLine(plotSize.width() * 0.75, 0, plotSize.width() * 0.75, plotSize.height());
        p.drawLine(0, plotSize.height() * 0.25, plotSize.width(), plotSize.height() * 0.25);
        p.drawLine(0, plotSize.height() * 0.50, plotSize.width(), plotSize.height() * 0.50);
        p.drawLine(0, plotSize.height() * 0.75, plotSize.width(), plotSize.height() * 0.75);
        // base line
        p.setRenderHint(QPainter::Antialiasing);
        p.drawLine(0, plotSize.height(), plotSize.width(), 0);

        if (mSplineInterpolator)
        {
            // curve
            QPen pen(palette().color(QPalette::Light), 1.5);
            pen.setCosmetic(true);
            p.setPen(pen);
            p.setBrush(QColor(255, 255, 255, 16));
            p.scale(plotSize.width(), -(plotSize.height()));
            p.translate(0.0, -1.0);
            p.drawPolygon(mPolygon);
            p.resetTransform();

            // knots
            SplineInterpolatorKnot knot;
            QColor hlc = palette().highlight().color();
            hlc.setAlpha(this->hasFocus() ? 212 : 128);
            for (int i = 0; i < mSplineInterpolator->size(); i++)
            {
                knot = mSplineInterpolator->knot(i);
                knot.setX(knot.x() * (plotSize.width() - 1));
                knot.setY(plotSize.height() - 1 - knot.y() * (plotSize.height() - 1));
                p.setPen(palette().color(QPalette::Shadow));
                p.setBrush(i == mKnotSelected - 1 ? hlc : QColor(0, 0, 0, 64));
                p.drawEllipse(QPointF(knot.x(), knot.y()), KNOT_RADIUS, KNOT_RADIUS);
                p.setPen(palette().color(QPalette::Light));
                p.setBrush(Qt::transparent);
                p.drawEllipse(QPointF(knot.x(), knot.y()), KNOT_RADIUS - 1, KNOT_RADIUS - 1);
            }
        }

        p.setClipping(false);
    }

    p.setRenderHint(QPainter::Antialiasing, false);
    p.setBrush(Qt::transparent);

    // bars
    if (mShowBars)
    {
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        p.drawImage(QRect(0, this->height() - TOTAL_BAR_SIZE, this->width(), BAR_SIZE), mBar1);
        p.drawImage(QRect(0, this->height() - BAR_SIZE, this->width(), BAR_SIZE), mBar2);
        p.setRenderHint(QPainter::SmoothPixmapTransform, false);
    }

    // border
    p.setPen(palette().color(QPalette::Shadow));
    p.drawRect(QRect(QPoint(0, 0), plotSize).adjusted(0, 0, -1, -1));
    p.setPen(palette().color(QPalette::Light));
    p.drawRect(QRect(QPoint(0, 0), plotSize).adjusted(1, 1, -2, -2));
    if (mShowBars)
    {
        p.setPen(palette().color(QPalette::Shadow));
        p.drawRect(0, this->height() - TOTAL_BAR_SIZE, this->width() - 1, TOTAL_BAR_SIZE - 1);
        p.setPen(palette().color(QPalette::Light));
        p.drawRect(1, this->height() - TOTAL_BAR_SIZE + 1, this->width() - 3, TOTAL_BAR_SIZE - 3);
    }
}

void Curves::mousePressEvent(QMouseEvent *e)
{
    if (!mSplineInterpolator) return;
    if (mKnotPressed != 0 || (!(e->buttons() & Qt::LeftButton) && !(e->buttons() & Qt::RightButton)))
        return;

    QSize plotSize = this->size();
    if (mShowBars)
    {
        plotSize.setHeight(plotSize.height() - TOTAL_BAR_SIZE - SEPARATOR_SIZE);
        if (e->y() >= plotSize.height()) return;
    }

    int knotDist;
    int knotPressed = 0;
    SplineInterpolatorKnot knot;
    for (int i = 0; i < mSplineInterpolator->size(); i++)
    {
        knot = mSplineInterpolator->knot(i);
        knot.setX(knot.x() * (plotSize.width() - 1));
        knot.setY((plotSize.height() -1) - knot.y() * (plotSize.height() -1));

        knotDist = sqrt((e->x() - knot.x()) * (e->x() - knot.x()) +
                        (e->y() - knot.y()) * (e->y() - knot.y()));
        if (knotDist <= KNOT_MOUSE_DISTANCE)
        {
            knotPressed = i + 1;
            break;
        }
    }

    if (e->buttons() & Qt::LeftButton)
    {
        mKnotPressed = knotPressed;
        if (mKnotPressed > 0 && mKnotPressed != mKnotSelected)
        {
            mKnotSelected = mKnotPressed;
            repaint();
            emit selectedKnotChanged(mKnotSelected - 1);
        }
        else if (mKnotPressed == 0 && mSplineInterpolator->size() < MAX_N_KNOTS)
        {
            double x, y;
            x = (double)e->x() / (plotSize.width() - 1);
            y = (double)((plotSize.height() - 1) - e->y()) / (plotSize.height() - 1);
            for (int i = 0; i < mSplineInterpolator->size() - 1; i++)
            {
                if (x > mSplineInterpolator->knot(i).x() + MINIMUM_DISTANCE_BETWEEN_KNOTS &&
                    x < mSplineInterpolator->knot(i + 1).x() - MINIMUM_DISTANCE_BETWEEN_KNOTS)
                {
                    mSplineInterpolator->addKnot(SplineInterpolatorKnot(x, y));
                    mKnotSelected = mKnotPressed = i + 2;
                    mFunctionValuesCalculated = false;
                    repaint();
                    emit knotsChanged(mSplineInterpolator->knots());
                    break;
                }
            }
        }
    }
    else
    {
        if (knotPressed > 1 && knotPressed < mSplineInterpolator->size())
        {
            mSplineInterpolator->removeKnot(knotPressed - 1);
            if (mKnotSelected >= knotPressed) mKnotSelected--;
            mFunctionValuesCalculated = false;
            repaint();
            emit selectedKnotChanged(mKnotSelected - 1);
            emit knotsChanged(mSplineInterpolator->knots());
        }
    }
}
void Curves::mouseReleaseEvent(QMouseEvent *e)
{
    if (!mSplineInterpolator) return;
    if (e->buttons() & Qt::LeftButton)
        return;

    mKnotPressed = 0;
}
void Curves::mouseMoveEvent(QMouseEvent *e)
{
    if (!mSplineInterpolator) return;
    if (mKnotPressed == 0 || !(e->buttons() & Qt::LeftButton))
        return;

    QSize plotSize = this->size();
    if (mShowBars) plotSize.setHeight(plotSize.height() - TOTAL_BAR_SIZE);

    double x, y, minX, maxX;

    x = (double)e->x() / (plotSize.width() - 1);
    y = (double)((plotSize.height() - 1) - e->y()) / (plotSize.height() - 1);

    minX = mKnotSelected == 1 ?
                0.0 : mSplineInterpolator->knot(mKnotSelected - 2).x() + MINIMUM_DISTANCE_BETWEEN_KNOTS;
    maxX = mKnotSelected == mSplineInterpolator->size() ?
                1.0 : mSplineInterpolator->knot(mKnotSelected).x() - MINIMUM_DISTANCE_BETWEEN_KNOTS;

    mSplineInterpolator->setKnot(mKnotSelected - 1, qBound(minX, x, maxX), qBound(0.0, y, 1.0));

    mFunctionValuesCalculated = false;

    repaint();

    emit knotsChanged(mSplineInterpolator->knots());
}
void Curves::focusInEvent(QFocusEvent *e)
{
    repaint();
    QWidget::focusInEvent(e);
}
void Curves::focusOutEvent(QFocusEvent *e)
{
    repaint();
    QWidget::focusOutEvent(e);
}

void Curves::keyPressEvent(QKeyEvent *e)
{
    if (!mSplineInterpolator) return;
    double clampValue;
    SplineInterpolatorKnot k = mSplineInterpolator->knot(mKnotSelected - 1);
    switch (e->key())
    {
    case Qt::Key_Right:
        if (e->modifiers() & Qt::ControlModifier)
        {
            mKnotSelected++;
            if (mKnotSelected > mSplineInterpolator->size()) mKnotSelected = mSplineInterpolator->size();
            repaint();
            emit selectedKnotChanged(mKnotSelected - 1);
        }
        else
        {
            clampValue = mKnotSelected == mSplineInterpolator->size() ?
                        1.0 :
                        mSplineInterpolator->knot(mKnotSelected).x() - MINIMUM_DISTANCE_BETWEEN_KNOTS;
            mSplineInterpolator->setKnot(mKnotSelected - 1, qMin(k.x() + KEYPRESS_INCREMENT, clampValue), k.y());
            mFunctionValuesCalculated = false;
            repaint();
            emit knotsChanged(mSplineInterpolator->knots());
        }
        break;
    case Qt::Key_Left:
        if (e->modifiers() & Qt::ControlModifier)
        {
            mKnotSelected--;
            if (mKnotSelected < 1) mKnotSelected = 1;
            repaint();
            emit selectedKnotChanged(mKnotSelected - 1);
        }
        else
        {
            clampValue = mKnotSelected == 1 ?
                        0.0 : mSplineInterpolator->knot(mKnotSelected - 2).x() + MINIMUM_DISTANCE_BETWEEN_KNOTS;
            mSplineInterpolator->setKnot(mKnotSelected - 1, qMax(k.x() - KEYPRESS_INCREMENT, clampValue), k.y());
            mFunctionValuesCalculated = false;
            repaint();
            emit knotsChanged(mSplineInterpolator->knots());
        }
        break;
    case Qt::Key_Up:
        mSplineInterpolator->setKnot(mKnotSelected - 1, k.x(), qMin(k.y() + KEYPRESS_INCREMENT, 1.0));
        mFunctionValuesCalculated = false;
        repaint();
        emit knotsChanged(mSplineInterpolator->knots());
        break;
    case Qt::Key_Down:
        mSplineInterpolator->setKnot(mKnotSelected - 1, k.x(), qMax(k.y() - KEYPRESS_INCREMENT, 0.0));
        mFunctionValuesCalculated = false;
        repaint();
        emit knotsChanged(mSplineInterpolator->knots());
        break;
    default:
        QWidget::keyPressEvent(e);
        return;
    }
}

void Curves::calculateFunctionValues()
{
    if (mSplineInterpolator->size() < 1) return;

    const double step = 1.0 / 255.0;
    double xPos, yPos;
    int i, gray;
    mPolygon.clear();
    for (i = 0; i < mSplineInterpolator->size() - 1; i++)
    {
        for (xPos = mSplineInterpolator->knot(i).x(); xPos < mSplineInterpolator->knot(i + 1).x(); xPos += step)
        {
            yPos = mSplineInterpolator->f(xPos);
            mPolygon.append(QPointF(xPos, yPos));
            gray = qBound(0.0, round(yPos * 255.0), 255.0);
            mBar2.setPixel(round(xPos * 255.0), 0, qRgb(gray, gray, gray));
        }
    }

    mPolygon.prepend(QPointF(-0.01, mSplineInterpolator->f(-0.01)));
    mPolygon.prepend(QPointF(-0.01, -0.01));
    mPolygon.append(mSplineInterpolator->knot(mSplineInterpolator->size() - 1));
    mPolygon.append(QPointF(1.01, mSplineInterpolator->f(1.01)));
    mPolygon.append(QPointF(1.01, -0.01));

    gray = qBound(0.0, round(mSplineInterpolator->knot(0).y() * 255.0), 255.0);
    for (i = 0; i < round(mSplineInterpolator->knot(0).x() * 255.0); i++)
        mBar2.setPixel(i, 0, qRgb(gray, gray, gray));
    gray = qBound(0.0, round(mSplineInterpolator->knot(mSplineInterpolator->size() - 1).y() * 255.0), 255.0);
    for (i = round(mSplineInterpolator->knot(mSplineInterpolator->size() - 1).x() * 255.0); i < 256; i++)
        mBar2.setPixel(i, 0, qRgb(gray, gray, gray));

    mFunctionValuesCalculated = true;
}


QSize Curves::optimalSizeForWidth(int w) const
{
    return QSize(w, mShowBars ? w + TOTAL_BAR_SIZE : w);
}

int Curves::selectedKnotIndex() const
{
    return mKnotSelected - 1;
}

SplineInterpolatorKnot Curves::selectedKnot() const
{
    return mSplineInterpolator->knot(selectedKnotIndex());
}

void Curves::setSelectedKnot(double x, double y)
{
    double minX, maxX;
    minX = mKnotSelected == 1 ?
             0.0 : mSplineInterpolator->knot(mKnotSelected - 2).x() + MINIMUM_DISTANCE_BETWEEN_KNOTS;
    maxX = mKnotSelected == mSplineInterpolator->size() ?
             1.0 : mSplineInterpolator->knot(mKnotSelected).x() - MINIMUM_DISTANCE_BETWEEN_KNOTS;

    mSplineInterpolator->setKnot(mKnotSelected - 1, qBound(minX, x, maxX), qBound(0.0, y, 1.0));

    mFunctionValuesCalculated = false;

    repaint();

    emit knotsChanged(mSplineInterpolator->knots());
}

void Curves::setSelectedKnot(const SplineInterpolatorKnot & k)
{
    double minX, maxX;
    minX = mKnotSelected == 1 ?
             0.0 : mSplineInterpolator->knot(mKnotSelected - 2).x() + MINIMUM_DISTANCE_BETWEEN_KNOTS;
    maxX = mKnotSelected == mSplineInterpolator->size() ?
             1.0 : mSplineInterpolator->knot(mKnotSelected).x() - MINIMUM_DISTANCE_BETWEEN_KNOTS;

    mSplineInterpolator->setKnot(mKnotSelected - 1, qBound(minX, k.x(), maxX), qBound(0.0, k.y(), 1.0));

    mFunctionValuesCalculated = false;

    repaint();

    emit knotsChanged(mSplineInterpolator->knots());
}

}}
