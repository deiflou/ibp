#include "filter.h"

Filter::Filter()
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    return new Filter();
}

extern "C" QHash<QString, QString> getAnitoolsPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getAnitoolsPluginInfo();
}

QImage Filter::process(const QImage &inputImage)
{
    return inputImage;
}

bool Filter::loadParameters(QSettings &s)
{
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    return 0;
}
