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

#include <QMessageBox>

#include "imagebutton.h"
#include "filedialog.h"
#include "../imgproc/freeimage.h"

namespace anitools {
namespace widgets {

using namespace anitools::imgproc;

ImageButton::ImageButton(QWidget *parent) :
    ToolButtonEx(parent)
{
    setExtraDataType(Image);
    setExtraDataFlags(CheckerboardBackground);

    connect(this, SIGNAL(clicked()), this, SLOT(On_Clicked()));
}

void ImageButton::setImage(const QImage & i)
{
    if (i == image())
        return;
    ToolButtonEx::setImage(i);
    emit imageChanged(i);
}

void ImageButton::On_Clicked()
{
    QString fileName;
    fileName = getOpenFileName(this, "images", freeimageGetOpenFilterString());
    if (fileName.isEmpty())
        return;

    QImage img = freeimageLoadAs32Bits(fileName, true);
    if (img.isNull())
    {
        QMessageBox::information(this, QString(), tr("The selected file has an unsupported format."));
        return;
    }

    setImage(img);
}

}}
