#include "colorpicker.h"
#include "ui_colorpicker.h"
#include "../imgproc/types.h"

namespace anitools {
namespace widgets {

using namespace anitools::imgproc;

ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorPicker)
{
    ui->setupUi(this);

    ui->mSliderBig->setColorModel(ColorModel_BGR);
    ui->mSliderBig->setColorChannel(ColorChannel_Red);

    ui->mSliderRed->setColorModel(ColorModel_BGR);
    ui->mSliderRed->setColorChannel(ColorChannel_Red);
    ui->mSliderGreen->setColorModel(ColorModel_BGR);
    ui->mSliderGreen->setColorChannel(ColorChannel_Green);
    ui->mSliderBlue->setColorModel(ColorModel_BGR);
    ui->mSliderBlue->setColorChannel(ColorChannel_Blue);

    ui->mSliderHue1->setColorModel(ColorModel_HSV);
    ui->mSliderHue1->setColorChannel(ColorChannel_Hue);
    ui->mSliderSaturation1->setColorModel(ColorModel_HSV);
    ui->mSliderSaturation1->setColorChannel(ColorChannel_Saturation);
    ui->mSliderValue->setColorModel(ColorModel_HSV);
    ui->mSliderValue->setColorChannel(ColorChannel_Value);

    ui->mSliderHue2->setColorModel(ColorModel_HSL);
    ui->mSliderHue2->setColorChannel(ColorChannel_Hue);
    ui->mSliderSaturation2->setColorModel(ColorModel_HSL);
    ui->mSliderSaturation2->setColorChannel(ColorChannel_Saturation);
    ui->mSliderLightness->setColorModel(ColorModel_HSL);
    ui->mSliderLightness->setColorChannel(ColorChannel_Lightness);
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

}}
