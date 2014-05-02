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

#ifndef ROTATIONGAUGE_H
#define ROTATIONGAUGE_H

#include <QWidget>

namespace anitools {
namespace widgets {

class RotationGauge : public QWidget
{
    Q_OBJECT

private:
    double mAngle;
    double mAltitude;
    bool mAltitudeEnabled;

    bool mMouseButtonPressed;

public:
    explicit RotationGauge(QWidget *parent = 0);
    
    double angle();
    double altitude();
    bool isAltitudeEnabled();

public slots:
    void setAngle(double v);
    void setAltitude(double v);
    void enableAltitude(bool v);

protected:
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent * e);

signals:
    void angleChanged(double v);
    void altitudeChanged(double v);
    
};

}}
#endif // ROTATIONGAUGE_H
