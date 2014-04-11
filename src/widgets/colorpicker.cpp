#include "colorpicker.h"
#include "ui_colorpicker.h"
#include "../imgproc/types.h"

namespace anitools {
namespace widgets {

using namespace anitools::imgproc;

ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorPicker),
    mCanUpdate(false)
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

    ui->mSliderCIEL->setColorModel(ColorModel_Lab);
    ui->mSliderCIEL->setColorChannel(ColorChannel_CIEL);
    ui->mSliderCIEa->setColorModel(ColorModel_Lab);
    ui->mSliderCIEa->setColorChannel(ColorChannel_CIEa);
    ui->mSliderCIEb->setColorModel(ColorModel_Lab);
    ui->mSliderCIEb->setColorChannel(ColorChannel_CIEb);

    ui->mSliderCyan->setColorModel(ColorModel_CMYK);
    ui->mSliderCyan->setColorChannel(ColorChannel_Cyan);
    ui->mSliderMagenta->setColorModel(ColorModel_CMYK);
    ui->mSliderMagenta->setColorChannel(ColorChannel_Magenta);
    ui->mSliderYellow->setColorModel(ColorModel_CMYK);
    ui->mSliderYellow->setColorChannel(ColorChannel_Yellow);
    ui->mSliderBlack->setColorModel(ColorModel_CMYK);
    ui->mSliderBlack->setColorChannel(ColorChannel_Black);

