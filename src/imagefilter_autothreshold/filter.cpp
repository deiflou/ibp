#include <opencv2/imgproc.hpp>
#include <vector>

#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"

Filter::Filter() :
    mThresholdMode(0),
    mRGBMode(0)
{
    for (int i = 0; i < 5; i++)
        mAffectedChannel[i] = false;
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mThresholdMode = mThresholdMode;
    f->mRGBMode = mRGBMode;
    for (int i = 0; i < 5; i++)
        f->mAffectedChannel[i] = mAffectedChannel[i];
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

    QImage i = inputImage.copy();
    cv::Mat srcMat(i.height(), i.width(), CV_8UC4, i.bits(), i.bytesPerLine());
    std::vector<cv::Mat> channels(4);

    if (mRGBMode == 0)
    {
        if (!mAffectedChannel[0] && !mAffectedChannel[4])
            return i;

        if (mAffectedChannel[0])
        {
            register unsigned short gR = .2126 * 0x10000;
            register unsigned short gG = .7152 * 0x10000;
            register unsigned short gB = .0722 * 0x10000;
            register BGRA * bits = (BGRA *)i.bits();
            register int totalPixels = i.width() * i.height();
            while (totalPixels--)
            {
                bits->r = bits->g = bits->b = (bits->r * gR >> 16) + (bits->g * gG >> 16) + (bits->b * gB >> 16);
                bits++;
            }
        }

        cv::split(srcMat, channels);
        if (mThresholdMode == 0)
        {
            if (mAffectedChannel[0])
            {
                cv::threshold(channels.at(0), channels.at(0), 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
                cv::threshold(channels.at(1), channels.at(1), 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
                cv::threshold(channels.at(2), channels.at(2), 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            }
        }
        else
        {
            if (mAffectedChannel[0])
            {
                cv::adaptiveThreshold(channels.at(0), channels.at(0), 255,
                                      cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 5);
                cv::adaptiveThreshold(channels.at(1), channels.at(1), 255,
                                      cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 5);
                cv::adaptiveThreshold(channels.at(2), channels.at(2), 255,
                                      cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 5);
            }
        }
    }
    else
    {
        if (!mAffectedChannel[1] && !mAffectedChannel[2] &&
            !mAffectedChannel[3] && !mAffectedChannel[4])
            return i;
        cv::split(srcMat, channels);
        if (mThresholdMode == 0)
        {
            if (mAffectedChannel[1])
                cv::threshold(channels.at(0), channels.at(0), 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            if (mAffectedChannel[2])
                cv::threshold(channels.at(1), channels.at(1), 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            if (mAffectedChannel[3])
                cv::threshold(channels.at(2), channels.at(2), 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        }
        else
        {
            if (mAffectedChannel[1])
                cv::adaptiveThreshold(channels.at(0), channels.at(0), 255,
                                      cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 5);
            if (mAffectedChannel[2])
                cv::adaptiveThreshold(channels.at(1), channels.at(1), 255,
                                      cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 5);
            if (mAffectedChannel[3])
                cv::adaptiveThreshold(channels.at(2), channels.at(2), 255,
                                      cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 5);
        }
    }
    if (mThresholdMode == 0)
    {
        if (mAffectedChannel[4])
            cv::threshold(channels.at(3), channels.at(3), 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    }
    else
    {
        if (mAffectedChannel[4])
            cv::adaptiveThreshold(channels.at(3), channels.at(3), 255,
                                  cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 5);
    }

    cv::merge(channels, srcMat);

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString thresholdModeStr, rgbModeStr, affectedChannelStr;
    int thresholdMode, rgbMode;
    QStringList affectedChannelList;
    bool affectedChannel[5] =  { false };

    thresholdModeStr = s.value("thresholdmode", "global").toString();
    if (thresholdModeStr == "global")
        thresholdMode = 0;
    else if (thresholdModeStr == "local")
        thresholdMode = 1;
    else
        return false;

    rgbModeStr = s.value("rgbmode", "tight").toString();
    if (rgbModeStr == "tight")
        rgbMode = 0;
    else if (rgbModeStr == "independent")
        rgbMode = 1;
    else
        return false;

    affectedChannelStr = s.value("affectedchannels", "").toString();
    affectedChannelList = affectedChannelStr.split(" ", QString::SkipEmptyParts);
    for (int i = 0; i < affectedChannelList.size(); i++)
    {
        affectedChannelStr = affectedChannelList.at(i);
        if (affectedChannelList.at(i) == "rgb")
            affectedChannel[0] = true;
        else if (affectedChannelList.at(i) == "red")
            affectedChannel[1] = true;
        else if (affectedChannelList.at(i) == "green")
            affectedChannel[2] = true;
        else if (affectedChannelList.at(i) == "blue")
            affectedChannel[3] = true;
        else if (affectedChannelList.at(i) == "alpha")
            affectedChannel[4] = true;
        else
            return false;
    }

    setThresholdMode(thresholdMode);
    setRGBMode(rgbMode);
    for (int i = 0; i < 5; i++)
        setAffectedChannel(i, affectedChannel[i]);

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("thresholdmode", mThresholdMode == 0 ? "global" : "local");

    s.setValue("rgbmode", mRGBMode == 0 ? "tight" : "independent");

    QStringList affectedChannelList;
    if (mAffectedChannel[0])
        affectedChannelList.append("rgb");
    if (mAffectedChannel[1])
        affectedChannelList.append("red");
    if (mAffectedChannel[2])
        affectedChannelList.append("green");
    if (mAffectedChannel[3])
        affectedChannelList.append("blue");
    if (mAffectedChannel[4])
        affectedChannelList.append("alpha");
    s.setValue("affectedchannels", affectedChannelList.join(" "));

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setThresholdMode(mThresholdMode);
    fw->setRGBMode(mRGBMode);
    for (int i = 0; i < 5; i++)
        fw->setAffectedChannel(i, mAffectedChannel[i]);
    connect(this, SIGNAL(thresholdModeChanged(int)), fw, SLOT(setThresholdMode(int)));
    connect(this, SIGNAL(rgbModeChanged(int)), fw, SLOT(setRGBMode(int)));
    connect(this, SIGNAL(affectedChannelChanged(int,bool)), fw, SLOT(setAffectedChannel(int,bool)));
    connect(fw, SIGNAL(thresholdModeChanged(int)), this, SLOT(setThresholdMode(int)));
    connect(fw, SIGNAL(rgbModeChanged(int)), this, SLOT(setRGBMode(int)));
    connect(fw, SIGNAL(affectedChannelChanged(int,bool)), this, SLOT(setAffectedChannel(int,bool)));
    return fw;
}

void Filter::setThresholdMode(int m)
{
    if (m == mThresholdMode)
        return;
    mThresholdMode = m;
    emit thresholdModeChanged(m);
    emit parametersChanged();
}

void Filter::setRGBMode(int m)
{
    if (m == mRGBMode)
        return;
    mRGBMode = m;
    emit rgbModeChanged(m);
    emit parametersChanged();
}

void Filter::setAffectedChannel(int c, bool a)
{
    if (a == mAffectedChannel[c])
        return;
    mAffectedChannel[c] = a;
    emit affectedChannelChanged(c, a);
    emit parametersChanged();
}

