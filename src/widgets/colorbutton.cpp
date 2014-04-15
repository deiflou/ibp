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

#include "colorbutton.h"
#include "genericdialog.h"

namespace anitools {
namespace widgets {

ColorButton::ColorButton(QWidget *parent) :
    ToolButtonEx(parent),
    mColorPickerFlags(ColorPicker::None)
{
    setExtraDataType(Color);
    setExtraDataFlags(CheckerboardBackground);

    connect(this, SIGNAL(clicked()), this, SLOT(On_Clicked()));
}

ColorPicker::ColorPickerFlags ColorButton::colorPickerFlags() const
{
    return mColorPickerFlags;
}

void ColorButton::setColorPickerFlags(ColorPicker::ColorPickerFlags f)
{
    mColorPickerFlags = f;
}

void ColorButton::setColor(const QColor &c)
{
    if (c == color())
        return;
    ToolButtonEx::setColor(c);
    emit colorChanged(c);
}

void ColorButton::On_Clicked()
{
    GenericDialog dlg(this);
    ColorPicker * cp = new ColorPicker();

    dlg.setWidget(cp);
    dlg.setFixedSize(dlg.minimumSizeHint());
    dlg.setWindowFlags((dlg.windowFlags() & ~Qt::WindowContextHelpButtonHint) | Qt::MSWindowsFixedSizeDialogHint);

    QColor clr = color();
    cp->setColor(clr);
    cp->setFlags(mColorPickerFlags);

    connect(cp, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));

    if (dlg.exec() == QDialog::Rejected)
    {
        setColor(clr);
    }
}

}}
