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
