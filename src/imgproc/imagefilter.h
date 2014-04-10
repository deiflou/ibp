#ifndef ANITOOLS_IMGPROC_IMAGEFILTER_H
#define ANITOOLS_IMGPROC_IMAGEFILTER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QVariant>
#include <QHash>
#include <QSettings>
#include <QWidget>

namespace anitools {
namespace imgproc {

class ImageFilter : public QObject
{
    Q_OBJECT

public:
    virtual ~ImageFilter() {}
    virtual ImageFilter * clone() = 0;
    virtual QHash<QString, QString> info() = 0;
    virtual QImage process(const QImage & inputImage) = 0;
    virtual bool loadParameters(QSettings & s) = 0;
    virtual bool saveParameters(QSettings & s) = 0;
    virtual QWidget * widget(QWidget * parent = 0) = 0;
signals:
    void parametersChanged();
};

}}

#endif // ANITOOLS_IMGPROC_IMAGEFILTER_H
