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

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "imagefilterpluginloader.h"

namespace ibp {
namespace plugins {

ImageFilterPluginLoader::ImageFilterPluginLoader()
{

}

ImageFilterPluginLoader::ImageFilterPluginLoader(const QString &path)
{
    load(path);
}

ImageFilterPluginLoader::~ImageFilterPluginLoader()
{
    clear();
}

bool ImageFilterPluginLoader::load(const QString &path)
{
    QDir pluginDir(path);
    if (!pluginDir.exists()) return false;
    QStringList fileList = pluginDir.entryList(QDir::Files);
    if (fileList.count() == 0) return false;

    QLibrary * library;
    typedef QHash<QString, QString> (* _getIBPPluginInfo)();
    _getIBPPluginInfo getIBPPluginInfo = 0;
    QHash<QString, QString> info;
    QString id;

    foreach (QString file, fileList)
    {
        library = 0;
        library = new QLibrary(pluginDir.path() + "/" + file);
        if (!library)
            continue;
        if (!library->load())
        {
            qInfo(library->errorString().toUtf8());
            delete library;
            continue;
        }

        if (!library->resolve("getIBPPluginInfo") |
            !library->resolve("getImageFilterInstance"))
        {
            library->unload();
            delete library;
            continue;
        }

        getIBPPluginInfo = (_getIBPPluginInfo)library->resolve("getIBPPluginInfo");
        info = getIBPPluginInfo();

        id = info.take("id");
        if (!id.startsWith("ibp.imagefilter.") || mLibraries.contains(id))
        {
            library->unload();
            delete library;
            continue;
        }
        mLibraries.insert(id, QPair<QLibrary *, QHash<QString, QString> >(library, info));
    }
    return true;
}

void ImageFilterPluginLoader::clear()
{
    QHashIterator<QString, QPair<QLibrary *, QHash<QString, QString> > > i(mLibraries);
    QLibrary * lib;
    while (i.hasNext())
    {
        i.next();
        lib = i.value().first;
        if (lib)
            delete lib;
    }
    mLibraries.clear();
}

ImageFilter *ImageFilterPluginLoader::instantiateFilter(const QString &id)
{
    if (!mLibraries.contains(id))
        return 0;

    QLibrary * lib = mLibraries.value(id).first;
    if (!lib)
        return 0;

    typedef ImageFilter * (* _getImageFilterInstance)();
    _getImageFilterInstance getImageFilterInstance = 0;
    getImageFilterInstance = (_getImageFilterInstance)lib->resolve("getImageFilterInstance");
    if (!getImageFilterInstance)
        return 0;
    return getImageFilterInstance();
}

QStringList ImageFilterPluginLoader::ids()
{
    return mLibraries.keys();
}

QHash<QString, QString> ImageFilterPluginLoader::info(const QString &id)
{
    return mLibraries.value(id).second;
}

int ImageFilterPluginLoader::count()
{
    return mLibraries.size();
}

}}
