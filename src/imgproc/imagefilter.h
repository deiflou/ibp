/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
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

#ifndef ANITOOLS_IMGPROC_IMAGEFILTER_H
#define ANITOOLS_IMGPROC_IMAGEFILTER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QVariant>
#include <QHash>
#include <QSettings>
#include <QWidget>

namespace anitools {
namespace imgproc {

class ImageFilter : public QObject
{
    Q_OBJECT

public:
    virtual ~ImageFilter() {}
    virtual ImageFilter * clone() = 0;
    virtual QHash<QString, QString> info() = 0;
    virtual QImage process(const QImage & inputImage) = 0;
    virtual bool loadParameters(QSettings & s) = 0;
    virtual bool saveParameters(QSettings & s) = 0;
    virtual QWidget * widget(QWidget * parent = 0) = 0;
signals:
    void parametersChanged();
};

}}

#endif // ANITOOLS_IMGPROC_IMAGEFILTER_H
