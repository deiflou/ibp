#include <opencv2/imgproc.hpp>

#include "filter.h"
#include "filterwidget.h"

Filter::Filter() :
    mRadius(0.0)
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

    if (qFuzzyIsNull(mRadius))
        return inputImage;

    QImage i = QImage(inputImage.width(), inputImage.height(), QImage::Format_ARGB32);

    cv::Mat msrc(inputImage.height(), inputImage.width(), CV_8UC4, (void *)inputImage.bits());
    cv::Mat mdst(i.height(), i.width(), CV_8UC4, i.bits());

    double sigma = (mRadius + 1.0) / 3.0;
    cv::GaussianBlur(msrc, mdst, cv::Size(0, 0), sigma);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    double radius;
    bool ok;
    radius = s.value("radius", 0.0).toDouble(&ok);
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
    connect(this, SIGNAL(radiusChanged(double)), fw, SLOT(setRadius(double)));
    connect(fw, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
    return fw;
}

void Filter::setRadius(double s)
{
    if (s == mRadius)
        return;
    mRadius = s;
    emit radiusChanged(s);
    emit parametersChanged();
}
