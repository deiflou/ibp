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

#ifndef IBP_PLUGINS_IMAGEFILTERPLUGINLOADER_H
#define IBP_PLUGINS_IMAGEFILTERPLUGINLOADER_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QLibrary>

#include "../imgproc/imagefilter.h"

namespace ibp {
namespace plugins {

using namespace ibp::imgproc;

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

#endif // IBP_PLUGINS_IMAGEFILTERPLUGINLOADER_H
