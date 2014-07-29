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

#include "QApplication"
#include "QFileInfo"

#include "filedialog.h"
#include "../misc/configurationmanager.h"

using namespace anitools::misc;

namespace anitools {
namespace widgets {

QString getOpenFileName(QWidget *parent,
                        const QString &fileType,
                        const QString &filter,
                        QString *selectedFilter,
                        QFileDialog::Options options)
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(
                   parent, QString(),
                   fileType.isEmpty() ? QString() : ConfigurationManager::value("searchpaths/" + fileType,
                                                    QApplication::applicationDirPath()).toString(),
                   filter, selectedFilter, options
               );

    if (!fileName.isEmpty())
    {
        ConfigurationManager::setValue("searchpaths/" + fileType, QFileInfo(fileName).absolutePath());
    }

    return fileName;
}

QString getSaveFileName(QWidget *parent,
                        const QString &fileType,
                        const QString &filter,
                        QString *selectedFilter,
                        QFileDialog::Options options)
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(
                   parent, QString(),
                   fileType.isEmpty() ? QString() : ConfigurationManager::value("searchpaths/" + fileType,
                                                    QApplication::applicationDirPath()).toString(),
                   filter, selectedFilter, options
               );

    if (!fileName.isEmpty())
    {
        ConfigurationManager::setValue("searchpaths/" + fileType, QFileInfo(fileName).absolutePath());
    }

    return fileName;
}

}}
