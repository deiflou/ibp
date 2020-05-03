//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef IBP_IMGPROC_IMAGEFILTERLIST_H
#define IBP_IMGPROC_IMAGEFILTERLIST_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QColor>

#include "imagefilter.h"
#include "../plugins/imagefilterpluginloader.h"

namespace ibp {
namespace imgproc {

using namespace ibp::plugins;

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

#endif // IBP_IMGPROC_IMAGEFILTERLIST_H
