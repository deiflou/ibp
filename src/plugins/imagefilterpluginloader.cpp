/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
**
** This file is part of Anitools
**
** Anitools is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "imagefilterpluginloader.h"

namespace anitools {
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
    typedef QHash<QString, QString> (* _getAnitoolsPluginInfo)();
    _getAnitoolsPluginInfo getAnitoolsPluginInfo = 0;
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
            delete library;
            continue;
        }

        if (!library->resolve("getAnitoolsPluginInfo") |
            !library->resolve("getImageFilterInstance"))
        {
            library->unload();
            delete library;
            continue;
        }

        getAnitoolsPluginInfo = (_getAnitoolsPluginInfo)library->resolve("getAnitoolsPluginInfo");
        info = getAnitoolsPluginInfo();

        id = info.take("id");
        if (!id.startsWith("anitools.imagefilter.") || mLibraries.contains(id))
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
