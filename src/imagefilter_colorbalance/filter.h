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
    int mShadowsRed, mShadowsGreen, mShadowsBlue,
        mMidtonesRed, mMidtonesGreen, mMidtonesBlue,
        mHighlightsRed, mHighlightsGreen, mHighlightsBlue;
    bool mPreserveLuminosity;

    unsigned char mLuts[3][256];

    void makeLUTs();

signals:
    void shadowsRedChanged(int v);
    void shadowsGreenChanged(int v);
    void shadowsBlueChanged(int v);
    void midtonesRedChanged(int v);
    void midtonesGreenChanged(int v);
    void midtonesBlueChanged(int v);
    void highlightsRedChanged(int v);
    void highlightsGreenChanged(int v);
    void highlightsBlueChanged(int v);
    void preserveLuminosityChanged(bool v);

public slots:
    void setShadowsRed(int v);
    void setShadowsGreen(int v);
    void setShadowsBlue(int v);
    void setMidtonesRed(int v);
    void setMidtonesGreen(int v);
    void setMidtonesBlue(int v);
    void setHighlightsRed(int v);
    void setHighlightsGreen(int v);
    void setHighlightsBlue(int v);
    void setPreserveLuminosity(bool v);
};

#endif // FILTER_H
