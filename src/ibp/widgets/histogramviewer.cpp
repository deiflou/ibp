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
#include <QStyle>
#include <QStyleOptionFrame>
#include <QWheelEvent>
#include <math.h>

#include "histogramviewer.h"
#include "../imgproc/types.h"
#include "../imgproc/lut.h"

namespace ibp {
namespace widgets {

using namespace ibp::imgproc;

HistogramViewer::HistogramViewer(QWidget *parent) :
    QWidget(parent),
    mHistMax(0),
    mHasHistogram(false),
    mIsMoving(false),
    mSelStartPos(-1),
    mSelEndPos(-1)
{
}

void HistogramViewer::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    if (mHasHistogram)
    {
        int linePos;
        double histV;
        QRect r = this->rect().adjusted(2, 2, -2, -2);
        p.fillRect(this->rect(), QColor(0, 0, 0));
        p.setPen(QColor(255, 255, 255));

        for (int i = 0; i < 256; i++)
        {
            linePos = i * (r.width() - 1) / 255;
            histV = (double)mHistogram[i] / mHistMax;
            p.drawLine(linePos + 2, r.bottom(), linePos + 2,
                       r.bottom() - histV * (r.height() - 1));
        }

        p.setCompositionMode(QPainter::CompositionMode_Difference);
        if (mSelStartPos != -1 && mSelStartPos != -1)
            p.fillRect(r.adjusted(qMin<int>(mSelStartPos, mSelEndPos) - r.left(),
                             0,
                             qMin<int>(mSelStartPos, mSelEndPos) - r.right(),
                             0),
                       QColor(255, 255, 255));
    }
    else
    {
        p.fillRect(this->rect(), this->palette().dark());

        p.setRenderHint(QPainter::Antialiasing);
        QPoint center(this->width() >> 1, this->height() >> 1);
        int radius = (qMin<int>(this->width(), this->height()) - 16) >> 1;
        int offset = (radius - radius / 6) * 0.7;

        p.setBrush(Qt::transparent);
        p.setPen(QPen(this->palette().mid(), radius / 3, Qt::SolidLine, Qt::FlatCap));

        p.drawEllipse(center, radius - radius / 6, radius - radius / 6);
        p.drawLine(center.x() - offset, center.y() - offset, center.x() + offset, center.y() + offset);
    }

    p.resetTransform();
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setRenderHint(QPainter::SmoothPixmapTransform, false);
    p.setBrush(Qt::transparent);
    p.setPen(QPen(this->palette().shadow(), 1));
    p.drawRect(this->rect().adjusted(0, 0, -1, -1));
    p.setPen(QPen(this->palette().mid(), 1));
    p.drawRect(this->rect().adjusted(1, 1, -2, -2));

    e->accept();
}

void HistogramViewer::mousePressEvent(QMouseEvent *e)
{
    if ((e->buttons() & Qt::LeftButton) && mHasHistogram)
    {
        mSelStartPos = mSelEndPos = qBound<int>(2, e->pos().x(), this->rect().right() - 2);
        mIsMoving = true;
        update();
        e->accept();
    }
}
void HistogramViewer::mouseReleaseEvent(QMouseEvent *e)
{
    if ((e->button() == Qt::LeftButton) && mHasHistogram)
    {
        mSelStartPos = mSelEndPos = -1;
        mIsMoving = false;
        update();
        e->accept();
    }
}
void HistogramViewer::mouseMoveEvent(QMouseEvent *e)
{
    if ((e->buttons() & Qt::LeftButton) && mIsMoving && mHasHistogram)
    {
        mSelEndPos = qBound<int>(2, e->pos().x(), this->rect().right() - 2);
        update();
        e->accept();
    }
}

void HistogramViewer::setImage(const QImage & newImage)
{
    if (newImage.isNull())
    {
        mHasHistogram = false;
        update();
        return;
    }

    int gray;
    unsigned int totalPixels = newImage.width() * newImage.height();
    BGRA * bits = (BGRA*)newImage.bits();

    memset(mHistogram, 0, sizeof(mHistogram));
    mHistMax = 0;

    while (totalPixels--)
    {
        gray = round(bits->r * .2126 + bits->g * .7152 + bits->b * .0722);
        gray += lut01[255 - gray][255 - bits->a];
        mHistogram[gray]++;
        if (mHistogram[gray] > mHistMax)
            mHistMax = mHistogram[gray];
        bits++;
    }

    mHasHistogram = true;
    update();
}

void HistogramViewer::setHistogram(const QVector<unsigned int> &newHistogram)
{
    if (newHistogram.isEmpty() || newHistogram.size() != 256)
    {
        mHasHistogram = false;
        update();
        return;
    }
    mHistMax = 0;
    for (int i = 0; i < 256; i++)
    {
        mHistogram[i] = newHistogram.at(i);
        if (mHistogram[i] > mHistMax)
            mHistMax = mHistogram[i];
    }
    mHasHistogram = true;
    update();
}

}}
