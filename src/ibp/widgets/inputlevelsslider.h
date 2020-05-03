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

#ifndef IBP_WIDGETS_INPUTLEVELSSLIDER_H
#define IBP_WIDGETS_INPUTLEVELSSLIDER_H

#include <QWidget>

namespace ibp {
namespace widgets {

class InputLevelsSlider : public QWidget
{
    Q_OBJECT

private:
    static constexpr double kLog1Over2 = -0.69314718055994530941723212145818;
    static constexpr double kPow1Over2ToThe10 = 0.0009765625;
    static const int kLeftMargin = 4;
    static const int kTopMargin = 1;
    static const int kRightMargin = 4;
    static const int kBottomMargin = 7;
    static const int kHandleWidth = 8;
    static const int kHandleHeight = 8;

    double mBlackPoint, mWhitePoint, mGammaCorrection;
    QImage mFunction;
    int mHandlePressed, mHandleSelected;

    void makeFunction();
    void paintHandle(QPainter & p, const QPoint & pos, const QColor & c, bool selected = false);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *);

public:
    explicit InputLevelsSlider(QWidget *parent = 0);

    double blackPoint();
    double whitePoint();
    double gammaCorrection();
    
signals:
    void blackPointChanged(double v);
    void whitePointChanged(double v);
    void gammaCorrectionChanged(double v);

public slots:
    void setBlackPoint(double v);
    void setWhitePoint(double v);
    void setGammaCorrection(double v);
    void setValues(double b, double w, double g);
};

}}
#endif // IBP_WIDGETS_INPUTLEVELSSLIDER_H
