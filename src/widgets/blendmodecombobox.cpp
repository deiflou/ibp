#include "blendmodecombobox.h"

namespace anitools {
namespace widgets {

BlendModeComboBox::BlendModeComboBox(QWidget *parent) :
    QComboBox(parent)
{
    this->addItem(tr("Normal"), BlendMode_Normal);
    this->insertSeparator(this->count());
    this->addItem(tr("Darken"), BlendMode_Darken);
    this->addItem(tr("Multiply"), BlendMode_Multiply);
    this->addItem(tr("Color Burn"), BlendMode_ColorBurn);
    this->addItem(tr("Linear Burn (Subtractive)"), BlendMode_LinearBurn);
    this->addItem(tr("Darker Color"), BlendMode_DarkerColor);
    this->insertSeparator(this->count());
    this->addItem(tr("Lighten"), BlendMode_Lighten);
    this->addItem(tr("Screen"), BlendMode_Screen);
    this->addItem(tr("Color Dodge"), BlendMode_ColorDodge);
    this->addItem(tr("Linear Dodge (Additive)"), BlendMode_LinearDodge);
    this->addItem(tr("Lighter Color"), BlendMode_LighterColor);
    this->insertSeparator(this->count());
    this->addItem(tr("Overlay"), BlendMode_Overlay);
    this->addItem(tr("Soft Light"), BlendMode_softLight);
    this->addItem(tr("Hard Light"), BlendMode_HardLight);
    this->addItem(tr("Vivid Light"), BlendMode_VividLight);
    this->addItem(tr("Linear Light"), BlendMode_LinearLight);
    this->addItem(tr("Pin Light"), BlendMode_PinLight);
    this->addItem(tr("Hard Mix"), BlendMode_HardMix);
    this->insertSeparator(this->count());
    this->addItem(tr("Difference"), BlendMode_Difference);
    this->addItem(tr("Exclusion"), BlendMode_Exclusion);
    this->insertSeparator(this->count());
    this->addItem(tr("Hue"), BlendMode_Hue);
    this->addItem(tr("Saturation"), BlendMode_Saturation);
    this->addItem(tr("Color"), BlendMode_Color);
    this->addItem(tr("Luminosity"), BlendMode_Luminosity);
    this->setCurrentIndex(0);
    this->setMaxVisibleItems(29);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(On_currentIndexChanged(int)));
}

BlendMode BlendModeComboBox::blendMode() const
{
    return (BlendMode)this->itemData(this->currentIndex()).toInt();
}

void BlendModeComboBox::setBlendMode(BlendMode v)
{
    if (v == blendMode())
        return;
    if (v < BlendMode_Normal || v > BlendMode_Luminosity)
        return;
    this->setCurrentIndex(this->findData(v));
    blendModeChanged(v);
}

void BlendModeComboBox::On_currentIndexChanged(int i)
{
    emit blendModeChanged((BlendMode)this->itemData(i).toInt());
}

}}
