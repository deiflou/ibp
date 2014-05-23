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

#ifndef ANITOOLS_WIDGETS_IMAGEVIEWER_H
#define ANITOOLS_WIDGETS_IMAGEVIEWER_H

#include <QWidget>
#include <QAbstractScrollArea>

namespace anitools {
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

#endif // ANITOOLS_WIDGETS_IMAGEVIEWER_H
