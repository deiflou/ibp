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

#include "genericdialog.h"
#include "ui_genericdialog.h"

namespace anitools {
namespace widgets {

GenericDialog::GenericDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericDialog),
    mWidget(0)
{
    ui->setupUi(this);
}

GenericDialog::~GenericDialog()
{
    delete ui;

    if (mWidget)
        delete mWidget;
}

QDialogButtonBox *GenericDialog::buttonBox() const
{
    return ui->mButtonBox;
}

QWidget *GenericDialog::widget() const
{
    return mWidget;
}

void GenericDialog::setWidget(QWidget *w)
{
    if (mWidget)
        delete mWidget;
    mWidget = w;
    if (!mWidget)
        return;
    mWidget->setParent(this);
    ui->mContainerWidget->layout()->addWidget(mWidget);
    setWindowTitle(mWidget->windowTitle());
}

}}
