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
    enum SizeMode
    {
        Percent,
        Pixels,
        KeepAspectRatio
    };
    enum ResamplingMode
    {
        NearestNeighbor,
        Bilinear,
        Bicubic,
        Lanczos
    };

    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    int mWidth, mHeight;
    SizeMode mWidthMode, mHeightMode;
    ResamplingMode mResamplingMode;

signals:
    void widthChanged(int w);
    void heightChanged(int h);
    void widthModeChanged(Filter::SizeMode wm);
    void heightModeChanged(Filter::SizeMode hm);
    void resamplingModeChanged(Filter::ResamplingMode rm);

public slots:
    void setWidth(int w);
    void setHeight(int h);
    void setWidthMode(Filter::SizeMode wm);
    void setHeightMode(Filter::SizeMode hm);
    void setResamplingMode(Filter::ResamplingMode rm);
};

#endif // FILTER_H
