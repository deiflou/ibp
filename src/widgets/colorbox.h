#ifndef ANITOOLS_WIDGETS_COLORBOX_H
#define ANITOOLS_WIDGETS_COLORBOX_H

#include <QWidget>

#include "../imgproc/types.h"
#include "../imgproc/colorconversion.h"

namespace anitools {
namespace widgets {

using namespace anitools::imgproc;

class ColorBox : public QWidget
{
    Q_OBJECT
public:
    explicit ColorBox(QWidget *parent = 0);

    QColor color() const;
    void color(unsigned char *x, unsigned char *y, unsigned char *z, unsigned char *w = 0) const;
    void color(unsigned int *c) const;
    unsigned char value() const;
    ColorModel colorModel() const;
    ColorChannel colorChannel() const;

private:
    unsigned char mColor[4];
    ColorModel mColorModel;
    ColorChannel mColorChannel;

    BGRA mBGRAImageData[256 * 256];
    unsigned char mRealImageData[1024 * 256];

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
    void colorChanged();

public slots:
    void setColor(unsigned char x, unsigned char y, unsigned char z, unsigned char w = 0);
    void setColor(unsigned int c);
    void setColor(const QColor & c);
    void setValue(int v);
    void setColorModel(ColorModel cm);
    void setColorChannel(ColorChannel cc);
};

}}

#endif // ANITOOLS_WIDGETS_COLORBOX_H
