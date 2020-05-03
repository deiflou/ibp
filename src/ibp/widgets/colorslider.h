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

#ifndef IBP_WIDGETS_COLORSLIDER_H
#define IBP_WIDGETS_COLORSLIDER_H

#include <QWidget>

#include "../imgproc/types.h"
#include "../imgproc/colorconversion.h"

namespace ibp {
namespace widgets {

using namespace ibp::imgproc;

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

#endif // IBP_WIDGETS_COLORSLIDER_H
