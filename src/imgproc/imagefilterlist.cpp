#include <QSettings>
#include <QFile>
#include <QDebug>
#include <math.h>

#include "imagefilterlist.h"

namespace anitools {
namespace imgproc {

ImageFilterList::ImageFilterList(QObject *parent) :
    QThread(parent),
    mInputImage(),
    mFilters(),
    mAutoRun(false),
    mUseCache(false),
    mName(),
    mDescription(),
    mPluginLoader(0),
    mMustRestart(false)
{
}

ImageFilterList::ImageFilterList(const ImageFilterList &other) :
    QThread(other.parent()),
    mInputImage(other.mInputImage),
    mAutoRun(other.mAutoRun),
    mUseCache(other.mUseCache),
    mName(other.mName),
    mDescription(other.mDescription),
    mPluginLoader(other.mPluginLoader),
    mMustRestart(false)
{
    mFilters = copyFilterList(other.mFilters);
    mBypasses = other.mBypasses;
    mCache = other.mCache;
    for (int i = 0; i < mFilters.size(); i++)
        connect(other.mFilters.at(i), SIGNAL(parametersChanged()), this, SLOT(On_ImageFilter_parametersChanged()));
}

ImageFilterList &ImageFilterList::operator=(const ImageFilterList &other)
{
    mMutex.lock();
    mInputImage = other.mInputImage;
    mAutoRun = other.mAutoRun;
    mUseCache = other.mUseCache;
    mName = other.mName;
    mDescription = other.mDescription;
    mPluginLoader = other.mPluginLoader;
    clearFilterList(mFilters);
    mFilters = copyFilterList(other.mFilters);
    mBypasses = other.mBypasses;
    mCache = other.mCache;
    for (int i = 0; i < mFilters.size(); i++)
        connect(other.mFilters.at(i), SIGNAL(parametersChanged()), this, SLOT(On_ImageFilter_parametersChanged()));

    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return *this;
    }
    mMutex.unlock();
    return *this;
}

QImage ImageFilterList::inputImage() const
{
    return mInputImage;
}

bool ImageFilterList::autoRun() const
{
    return mAutoRun;
}

bool ImageFilterList::useCache() const
{
    return mUseCache;
}

bool ImageFilterList::bypass(int i) const
{
    return mBypasses.at(i);
}

const ImageFilter *ImageFilterList::at(int index) const
{
    return mFilters.at(index);
}

void ImageFilterList::append(ImageFilter *f)
{
    if (!f)
        return;
    mMutex.lock();
    mFilters.append(f);
    mBypasses.append(false);
    connect(f, SIGNAL(parametersChanged()), this, SLOT(On_ImageFilter_parametersChanged()));
    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return;
    }
    mMutex.unlock();
}

void ImageFilterList::insert(int index, ImageFilter *f)
{
    if (!f)
        return;
    mMutex.lock();
    mFilters.insert(index, f);
    mBypasses.insert(index, false);
    while (mCache.size() > index)
        mCache.removeLast();
    connect(f, SIGNAL(parametersChanged()), this, SLOT(On_ImageFilter_parametersChanged()));
    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return;
    }
    mMutex.unlock();
}

int ImageFilterList::count() const
{
    return mFilters.size();
}

bool ImageFilterList::isEmpty() const
{
    return mFilters.isEmpty();
}

QString ImageFilterList::description() const
{
    return mDescription;
}

ImageFilterPluginLoader *ImageFilterList::pluginLoader() const
{
    return mPluginLoader;
}

bool ImageFilterList::load(const QString &fileName)
{
    if (!mPluginLoader)
        return false;
    if (!QFile::exists(fileName))
        return false;

    QSettings s(fileName, QSettings::IniFormat, this);

    QString fileType, name, description, id;
    int nFilters = 0;
    ImageFilter * filter;

    s.beginGroup("info");
    fileType = s.value("fileType", QString()).toString();
    if (fileType != "anitools.imagefilterlist")
        return false;
    name = s.value("name", QString()).toString();
    description = s.value("description", QString()).toString();
    nFilters = s.value("nFilters", 0).toInt();
    s.endGroup();

    mMutex.lock();
    clearFilterList(mFilters);
    mBypasses.clear();
    for (int i = 0; i < nFilters; i++)
    {
        s.beginGroup("imageFilter" + QString::number(i + 1));
        id = s.value("id", QString()).toString();
        filter = mPluginLoader->instantiateFilter(id);
        if (!filter)
        {
            s.endGroup();
            continue;
        }
        filter->loadParameters(s);
        mFilters.append(filter);
        mBypasses.append(s.value("bypass", false).toBool());
        connect(filter, SIGNAL(parametersChanged()), this, SLOT(On_ImageFilter_parametersChanged()));
        s.endGroup();
    }
    mCache.clear();

    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return true;
    }
    mMutex.unlock();

    return true;
}

bool ImageFilterList::save(const QString &fileName)
{
    if (QFile::exists(fileName))
        if (!QFile::remove(fileName))
            return false;

    QSettings s(fileName, QSettings::IniFormat);
    ImageFilter * filter;

    mMutex.lock();

    s.beginGroup("info");
    s.setValue("fileType", "anitools.imagefilterlist");
    s.setValue("name", mName);
    s.setValue("description", mDescription);
    s.setValue("nFilters", mFilters.count());
    s.endGroup();

    for (int i = 0; i < mFilters.count(); i++)
    {
        filter = mFilters.at(i);
        s.beginGroup("imageFilter" + QString::number(i + 1));
        s.setValue("id", filter->info().value("id"));
        s.setValue("bypass", mBypasses.at(i));
        filter->saveParameters(s);
        s.endGroup();
    }

    mMutex.unlock();

    return true;
}

