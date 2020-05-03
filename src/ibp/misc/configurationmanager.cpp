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

#include "configurationmanager.h"

namespace ibp {
namespace misc {

QString ConfigurationManager::sFolder = QString();
QString ConfigurationManager::sFileName = QString();

QString ConfigurationManager::folder()
{
    return sFolder;
}
void ConfigurationManager::setFolder(const QString & f)
{
    if (f.isEmpty())
        return;
    sFolder = f;
}

QString ConfigurationManager::fileName()
{
    return sFileName;
}
void ConfigurationManager::setFileName(const QString & fn)
{
    if (fn.isEmpty())
        return;
    sFileName = fn;
}

QVariant ConfigurationManager::value(const QString & key, const QVariant & defaultValue)
{
    if (sFileName.isEmpty())
        return defaultValue;
    return QSettings(sFolder + "/" + sFileName, QSettings::IniFormat).value(key, defaultValue);
}

void ConfigurationManager::setValue(const QString & key, const QVariant & value)
{
    if (sFileName.isEmpty())
        return;
    QSettings(sFolder + "/" + sFileName, QSettings::IniFormat).setValue(key, value);
}

}}
