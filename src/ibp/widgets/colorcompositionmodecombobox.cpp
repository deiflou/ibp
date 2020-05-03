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

#include "colorcompositionmodecombobox.h"

namespace ibp {
namespace widgets {

ColorCompositionModeComboBox::ColorCompositionModeComboBox(QWidget *parent) :
    QComboBox(parent)
{
    this->addItem(tr("Normal"), ColorCompositionMode_Normal);
    this->insertSeparator(this->count());
    this->addItem(tr("Darken"), ColorCompositionMode_Darken);
    this->addItem(tr("Multiply"), ColorCompositionMode_Multiply);
    this->addItem(tr("Color Burn"), ColorCompositionMode_ColorBurn);
    this->addItem(tr("Linear Burn (Subtractive)"), ColorCompositionMode_LinearBurn);
    this->addItem(tr("Darker Color"), ColorCompositionMode_DarkerColor);
    this->insertSeparator(this->count());
    this->addItem(tr("Lighten"), ColorCompositionMode_Lighten);
    this->addItem(tr("Screen"), ColorCompositionMode_Screen);
    this->addItem(tr("Color Dodge"), ColorCompositionMode_ColorDodge);
    this->addItem(tr("Linear Dodge (Additive)"), ColorCompositionMode_LinearDodge);
    this->addItem(tr("Lighter Color"), ColorCompositionMode_LighterColor);
    this->insertSeparator(this->count());
    this->addItem(tr("Overlay"), ColorCompositionMode_Overlay);
    this->addItem(tr("Soft Light"), ColorCompositionMode_softLight);
    this->addItem(tr("Hard Light"), ColorCompositionMode_HardLight);
    this->addItem(tr("Vivid Light"), ColorCompositionMode_VividLight);
    this->addItem(tr("Linear Light"), ColorCompositionMode_LinearLight);
    this->addItem(tr("Pin Light"), ColorCompositionMode_PinLight);
    this->addItem(tr("Hard Mix"), ColorCompositionMode_HardMix);
    this->insertSeparator(this->count());
    this->addItem(tr("Difference"), ColorCompositionMode_Difference);
    this->addItem(tr("Exclusion"), ColorCompositionMode_Exclusion);
    this->insertSeparator(this->count());
    this->addItem(tr("Hue"), ColorCompositionMode_Hue);
    this->addItem(tr("Saturation"), ColorCompositionMode_Saturation);
    this->addItem(tr("Color"), ColorCompositionMode_Color);
    this->addItem(tr("Luminosity"), ColorCompositionMode_Luminosity);
    this->setCurrentIndex(0);
    this->setMaxVisibleItems(29);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(On_currentIndexChanged(int)));
}

ColorCompositionMode ColorCompositionModeComboBox::colorCompositionMode() const
{
    return (ColorCompositionMode)this->itemData(this->currentIndex()).toInt();
}

void ColorCompositionModeComboBox::setColorCompositionMode(ColorCompositionMode v)
{
    if (v == colorCompositionMode())
        return;
    if (v < ColorCompositionMode_Normal || v > ColorCompositionMode_Luminosity)
        return;
    this->setCurrentIndex(this->findData(v));
    colorCompositionModeChanged(v);
}

void ColorCompositionModeComboBox::On_currentIndexChanged(int i)
{
    emit colorCompositionModeChanged((ColorCompositionMode)this->itemData(i).toInt());
}

}}
