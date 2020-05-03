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

#include <QApplication>
#ifdef USE_USER_CONFIGURATION_FOLDER
#include <QStandardPaths>
#endif

#include "mainwindow.h"
#include "../misc/configurationmanager.h"
#include "../widgets/style.h"

using namespace ibp::misc;
using namespace ibp::widgets;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(widgets);

    QApplication a(argc, argv);
    a.setApplicationName("Image Batch Processor");
    a.setApplicationDisplayName("Image Batch Processor");

    setIBPStyle();

#ifdef USE_USER_CONFIGURATION_FOLDER
    ConfigurationManager::setFolder(QStandardPaths::writableLocation(GenericDataLocation) + "/ibp");
#else
    ConfigurationManager::setFolder(a.applicationDirPath() + "/settings");
#endif
    ConfigurationManager::setFileName("imagebatchprocessor.cfg");

    MainWindow w;
    w.setWindowTitle(a.applicationDisplayName());
    w.show();

    return a.exec();
}
