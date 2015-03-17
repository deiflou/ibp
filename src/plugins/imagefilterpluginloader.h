/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
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

#ifndef ANITOOLS_PLUGINS_IMAGEFILTERPLUGINLOADER_H
#define ANITOOLS_PLUGINS_IMAGEFILTERPLUGINLOADER_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QLibrary>

#include "../imgproc/imagefilter.h"

namespace anitools {
namespace plugins {

using namespace anitools::imgproc;

class ImageFilterPluginLoader
{
public:
    ImageFilterPluginLoader();
    ImageFilterPluginLoader(const QString & path);
    ~ImageFilterPluginLoader();

    bool load(const QString & path);
    void clear();
    ImageFilter * instantiateFilter(const QString & id);
    QStringList ids();
    QHash<QString, QString> info(const QString & id);
    int count();

private:
    QHash<QString, QPair<QLibrary *, QHash<QString, QString> > > mLibraries;
};

}}

#endif // ANITOOLS_PLUGINS_IMAGEFILTERPLUGINLOADER_H
