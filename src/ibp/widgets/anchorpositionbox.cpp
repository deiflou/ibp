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

#include <QPainter>
#include <QMouseEvent>

#include "anchorpositionbox.h"

namespace ibp {
namespace widgets {

AnchorPositionBox::AnchorPositionBox(QWidget* parent) :
    QWidget(parent),
    mAnchorPosition(ibp::misc::CenterCenter)
{
}

void AnchorPositionBox::focusInEvent(QFocusEvent *e)
{
    update();
    QWidget::focusInEvent(e);
}
void AnchorPositionBox::focusOutEvent(QFocusEvent *e)
{
    update();
    QWidget::focusOutEvent(e);
}

ibp::misc::Alignment AnchorPositionBox::anchorPosition() const
{
    return mAnchorPosition;
}

void AnchorPositionBox::setAnchorPosition(ibp::misc::Alignment ap)
{
    if (ap == mAnchorPosition)
        return;
    mAnchorPosition = ap;
    update();
    emit anchorPositionChanged(ap);
}

void AnchorPositionBox::paintArrow(QPainter & p, int x0, int y0, int x1, int y1)
{
    const double preferredArrowLength = 6.;
    const double preferredArrowWidth = 4.;
    const double preferredNeckWidth = 1.;
    double dist, angle, arrowLength = preferredArrowLength, midArrowWidth, midNeckWidth;
    dist = ibp::misc::euclideanDistance(x0, y0, x1, y1);
    angle = ibp::misc::angleBetweenPoints(x0, y0, x1, y1);
    if (dist < arrowLength * 2.)
        arrowLength = dist / 2.;
    midArrowWidth = preferredArrowWidth / 2.;
    midNeckWidth = preferredNeckWidth / 2.;

    QPainterPath path;
    if (arrowLength < preferredArrowLength)
    {
        path.lineTo(arrowLength, midArrowWidth);
        path.lineTo(dist, 0);
        path.lineTo(arrowLength, -midArrowWidth);
        path.closeSubpath();
    }
    else
    {
        path.lineTo(arrowLength, midArrowWidth);
        path.lineTo(arrowLength, midNeckWidth);
        path.lineTo(dist - arrowLength, midNeckWidth);
        path.lineTo(dist - arrowLength, midArrowWidth);
        path.lineTo(dist, 0);
        path.lineTo(dist - arrowLength, -midArrowWidth);
        path.lineTo(dist - arrowLength, -midNeckWidth);
        path.lineTo(arrowLength, -midNeckWidth);
        path.lineTo(arrowLength, -midArrowWidth);
        path.closeSubpath();
    }
/*
    p.save();
    p.translate(x0 + 1, y0 + 1);
    p.rotate(IBP_RadToDeg(angle));
    p.fillPath(path, palette().color(QPalette::Shadow));
    p.restore();
*/
    p.save();
    p.translate(x0, y0);
    p.rotate(IBP_RadToDeg(angle));
    p.fillPath(path, palette().color(QPalette::Light));
    p.restore();
}

void AnchorPositionBox::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int row = (int)mAnchorPosition / 3, col = (int)mAnchorPosition - row * 3;
    QSizeF smallRectSize(this->width() * .333, this->height() * .333);
    QSizeF thisSize(this->size());

    p.setBrush(palette().color(QPalette::Dark));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(this->rect(), 2, 2);

    p.setBrush(palette().color(QPalette::Button));
    p.drawRoundedRect(col * smallRectSize.width(), row * smallRectSize.height(),
                      smallRectSize.width(), smallRectSize.height(), 2, 2);

