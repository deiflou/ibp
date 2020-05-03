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

#ifndef IBP_WIDGETS_ANCHORPOSITIONBOX_H
#define IBP_WIDGETS_ANCHORPOSITIONBOX_H

#include <QWidget>

#include "../misc/util.h"

namespace ibp {
namespace widgets {

class AnchorPositionBox : public QWidget
{
    Q_OBJECT

public:
    explicit AnchorPositionBox(QWidget *parent = 0);

    ibp::misc::Alignment anchorPosition() const;

private:
    ibp::misc::Alignment mAnchorPosition;

    void paintArrow(QPainter &p, int x0, int y0, int x1, int y1);

public slots:
    void setAnchorPosition(ibp::misc::Alignment ap);

protected:
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent * e);

signals:
    void anchorPositionChanged(ibp::misc::Alignment ap);
    
};

}}
#endif // IBP_WIDGETS_ANCHORPOSITIONBOX_H
