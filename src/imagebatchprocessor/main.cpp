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

#include <QApplication>
#ifdef USE_USER_CONFIGURATION_FOLDER
#include <QStandardPaths>
#endif

#include "mainwindow.h"
#include "../misc/configurationmanager.h"

using namespace anitools::misc;

void setDarkPalette()
{
    static QPalette darkPalette;
    static bool isInitialized = false;
    if (!isInitialized)
    {
        darkPalette.setColor(QPalette::Window, QColor(53,53,53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25,25,25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53,53,53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(227, 0, 79));
        darkPalette.setColor(QPalette::Highlight, QColor(0, 180, 230));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        darkPalette.setColor(QPalette::Light, QColor(80, 80, 80));
        darkPalette.setColor(QPalette::Midlight, QColor(65, 65, 65));
        darkPalette.setColor(QPalette::Mid, QColor(40, 40, 40));
        darkPalette.setColor(QPalette::Dark, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::Shadow, QColor(0, 0, 0));
    }
    QApplication::setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: rgb(128, 128, 128); background-color: palette(dark);"
                        "border: 1px solid palette(shadow); padding: 0px 5px; }");
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(widgets);

    QApplication a(argc, argv);
    a.setApplicationDisplayName("Anitools Image Batch Processor");
    a.setObjectName("App");
    setDarkPalette();
    QApplication::setStyle("Fusion");

#ifdef USE_USER_CONFIGURATION_FOLDER
    ConfigurationManager::setFolder(QStandardPaths::writableLocation(GenericDataLocation) + "/anitools");
#else
    ConfigurationManager::setFolder(a.applicationDirPath() + "/settings");
#endif
    ConfigurationManager::setFileName("imagebatchprocessor.cfg");

    MainWindow w;
    w.setObjectName("MainWindow");
    w.show();

    return a.exec();
}
