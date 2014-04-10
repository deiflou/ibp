#include "colorbutton.h"
#include "genericdialog.h"
#include "colorpicker.h"

namespace anitools {
namespace widgets {

ColorButton::ColorButton(QWidget *parent) :
    ToolButtonEx(parent)
{
    setExtraDataType(Color);
    setExtraDataFlags(CheckerboardBackground);

    connect(this, SIGNAL(clicked()), this, SLOT(On_Clicked()));
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


    //connect(&dlg, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));

    if (dlg.exec() == QDialog::Rejected)
    {
        //setColor(color);
    }
}

}}
