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

#ifndef ANITOOLS_WIDGETS_AFFINETRANSFORMATIONLIST_H
#define ANITOOLS_WIDGETS_AFFINETRANSFORMATIONLIST_H

#include <QWidget>

#include "../imgproc/types.h"

namespace anitools {
namespace widgets {

using namespace anitools::imgproc;

namespace Ui {
class AffineTransformationList;
}

class AffineTransformationList : public QWidget
{
    Q_OBJECT

public:
    explicit AffineTransformationList(QWidget *parent = 0);
    ~AffineTransformationList();

    QList<AffineTransformation> transformations() const;
    QList<bool> bypasses() const;

private:
    Ui::AffineTransformationList *ui;

    QList<AffineTransformation> mTransformations;
    QList<bool> mBypasses;

    void appendTransformation(const AffineTransformation & t);

signals:
    void transformationsChanged();

public slots:
    void setTransformations(const QList<AffineTransformation> & t);
    void setBypasses(const QList<bool> &b);

private slots:
    void On_mButtonAppend_menuActionClicked();
    void On_valueChanged(double v);

    void on_mWidgetList_widgetInserted(int i);
    void on_mWidgetList_widgetMoved(int from, int to);
    void on_mWidgetList_widgetRemoved(int i);
    void on_mWidgetList_widgetBypassStatusChanged(int i, bool c);
};

}}

#endif // ANITOOLS_WIDGETS_AFFINETRANSFORMATIONLIST_H
