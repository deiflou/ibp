#ifndef ANITOOLS_IMGPROC_IMAGEFILTERLIST_H
#define ANITOOLS_IMGPROC_IMAGEFILTERLIST_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QColor>

#include "imagefilter.h"
#include "../plugins/imagefilterpluginloader.h"

namespace anitools {
namespace imgproc {

using namespace anitools::plugins;

class ImageFilterList : public QThread
{
    Q_OBJECT
public:
    explicit ImageFilterList(QObject *parent = 0);
    ImageFilterList(const ImageFilterList & other);
    ImageFilterList & operator=(const ImageFilterList & other);

    QImage inputImage() const;
    bool autoRun() const;
    bool useCache() const;
    bool bypass(int i) const;
    const ImageFilter *at(int index) const;
    int count() const;
    bool isEmpty() const;
    QString name() const;
    QString description() const;
    ImageFilterPluginLoader * pluginLoader() const;
    bool load(const QString & fileName);
    bool save(const QString & fileName);

protected:
    void run();

private:
    QImage mInputImage;
    QList<ImageFilter *> mFilters;
    QList<bool> mBypasses;
    bool mAutoRun;
    bool mUseCache;
    QList<QImage> mCache;
    QString mName, mDescription;
    ImageFilterPluginLoader * mPluginLoader;

    bool mMustRestart;
    QMutex mMutex;

    void clearFilterList(QList<ImageFilter *> & list);
    QList<ImageFilter *> copyFilterList(const QList<ImageFilter *> & list) const;

signals:
    void processingProgress(int p);
    void processingStarted();
    void processingRestarted();
    void processingCompleted(const QImage & outputImage);

public slots:
    void setInputImage(const QImage & i);
    void setAutoRun(bool a);
    void setUseCache(bool c);
    void setBypass(int i, bool b);
    void append(ImageFilter * f);
    void insert(int index, ImageFilter * f);
    void move(int from, int to);
    void removeAt(int i);
    void clear();
    void setName(const QString & n);
    void setDescription(const QString & d);
    void setPluginLoader(ImageFilterPluginLoader * pl);

    void startProcessing(Priority p = InheritPriority);

private slots:
    void On_ImageFilter_parametersChanged();
};

}}

#endif // ANITOOLS_IMGPROC_IMAGEFILTERLIST_H