    const double arrowMargin = 4.;
    if (mAnchorPosition == ibp::misc::TopLeft)
    {
        paintArrow(p, smallRectSize.width() + arrowMargin, smallRectSize.height() / 2,
                   thisSize.width() - arrowMargin - 1, smallRectSize.height() / 2);
        paintArrow(p, smallRectSize.width() / 2, smallRectSize.height() + arrowMargin,
                   smallRectSize.width() / 2, thisSize.height() - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() + arrowMargin, smallRectSize.height() + arrowMargin,
                   thisSize.width() - arrowMargin - 1, thisSize.height() - arrowMargin - 1);
    }
    else if (mAnchorPosition == ibp::misc::TopCenter)
    {
        paintArrow(p, arrowMargin, smallRectSize.height() / 2,
                   smallRectSize.width() - arrowMargin - 1, smallRectSize.height() / 2);
        paintArrow(p, smallRectSize.width() * 2 + arrowMargin, smallRectSize.height() / 2,
                   thisSize.width() - arrowMargin - 1, smallRectSize.height() / 2);
        paintArrow(p, smallRectSize.width() * 3 / 2, smallRectSize.height() + arrowMargin,
                   smallRectSize.width() * 3 / 2, thisSize.height() - arrowMargin - 1);
        paintArrow(p, arrowMargin, thisSize.height() - arrowMargin - 1,
                   smallRectSize.width() - arrowMargin - 1, smallRectSize.height() + arrowMargin);
        paintArrow(p, smallRectSize.width() * 2 + arrowMargin, smallRectSize.height() + arrowMargin,
                   thisSize.width() - arrowMargin - 1, thisSize.height() - arrowMargin - 1);
    }
    else if (mAnchorPosition == ibp::misc::TopRight)
    {
        paintArrow(p, arrowMargin, smallRectSize.height() / 2,
                   smallRectSize.width() * 2 - arrowMargin - 1, smallRectSize.height() / 2);
        paintArrow(p, smallRectSize.width() * 5 / 2, smallRectSize.height() + arrowMargin,
                   smallRectSize.width() * 5 / 2, thisSize.height() - arrowMargin - 1);
        paintArrow(p, arrowMargin, thisSize.height() - arrowMargin - 1,
                   smallRectSize.width() * 2 - arrowMargin - 1, smallRectSize.height() + arrowMargin);
    }
    else if (mAnchorPosition == ibp::misc::CenterLeft)
    {
        paintArrow(p, smallRectSize.width() / 2, arrowMargin,
                   smallRectSize.width() / 2, smallRectSize.height() - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() + arrowMargin, smallRectSize.height() - arrowMargin - 1,
                   thisSize.width() - arrowMargin - 1, arrowMargin);
        paintArrow(p, smallRectSize.width() + arrowMargin, smallRectSize.height() * 3 / 2,
                   thisSize.width() - arrowMargin - 1, smallRectSize.height() * 3 / 2);
        paintArrow(p, smallRectSize.width() + arrowMargin, smallRectSize.height() * 2 + arrowMargin,
                   thisSize.width() - arrowMargin - 1, thisSize.height() - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() / 2 , smallRectSize.height() * 2 + arrowMargin,
                   smallRectSize.width() / 2, thisSize.height() - arrowMargin - 1);
    }
    else if (mAnchorPosition == ibp::misc::CenterCenter)
    {
        paintArrow(p, arrowMargin, arrowMargin,
                   smallRectSize.width() - arrowMargin - 1, smallRectSize.height() - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() * 3 / 2, arrowMargin,
                   smallRectSize.width() * 3 / 2, smallRectSize.height() - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() * 2 + arrowMargin, smallRectSize.height() - arrowMargin - 1,
                   thisSize.width() - arrowMargin - 1, arrowMargin);
        paintArrow(p, arrowMargin, smallRectSize.height() * 3 / 2,
                   smallRectSize.width() - arrowMargin - 1, smallRectSize.height() * 3 / 2);
        paintArrow(p, smallRectSize.width() * 2 + arrowMargin, smallRectSize.height() * 3 / 2,
                   thisSize.width() - arrowMargin - 1, smallRectSize.height() * 3 / 2);
        paintArrow(p, arrowMargin, thisSize.height() - arrowMargin - 1,
                   smallRectSize.width() - arrowMargin - 1, smallRectSize.height() * 2 + arrowMargin);
        paintArrow(p, smallRectSize.width() * 3 / 2, smallRectSize.height() * 2 + arrowMargin,
                   smallRectSize.width() * 3 / 2, thisSize.height() - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() * 2 + arrowMargin, smallRectSize.height() * 2 + arrowMargin,
                   thisSize.width() - arrowMargin - 1, thisSize.height() - arrowMargin - 1);
    }
    else if (mAnchorPosition == ibp::misc::CenterRight)
    {
        paintArrow(p, arrowMargin, arrowMargin,
                   smallRectSize.width() * 2 - arrowMargin - 1, smallRectSize.height() - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() * 5 / 2, arrowMargin,
                   smallRectSize.width() * 5 / 2, smallRectSize.height() - arrowMargin - 1);
        paintArrow(p, arrowMargin, smallRectSize.height() * 3 / 2,
                   smallRectSize.width() * 2 - arrowMargin - 1, smallRectSize.height() * 3 / 2);
        paintArrow(p, arrowMargin, thisSize.height() - arrowMargin - 1,
                   smallRectSize.width() * 2 - arrowMargin - 1, smallRectSize.height() * 2 + arrowMargin);
        paintArrow(p, smallRectSize.width() * 5 / 2, smallRectSize.height() * 2 + arrowMargin,
                   smallRectSize.width() * 5 / 2, thisSize.height() - arrowMargin - 1);
    }
    else if (mAnchorPosition == ibp::misc::BottomLeft)
    {
        paintArrow(p, smallRectSize.width() / 2, arrowMargin,
                   smallRectSize.width() / 2, smallRectSize.height() * 2 - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() + arrowMargin, smallRectSize.height() * 2 - arrowMargin - 1,
                   thisSize.width() - arrowMargin - 1, arrowMargin);
        paintArrow(p, smallRectSize.width() + arrowMargin, smallRectSize.height() * 5 / 2,
                   thisSize.width() - arrowMargin - 1, smallRectSize.height() * 5 / 2);
    }
    else if (mAnchorPosition == ibp::misc::BottomCenter)
    {
        paintArrow(p, arrowMargin, arrowMargin,
                   smallRectSize.width() - arrowMargin - 1, smallRectSize.height() * 2 - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() * 3 / 2, arrowMargin,
                   smallRectSize.width() * 3 / 2, smallRectSize.height() * 2 - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() * 2 + arrowMargin, smallRectSize.height() * 2 - arrowMargin - 1,
                   thisSize.width() - arrowMargin - 1, arrowMargin);
        paintArrow(p, arrowMargin, smallRectSize.height() * 5 / 2,
                   smallRectSize.width() - arrowMargin - 1, smallRectSize.height() * 5 / 2);
        paintArrow(p, smallRectSize.width() * 2 + arrowMargin, smallRectSize.height() * 5 / 2,
                   thisSize.width() - arrowMargin - 1, smallRectSize.height() * 5 / 2);
    }
    else
    {
        paintArrow(p, arrowMargin, arrowMargin,
                   smallRectSize.width() * 2 - arrowMargin - 1, smallRectSize.height() * 2 - arrowMargin - 1);
        paintArrow(p, smallRectSize.width() * 5 / 2, arrowMargin,
                   smallRectSize.width() * 5 / 2, smallRectSize.height() * 2 - arrowMargin - 1);
        paintArrow(p, arrowMargin, smallRectSize.height() * 5 / 2,
                   smallRectSize.width() * 2 - arrowMargin - 1, smallRectSize.height() * 5 / 2);
    }

