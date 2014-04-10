#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>
#include <opencv2/imgproc.hpp>

#include "../imgproc/imagefilter.h"

using namespace anitools::imgproc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    enum MorphologyOp
    {
        Dilation,
        Erosion,
        Closing,
        Opening
    };
    enum KernelShape
    {
        Ellipse,
        Rectangle,
        Diamond,
        Octagon,
        Plus,
        Cross,
        Ring
    };

    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    MorphologyOp mMorphologyOp;
    KernelShape mKernelShape;
    int mVRadius, mHRadius;
    bool mLockRadius;

    cv::Mat getDiamondStructuringElement(cv::Size size);
    cv::Mat getOctagonStructuringElement(cv::Size size);
    cv::Mat getCrossStructuringElement(cv::Size size);
    cv::Mat getRingStructuringElement(cv::Size size);

signals:
    void morphologyOpChanged(Filter::MorphologyOp mop);
    void kernelShapeChanged(Filter::KernelShape shape);
    void vRadiusChanged(int r);
    void hRadiusChanged(int r);
    void lockRadiusChanged(bool l);

public slots:
    void setMorphologyOp(Filter::MorphologyOp mop);
    void setKernelShape(Filter::KernelShape shape);
    void setVRadius(int r);
    void setHRadius(int r);
    void setLockRadius(bool l);
};

#endif // FILTER_H
