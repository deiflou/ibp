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

#include <QHash>

#include "filter.h"
#include <imgproc/imagefilter.h>

using namespace ibp::imgproc;

#ifdef Q_OS_WIN32
#define IBP_EXPORT __declspec(dllexport)
#else
#define IBP_EXPORT
#endif

extern "C" IBP_EXPORT QHash<QString, QString> getIBPPluginInfo()
{
    QHash<QString, QString> info;

    info.insert("id", "ibp.imagefilter.nlmdenoising");
    info.insert("version", "0.1.0");
    info.insert("name", QObject::tr("Non-Local Means Denoising"));
    info.insert("description", QObject::tr("Removes the noise from the image using semi-local information"));
    info.insert("tags", QObject::tr("Smooth, Noise"));
    info.insert("author", QObject::tr("Deif Lou"));
    info.insert("copyright", QObject::tr(""));
    info.insert("url", QObject::tr(""));
    return info;
}

extern "C" IBP_EXPORT ImageFilter * getImageFilterInstance()
{
    return new Filter();
}
