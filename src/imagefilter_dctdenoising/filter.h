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

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>

#include "../imgproc/imagefilter.h"

using namespace anitools::imgproc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    double mStrength;

signals:
    void strengthChanged(double s);

public slots:
    void setStrength(double s);
};

#endif // FILTER_H
