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

#include "rotationgauge.h"
#include "../misc/util.h"

namespace anitools {
namespace widgets {

RotationGauge::RotationGauge(QWidget* parent) :
    QWidget(parent),
    mAngle(0.0),
    mAltitude(0.0),
    mAltitudeEnabled(false),
    mMouseButtonPressed(false)
{
    /*
    QGraphicsDropShadowEffect * shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(0, 2);
    shadow->setBlurRadius(8);
    shadow->setColor(QColor(0, 0, 0, 128));
    this->setGraphicsEffect(shadow);
*/
    setMinimumSize(32, 32);
}

void RotationGauge::focusInEvent(QFocusEvent *e)
{
    update();
    QWidget::focusInEvent(e);
}
void RotationGauge::focusOutEvent(QFocusEvent *e)
{
    update();
    QWidget::focusOutEvent(e);
}

double RotationGauge::angle()
{
    return mAngle;
}

double RotationGauge::altitude()
{
    return mAltitude;
}

bool RotationGauge::isAltitudeEnabled()
{
    return mAltitudeEnabled;
}

void RotationGauge::setAngle(double v)
{
    double t = mAngle;
    mAngle = fmod(v, 360.0);
    if (mAngle < 0.0) mAngle += 360.0;
    update();
    if (t != mAngle)
        emit angleChanged(mAngle);
}

void RotationGauge::setAltitude(double v)
{
    double t = mAltitude;
    mAltitude = qBound(0.0, v, 90.0);
    update();
    if (t != mAltitude)
        emit altitudeChanged(mAltitude);
}

void RotationGauge::enableAltitude(bool v)
{
    mAltitudeEnabled = v;
    update();
}

void RotationGauge::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);
    QPointF center(this->width() / 2.0, this->height() / 2.0);
    double radius = qMin(center.x(), center.y()) - 2.0;
    double alt = mAltitudeEnabled ? cos(AT_DegToRad(mAltitude)) : 1.0;

    painter.setRenderHints(QPainter::Antialiasing, true);

    QColor light = palette().color(QPalette::Light);
    QColor dark = palette().color(QPalette::Dark);
    QColor shadow = palette().color(QPalette::Shadow);
    QColor base = palette().color(QPalette::Base);
    base.setAlpha(0);
    QLinearGradient g1(center.x(), center.y() - radius, center.x(), center.y() + radius);
    g1.setColorAt(0.0, light);
    g1.setColorAt(1.0, dark);
    QRadialGradient g2(center, radius, QPointF(center.x() - radius / 2.0, center.y() - radius / 2.0));
    g2.setColorAt(0.0, light);
    g2.setColorAt(1.0, base);

    painter.setPen(Qt::transparent);
    painter.setBrush(g1);
    painter.drawEllipse(center, radius, radius);
    painter.setBrush(g2);
    painter.drawEllipse(center, radius, radius);

    if (this->hasFocus())
        painter.setPen(QPen(palette().color(QPalette::Highlight), 2.0));
    else
        painter.setPen(QPen(dark, 1.0));
    painter.setBrush(Qt::transparent);
    painter.drawEllipse(center, radius, radius);
    painter.setPen(QPen(dark, 1.0));
    painter.drawLine(center, QPointF(center.x() + cos(AT_DegToRad(mAngle)) * radius,
                                     center.y() - sin(AT_DegToRad(mAngle)) * radius));

    g2.setRadius(radius / 10.0);
    g2.setFocalPoint(center);
    g2.setColorAt(0.0, shadow);
    g2.setColorAt(1.0, dark);
    painter.setPen(Qt::transparent);
    painter.setBrush(g2);
    painter.drawEllipse(center, radius / 10.0, radius / 10.0);

    painter.setBrush(Qt::black);
    painter.drawEllipse(QPointF(center.x() + cos(AT_DegToRad(mAngle)) * alt * radius,
                                center.y() - sin(AT_DegToRad(mAngle)) * alt * radius),
                        radius / 10.0, radius / 10.0);
}

void RotationGauge::mousePressEvent(QMouseEvent * e)
{
    mMouseButtonPressed = true;
    mouseMoveEvent(e);
}

void RotationGauge::mouseReleaseEvent(QMouseEvent * e)
{
    Q_UNUSED(e)

    mMouseButtonPressed = false;
}

void RotationGauge::mouseMoveEvent(QMouseEvent *e)
{
    if (!mMouseButtonPressed)
        return;

    QPointF center(this->width() / 2.0, this->height() / 2.0);
    double radius = qMin(center.x(), center.y()) - 2.0;
    QPointF delta(e->x() - center.x(), e->y() - center.y());
    double angle = atan2(-delta.y(), delta.x());
    if (angle < 0.0) angle += 2.0 * AT_PI;
    double distance = qBound(0.0, sqrt(delta.x() * delta.x() + delta.y() * delta.y()), radius);
    double altitude = acos(distance / radius);

    mAngle = AT_RadToDeg(angle);
    mAltitude = AT_RadToDeg(altitude);

    update();

    emit angleChanged(mAngle);
    if (mAltitudeEnabled)
        emit altitudeChanged(mAltitude);
}

void RotationGauge::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Right)
    {
        if (e->modifiers() & Qt::Key_Control)
        {
            if (mAltitudeEnabled)
                setAltitude(mAltitude + 1.0);
        }
        else
            setAngle(mAngle + 1.0);
    }
    else if (e->key() == Qt::Key_Down || e->key() == Qt::Key_Left)
    {
        if (e->modifiers() & Qt::Key_Control)
        {
            if (mAltitudeEnabled)
                setAltitude(mAltitude - 1.0);
        }
        else
            setAngle(mAngle - 1.0);
    }
    else
        QWidget::keyPressEvent(e);
}

}}
