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

#include <QFileDialog>
#include <QMessageBox>

#include <math.h>

#include "filterwidget.h"
#include "ui_filterwidget.h"
#include "../imgproc/freeimage.h"
#include "../widgets/toolbuttonex.h"

using namespace anitools::widgets;

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true)
{
    ui->setupUi(this);
    ui->mContainerImage->hide();
    ui->mButtonImage->setExtraDataType(ToolButtonEx::Image);
    ui->mButtonImage->setExtraDataFlags(ToolButtonEx::ExtraDataFlags(ToolButtonEx::ImageStretched |
                                                                     ToolButtonEx::ImageKeepAspectRatio |
                                                                     ToolButtonEx::ImageStretchOnlyIfBiggerThanButton |
                                                                     ToolButtonEx::CheckerboardBackground));
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setMode(int m)
{
    if ((m == 0 && ui->mButtonAuto->isChecked()) || (m == 1 && ui->mButtonFromImage->isChecked()))
        return;
    mEmitSignals = false;
    if (m == 0)
        ui->mButtonAuto->setChecked(true);
    else
        ui->mButtonFromImage->setChecked(true);
    mEmitSignals = true;
    emit modeChanged(m);
}

void FilterWidget::setImage(const QImage &i)
{
    if (i == ui->mButtonImage->image())
        return;
    mEmitSignals = false;
    ui->mButtonImage->setImage(i);
    mEmitSignals = true;
    emit imageChanged(i);
}

void FilterWidget::on_mButtonAuto_toggled(bool c)
{
    if (!c)
        return;
    ui->mContainerImage->hide();
    if (mEmitSignals)
        emit modeChanged(0);
}

void FilterWidget::on_mButtonFromImage_toggled(bool c)
{
    if (!c)
        return;
    ui->mContainerImage->show();
    if (mEmitSignals)
        emit modeChanged(1);
}

void FilterWidget::on_mButtonImage_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, QString(), QString(), freeimageGetOpenFilterString());
    if (fileName.isEmpty()) return;

    QImage img = freeimageLoadAs32Bits(fileName, true);
    if (img.isNull())
    {
        QMessageBox::information(this, QString(), tr("The selected file has an unsupported format."));
        return;
    }

    setImage(img);
}
