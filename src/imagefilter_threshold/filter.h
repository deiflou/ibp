#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>

#include "../imgproc/imagefilter.h"

using namespace anitools::imgproc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    int mRGBMode;
    bool mAffectedChannel[5];
    int mThreshold[5];
    unsigned char mLUT[5][256];
    unsigned char mIdLUT[256];

signals:
    void rgbModeChanged(int m);
    void affectedChannelChanged(int c, bool a);
    void thresholdChanged(int c, int t);

public slots:
    void setRGBMode(int m);
    void setAffectedChannel(int c, bool a);
    void setThreshold(int c, int t);
};

#endif // FILTER_H
