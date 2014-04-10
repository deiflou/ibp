#include <opencv2/imgproc.hpp>

#include "filter.h"
#include "filterwidget.h"

Filter::Filter() :
    mRadius(0)
{
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mRadius = mRadius;
    return f;
}

extern "C" QHash<QString, QString> getAnitoolsPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getAnitoolsPluginInfo();
}

QImage Filter::process(const QImage &inputImage)
{
    if (inputImage.isNull() || inputImage.format() != QImage::Format_ARGB32)
        return inputImage;

    if (mRadius == 0)
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);

    cv::Mat msrc(inputImage.height(), inputImage.width(), CV_8UC4, (void *)inputImage.bits());
    cv::Mat mdst(i.height(), i.width(), CV_8UC4, i.bits());

    int size = (mRadius << 1) + 1;
    cv::medianBlur(msrc, mdst, size);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    int radius;
    bool ok;
    radius = s.value("radius", 0).toInt(&ok);
    if (!ok)
        return false;
    setRadius(radius);
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("radius", mRadius);
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setRadius(mRadius);
    connect(this, SIGNAL(radiusChanged(int)), fw, SLOT(setRadius(int)));
    connect(fw, SIGNAL(radiusChanged(int)), this, SLOT(setRadius(int)));
    return fw;
}

void Filter::setRadius(int s)
{
    if (s == mRadius)
        return;
    mRadius = s;
    emit radiusChanged(s);
    emit parametersChanged();
}
