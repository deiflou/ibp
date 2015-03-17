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

#include <QApplication>
#ifdef USE_USER_CONFIGURATION_FOLDER
#include <QStandardPaths>
#endif

#include "mainwindow.h"
#include "../misc/configurationmanager.h"
#include "../widgets/style.h"

using namespace anitools::misc;
using namespace anitools::widgets;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(widgets);

    QApplication a(argc, argv);
    a.setApplicationDisplayName("Anitools Image Batch Processor");

    setAnitoolsStyle();

#ifdef USE_USER_CONFIGURATION_FOLDER
    ConfigurationManager::setFolder(QStandardPaths::writableLocation(GenericDataLocation) + "/anitools");
#else
    ConfigurationManager::setFolder(a.applicationDirPath() + "/settings");
#endif
    ConfigurationManager::setFileName("imagebatchprocessor.cfg");

    MainWindow w;
    w.show();

    return a.exec();
}