QString ImageFilterList::name() const
{
    return mName;
}

void ImageFilterList::move(int from, int to)
{
    mMutex.lock();
    mFilters.move(from, to);
    mBypasses.move(from, to);
    while (mCache.size() > qMin<int>(from, to))
        mCache.removeLast();
    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return;
    }
    mMutex.unlock();
}

void ImageFilterList::removeAt(int i)
{
    mMutex.lock();
    ImageFilter * f = mFilters.takeAt(i);
    if (f)
        delete f;
    mBypasses.removeAt(i);
    while (mCache.size() > i)
        mCache.removeLast();
    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return;
    }
    mMutex.unlock();
}

void ImageFilterList::clear()
{
    mMutex.lock();
    clearFilterList(mFilters);
    mBypasses.clear();
    mCache.clear();
    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return;
    }
    mMutex.unlock();
}

void ImageFilterList::setName(const QString &n)
{
    mName = n;
}

void ImageFilterList::setDescription(const QString &d)
{
    mDescription = d;
}

void ImageFilterList::setPluginLoader(ImageFilterPluginLoader *pl)
{
    mPluginLoader = pl;
}

void ImageFilterList::setInputImage(const QImage &i)
{
    mMutex.lock();
    mInputImage = i;
    mCache.clear();
    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return;
    }
    mMutex.unlock();
}

void ImageFilterList::setAutoRun(bool a)
{
    mMutex.lock();
    mAutoRun = a;
    if (mAutoRun && !isRunning())
    {
        mMutex.unlock();
        startProcessing();
        return;
    }
    mMutex.unlock();
}

void ImageFilterList::setUseCache(bool c)
{
    mMutex.lock();
    mUseCache = c;
    if (!mUseCache)
        mCache.clear();
    mMutex.unlock();
}

void ImageFilterList::setBypass(int i, bool b)
{
    mMutex.lock();
    if (i < 0 || i > mBypasses.size() - 1 || mBypasses[i] == b)
    {
        mMutex.unlock();
        return;
    }
    mBypasses[i] = b;
    while (mCache.size() > i)
        mCache.removeLast();
    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return;
    }
    mMutex.unlock();
}

void ImageFilterList::startProcessing(Priority p)
{
    if (!isRunning())
    {
        mMutex.lock();
        mMustRestart = false;
        mMutex.unlock();
        start(p);
    }
    else
    {
        mMutex.lock();
        mMustRestart = true;
        mMutex.unlock();
    }
}

void ImageFilterList::On_ImageFilter_parametersChanged()
{
    mMutex.lock();
    int index;
    ImageFilter * filter = qobject_cast<ImageFilter *>(sender());
    for (index = 0; index < mFilters.size(); index++)
        if (mFilters.at(index) == filter)
            break;
    while (mCache.size() > index)
        mCache.removeLast();
    if (mAutoRun)
    {
        mMutex.unlock();
        startProcessing();
        return;
    }
    mMutex.unlock();
}

void ImageFilterList::run()
{
    emit processingStarted();

    QList<ImageFilter *> filters;
    QList<bool> bypasses;
    QList<QImage> cache;

    forever
    {
        mMutex.lock();

        if (mInputImage.isNull())
        {
            mMutex.unlock();
            return;
        }

        QImage image;
        if (mFilters.size() == 0)
        {
            image = mInputImage.copy();
            mMutex.unlock();
            emit processingCompleted(image);
            return;
        }

        bool uc = mUseCache;
        int nFilter = mCache.size() - 1;
        clearFilterList(filters);
        filters = copyFilterList(mFilters);
        bypasses.clear();
        bypasses = mBypasses;
        cache = mCache;
        ImageFilter * filter = 0;
        bool bypass;
        const int partialProgress = 100 / filters.count();
        int progress = 0;
        mMustRestart = false;

        emit processingProgress(0);
        if (uc && nFilter > -1)
        {
            for (int i = 0; i <= nFilter; i++)
            {
                filters.removeFirst();
                bypasses.removeFirst();
                emit processingProgress(progress += partialProgress);
            }
            image = cache.at(nFilter).copy();
        }
        else
            image = mInputImage.copy();


        mMutex.unlock();

        while (!filters.isEmpty())
        {
            mMutex.lock();
            if (mMustRestart)
            {
                mMutex.unlock();
                break;
            }
            mMutex.unlock();

            filter = filters.takeFirst();
            bypass = bypasses.takeFirst();
            if (uc)
            {
                if (filter && !bypass)
                {
                    image = filter->process(image);
                    cache.append(image);
                }
                else
                {
                    cache.append(image);
                }

            }
            else
            {
                if (filter && !bypass)
                    image = filter->process(image);
            }

            mMutex.lock();
            if (mMustRestart)
            {
                mMutex.unlock();
                break;
            }
            mCache = cache;
            mMutex.unlock();

            emit processingProgress(progress += partialProgress);
        }

        mMutex.lock();
        if (mMustRestart)
        {
            mMutex.unlock();
            emit processingRestarted();
            continue;
        }
        mMutex.unlock();

        emit processingCompleted(image);
        return;
    }
}

void ImageFilterList::clearFilterList(QList<ImageFilter *> &list)
{
    ImageFilter * f;
    while (!list.isEmpty())
    {
        f = list.takeLast();
        if (f)
            delete f;
    }
}

QList<ImageFilter *> ImageFilterList::copyFilterList(const QList<ImageFilter *> &list) const
{
    QList<ImageFilter *> otherList;
    for (int i = 0; i < list.size(); i++)
        otherList.append(list.at(i)->clone());
    return otherList;
}

}}
