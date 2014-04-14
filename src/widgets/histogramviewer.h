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

#ifndef ANITOOLS_WIDGETS_HISTOGRAMVIEWER_H
#define ANITOOLS_WIDGETS_HISTOGRAMVIEWER_H

#include <QWidget>

namespace anitools {
namespace widgets {

class HistogramViewer : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramViewer(QWidget *parent = 0);
    
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

public slots:
    void setImage(const QImage & newImage);
    void setHistogram(const QVector<unsigned int> &newHistogram);

private:
    unsigned int mHistogram[256], mHistMax;
    bool mHasHistogram;

    bool mIsMoving;
    int mSelStartPos, mSelEndPos;

};

}}

#endif // ANITOOLS_WIDGETS_HISTOGRAMVIEWER_H
