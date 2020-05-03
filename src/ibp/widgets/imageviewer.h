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

#ifndef IBP_WIDGETS_IMAGEVIEWER_H
#define IBP_WIDGETS_IMAGEVIEWER_H

#include <QWidget>
#include <QAbstractScrollArea>

namespace ibp {
namespace widgets {

class ImageViewer : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = 0);

    int zoomIndex();
    QImage image();
    
protected:
    bool viewportEvent(QEvent *e);

signals:
    void zoomIndexChanged(int zoomIndex);
    void viewportResized(const QRect & r);

public slots:
    void setZoomIndex(int newZoomIndex);
    void setImage(const QImage & newImage);

private:
    int mZoomIndex;
    float mZoom;
    QImage mImage;

    QImage mBackgroundImage;

    bool mIsMoving;
    QPoint mLastPos;

    void updateScrollBars();
};

}}

#endif // IBP_WIDGETS_IMAGEVIEWER_H
