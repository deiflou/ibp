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
