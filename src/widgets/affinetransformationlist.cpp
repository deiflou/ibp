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


#include "affinetransformationlist.h"
#include "ui_affinetransformationlist.h"

namespace anitools {
namespace widgets {

AffineTransformationList::AffineTransformationList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AffineTransformationList)
{
    ui->setupUi(this);
    ui->mWidgetList->setItemMargins(QMargins(0, 0, 0, 0));
    ui->mWidgetList->setItemWidgetMargins(QMargins(5, 0, 5, 5));
    ui->mWidgetList->setEmptyMessage("");
    ui->mWidgetList->setPlaceholderVisible(false);
    ui->mWidgetList->setItemContentsFrameVisible(false);
}

AffineTransformationList::~AffineTransformationList()
{
    delete ui;
}

void AffineTransformationList::on_mButtonAppend_clicked()
{
    ui->mWidgetList->append(new QLabel("hola", ui->mWidgetList));
}

}}

