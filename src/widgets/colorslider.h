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

#ifndef ANITOOLS_WIDGETS_COLORSLIDER_H
#define ANITOOLS_WIDGETS_COLORSLIDER_H

#include <QWidget>

#include "../imgproc/types.h"
#include "../imgproc/colorconversion.h"

namespace anitools {
namespace widgets {

using namespace anitools::imgproc;

class ColorSlider : public QWidget
{
    Q_OBJECT
public:
    explicit ColorSlider(QWidget *parent = 0);

    QColor color() const;
    void color(unsigned char *x, unsigned char *y, unsigned char *z, unsigned char *w = 0) const;
    void color(unsigned int *c) const;
    unsigned char value() const;
    ColorModel colorModel() const;
    ColorChannel colorChannel() const;
    Qt::Orientation orientation() const;

private:
    unsigned char mColor[4];
    ColorModel mColorModel;
    ColorChannel mColorChannel;
    Qt::Orientation mOrientation;

    BGRA mBGRAImageData[256];
    unsigned char mRealImageData[1024];

    QImage mBackgroundImage;

    void updateImageData();
    void updateImageDataAndPaint();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

signals:
    void valueChanged(int v);

public slots:
    void setColor(unsigned char x, unsigned char y, unsigned char z, unsigned char w = 0);
    void setColor(unsigned int c);
    void setColor(const QColor & c);
    void setValue(int v);
    void setColorModel(ColorModel cm);
    void setColorChannel(ColorChannel cc);
    void setOrientation(Qt::Orientation o);
};

}}

#endif // ANITOOLS_WIDGETS_COLORSLIDER_H
