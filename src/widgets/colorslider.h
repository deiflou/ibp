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
    ColorChannel Colorchannel() const;
    Qt::Orientation orientation() const;

private:
    unsigned char mColor[4];
    ColorModel mColorModel;
    ColorChannel mColorchannel;
    Qt::Orientation mOrientation;

    BGRA mBGRAImageData[256];
    unsigned char mRealImageData[1024];

    void updateImageData();
    void updateImageDataAndPaint();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

signals:
    void valueChanged(int v);

public slots:
    void setColor(unsigned char x, unsigned char y, unsigned char z, unsigned char w = 0);
    void setColor(unsigned int c);
    void setColor(const QColor & c);
    void setValue(unsigned char v);
    void setColorModel(ColorModel cm);
    void setColorChannel(ColorChannel cc);
    void setOrientation(Qt::Orientation o);
};

}}

#endif // ANITOOLS_WIDGETS_COLORSLIDER_H
