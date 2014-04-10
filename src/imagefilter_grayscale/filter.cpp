#include "filter.h"
#include "../imgproc/types.h"

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
    if (inputImage.isNull() || inputImage.format() != QImage::Format_ARGB32)
        return inputImage;

    QImage i = inputImage.copy();

    register BGRA * bits = (BGRA*)i.bits();
    register int totalPixels = i.width() * i.height();
    register unsigned short gR = .2126 * 0x10000;
    register unsigned short gG = .7152 * 0x10000;
    register unsigned short gB = .0722 * 0x10000;

    while (totalPixels--)
    {
        bits->r = bits->g = bits->b = (bits->r * gR >> 16) + (bits->g * gG >> 16) + (bits->b * gB >> 16);
        bits++;
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    Q_UNUSED(parent)
    return 0;
}