    p.setBrush(Qt::NoBrush);
    if (this->hasFocus())
    {
        p.setPen(QPen(palette().color(QPalette::Highlight), 2));
        p.drawRoundedRect(QRectF(this->rect()).adjusted(1, 1, -1, -1), 1, 1);
    }
    else
    {
        p.setPen(palette().color(QPalette::Shadow));
        p.drawRoundedRect(QRectF(this->rect()).adjusted(.5, .5, -.5, -.5), 2, 2);
    }
}

void AnchorPositionBox::mousePressEvent(QMouseEvent * e)
{
    if (e->button() != Qt::LeftButton)
        return;

    QSizeF smallRectSize(this->width() * .333, this->height() * .333);
    int row = int(e->y() / smallRectSize.height()) , col = int(e->x() / smallRectSize.width());
    int index = row * 3 + col;
    if (index < 0 || index > 8)
        return;
    mAnchorPosition = (ibp::misc::Alignment)index;
    update();
    emit anchorPositionChanged(mAnchorPosition);
}

void AnchorPositionBox::keyPressEvent(QKeyEvent * e)
{
    int row = (int)mAnchorPosition / 3, col = (int)mAnchorPosition - row * 3;
    if (e->key() == Qt::Key_Up)
        row--;
    else if (e->key() == Qt::Key_Down)
        row++;
    else if (e->key() == Qt::Key_Left)
        col--;
    else if (e->key() == Qt::Key_Right)
        col++;
    else
    {
        QWidget::keyPressEvent(e);
        return;
    }
    row = qBound(0, row, 2);
    col = qBound(0, col, 2);
    int index = row * 3 + col;
    if ((ibp::misc::Alignment)index == mAnchorPosition)
        return;
    mAnchorPosition = (ibp::misc::Alignment)index;
    update();
    emit anchorPositionChanged(mAnchorPosition);
}

}}
