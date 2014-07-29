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

#ifndef ANITOOLS_WIDGETS_FILEDIALOG_H
#define ANITOOLS_WIDGETS_FILEDIALOG_H

#include <QString>
#include <QWidget>
#include <QFileDialog>

namespace anitools {
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

#endif // ANITOOLS_WIDGETS_FILEDIALOG_H
