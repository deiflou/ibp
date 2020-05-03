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

#include "colorbutton.h"
#include "genericdialog.h"

namespace ibp {
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
