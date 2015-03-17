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

#include "colorcompositionmodecombobox.h"

namespace anitools {
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
