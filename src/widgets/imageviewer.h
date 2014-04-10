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