    mCanUpdate = true;
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

#define CHANGE_BGR \
    ui->mSliderRed->setColor(*dstColor2); \
    ui->mSliderGreen->setColor(*dstColor2); \
    ui->mSliderBlue->setColor(*dstColor2); \
    ui->mSpinRed->setValue(dstColor[ColorChannel_Red]); \
    ui->mSpinGreen->setValue(dstColor[ColorChannel_Green]); \
    ui->mSpinBlue->setValue(dstColor[ColorChannel_Blue]);
#define CHANGE_HSV \
    ui->mSliderHue1->setColor(*dstColor2); \
    ui->mSliderSaturation1->setColor(*dstColor2); \
    ui->mSliderValue->setColor(*dstColor2); \
    ui->mSpinHue1->setValue(qRound(dstColor[ColorChannel_Hue] * 359 / 255.)); \
    ui->mSpinSaturation1->setValue(qRound(dstColor[ColorChannel_Saturation] * 100 / 255.)); \
    ui->mSpinValue->setValue(qRound(dstColor[ColorChannel_Value] * 100 / 255.));
#define CHANGE_HSL \
    ui->mSliderHue2->setColor(*dstColor2); \
    ui->mSliderSaturation2->setColor(*dstColor2); \
    ui->mSliderLightness->setColor(*dstColor2); \
    ui->mSpinHue2->setValue(qRound(dstColor[ColorChannel_Hue] * 359 / 255.)); \
    ui->mSpinSaturation2->setValue(qRound(dstColor[ColorChannel_Saturation] * 100 / 255.)); \
    ui->mSpinLightness->setValue(qRound(dstColor[ColorChannel_Lightness] * 100 / 255.));
#define CHANGE_LAB \
    ui->mSliderCIEL->setColor(*dstColor2); \
    ui->mSliderCIEa->setColor(*dstColor2); \
    ui->mSliderCIEb->setColor(*dstColor2); \
    ui->mSpinCIEL->setValue(qRound(dstColor[ColorChannel_CIEL] * 100 / 255.)); \
    ui->mSpinCIEa->setValue(dstColor[ColorChannel_CIEa] - 128); \
    ui->mSpinCIEb->setValue(dstColor[ColorChannel_CIEb] - 128);
#define CHANGE_CMYK \
    ui->mSliderCyan->setColor(*dstColor2); \
    ui->mSliderMagenta->setColor(*dstColor2); \
    ui->mSliderYellow->setColor(*dstColor2); \
    ui->mSliderBlack->setColor(*dstColor2); \
    ui->mSpinCyan->setValue(qRound(dstColor[ColorChannel_Cyan] * 100 / 255.)); \
    ui->mSpinMagenta->setValue(qRound(dstColor[ColorChannel_Magenta] * 100 / 255.)); \
    ui->mSpinYellow->setValue(qRound(dstColor[ColorChannel_Yellow] * 100 / 255.)); \
    ui->mSpinBlack->setValue(qRound(dstColor[ColorChannel_Black] * 100 / 255.));

void ColorPicker::rgbChanged(unsigned int srcColor)
{
    unsigned char dstColor[4];
    unsigned int * dstColor2 = (unsigned int *)dstColor;

    convertBGRToHSV((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_HSV
    convertBGRToHSL((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_HSL
    convertBGRToLab((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_LAB
    convertBGRToCMYK((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_CMYK
}

void ColorPicker::hsvChanged(unsigned int srcColor)
{
    unsigned char dstColor[4];
    unsigned int * dstColor2 = (unsigned int *)dstColor;

    convertHSVToBGR((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_BGR
    convertHSVToHSL((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_HSL
    convertHSVToLab((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_LAB
    convertHSVToCMYK((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_CMYK
}

void ColorPicker::hslChanged(unsigned int srcColor)
{
    unsigned char dstColor[4];
    unsigned int * dstColor2 = (unsigned int *)dstColor;

    convertHSLToBGR((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_BGR
    convertHSLToHSV((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_HSV
    convertHSLToLab((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_LAB
    convertHSLToCMYK((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_CMYK
}

void ColorPicker::labChanged(unsigned int srcColor)
{
    unsigned char dstColor[4];
    unsigned int * dstColor2 = (unsigned int *)dstColor;

    convertLabToBGR((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_BGR
    convertLabToHSV((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_HSV
    convertLabToHSL((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_HSL
    convertLabToCMYK((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_CMYK
}

void ColorPicker::cmykChanged(unsigned int srcColor)
{
    unsigned char dstColor[4];
    unsigned int * dstColor2 = (unsigned int *)dstColor;

    convertCMYKToBGR((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_BGR
    convertCMYKToHSV((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_HSV
    convertCMYKToHSL((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_HSL
    convertCMYKToLab((unsigned char *)&srcColor, dstColor, 1);
    CHANGE_LAB
}

#define SLIDER_CODE(arg1, arg2, arg3, arg4, arg5) \
void ColorPicker::on_mSlider##arg1##_valueChanged(int v) \
{ \
    if (!mCanUpdate) \
        return; \
    mCanUpdate = false; \
    unsigned int srcColor; \
    ui->mSlider##arg1->color(&srcColor); \
    ui->mSlider##arg2->setColor(srcColor); \
    ui->mSlider##arg3->setColor(srcColor); \
    ui->mSpin##arg1->setValue(arg5); \
    arg4##Changed(srcColor); \
    mCanUpdate = true; \
}
#define SLIDER_CODE2(arg1, arg2, arg3, arg4, arg5, arg6) \
void ColorPicker::on_mSlider##arg1##_valueChanged(int v) \
{ \
    if (!mCanUpdate) \
        return; \
    mCanUpdate = false; \
    unsigned int srcColor; \
    ui->mSlider##arg1->color(&srcColor); \
    ui->mSlider##arg2->setColor(srcColor); \
    ui->mSlider##arg3->setColor(srcColor); \
    ui->mSlider##arg4->setColor(srcColor); \
    ui->mSpin##arg1->setValue(arg6); \
    arg5##Changed(srcColor); \
    mCanUpdate = true; \
}

SLIDER_CODE(Red, Green, Blue, rgb, v)
SLIDER_CODE(Green, Red, Blue, rgb, v)
SLIDER_CODE(Blue, Red, Green, rgb, v)
SLIDER_CODE(Hue1, Saturation1, Value, hsv, qRound(v * 359 / 255.))
SLIDER_CODE(Saturation1, Hue1, Value, hsv, qRound(v * 100 / 255.))
SLIDER_CODE(Value, Hue1, Saturation1, hsv, qRound(v * 100 / 255.))
SLIDER_CODE(Hue2, Saturation2, Lightness, hsl, qRound(v * 359 / 255.))
SLIDER_CODE(Saturation2, Hue2, Lightness, hsl, qRound(v * 100 / 255.))
SLIDER_CODE(Lightness, Hue2, Saturation2, hsl, qRound(v * 100 / 255.))
SLIDER_CODE(CIEL, CIEa, CIEb, lab, qRound(v * 100 / 255.))
SLIDER_CODE(CIEa, CIEL, CIEb, lab, v - 128)
SLIDER_CODE(CIEb, CIEL, CIEa, lab, v - 128)
SLIDER_CODE2(Cyan, Magenta, Yellow, Black, cmyk, qRound(v * 100 / 255.))
SLIDER_CODE2(Magenta, Cyan, Yellow, Black, cmyk, qRound(v * 100 / 255.))
SLIDER_CODE2(Yellow, Cyan, Magenta, Black, cmyk, qRound(v * 100 / 255.))
SLIDER_CODE2(Black, Cyan, Magenta, Yellow, cmyk, qRound(v * 100 / 255.))

}}
