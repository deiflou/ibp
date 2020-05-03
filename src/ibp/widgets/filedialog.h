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

#ifndef IBP_WIDGETS_FILEDIALOG_H
#define IBP_WIDGETS_FILEDIALOG_H

#include <QString>
#include <QWidget>
#include <QFileDialog>

namespace ibp {
namespace widgets {

QString getOpenFileName(QWidget * parent = 0,
                        const QString & fileType = QString(),
                        const QString & filter = QString(),
                        QString * selectedFilter = 0,
                        QFileDialog::Options options = 0);

QString getSaveFileName(QWidget * parent = 0,
                        const QString & fileType = QString(),
                        const QString & filter = QString(),
                        QString * selectedFilter = 0,
                        QFileDialog::Options options = 0);

}}

#endif // IBP_WIDGETS_FILEDIALOG_H
