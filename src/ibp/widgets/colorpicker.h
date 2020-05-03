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

#ifndef IBP_WIDGETS_COLORPICKER_H
#define IBP_WIDGETS_COLORPICKER_H

#include <QWidget>

#include "colorslider.h"

namespace ibp {
namespace widgets {

namespace Ui {
class ColorPicker;
}

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    enum ColorPickerFlag
    {
        None = 0,
        HideAlpha = 1
    };
    Q_DECLARE_FLAGS(ColorPickerFlags, ColorPickerFlag)

    explicit ColorPicker(QWidget *parent = 0);
    ~ColorPicker();

    QColor color() const;
    ColorPickerFlags flags() const;

    void setFlags(ColorPickerFlags f);

protected:
    bool eventFilter(QObject *o, QEvent *e);

private:
    Ui::ColorPicker *ui;

    bool mCanUpdate;
    ColorSlider * mMainSlider;
    QColor mReferenceColor;
    QImage mBackgroundImage;
    ColorPickerFlags mFlags;

    void rgbChanged(unsigned int srcColor);
    void hsvChanged(unsigned int srcColor);
    void hslChanged(unsigned int srcColor);
    void labChanged(unsigned int srcColor);
    void cmykChanged(unsigned int srcColor);

    void setWidgetReferenceColor();

signals:
    void colorChanged(const QColor & color);

public slots:
    void setColor(const QColor & color);

private slots:
    void on_mSliderBig_valueChanged(int v);
    void on_mBoxColor_colorChanged();

    void on_mSliderRed_valueChanged(int v);
    void on_mSliderGreen_valueChanged(int v);
    void on_mSliderBlue_valueChanged(int v);
    void on_mSliderAlpha_valueChanged(int v);
    void on_mSliderHue1_valueChanged(int v);
    void on_mSliderSaturation1_valueChanged(int v);
    void on_mSliderValue_valueChanged(int v);
    void on_mSliderHue2_valueChanged(int v);
    void on_mSliderSaturation2_valueChanged(int v);
    void on_mSliderLightness_valueChanged(int v);
    void on_mSliderCIEL_valueChanged(int v);
    void on_mSliderCIEa_valueChanged(int v);
    void on_mSliderCIEb_valueChanged(int v);
    void on_mSliderCyan_valueChanged(int v);
    void on_mSliderMagenta_valueChanged(int v);
    void on_mSliderYellow_valueChanged(int v);
    void on_mSliderBlack_valueChanged(int v);

    void on_mSpinRed_valueChanged(int v);
    void on_mSpinGreen_valueChanged(int v);
    void on_mSpinBlue_valueChanged(int v);
    void on_mSpinAlpha_valueChanged(int v);
    void on_mSpinHue1_valueChanged(int v);
    void on_mSpinSaturation1_valueChanged(int v);
    void on_mSpinValue_valueChanged(int v);
    void on_mSpinHue2_valueChanged(int v);
    void on_mSpinSaturation2_valueChanged(int v);
    void on_mSpinLightness_valueChanged(int v);
    void on_mSpinCIEL_valueChanged(int v);
    void on_mSpinCIEa_valueChanged(int v);
    void on_mSpinCIEb_valueChanged(int v);
    void on_mSpinCyan_valueChanged(int v);
    void on_mSpinMagenta_valueChanged(int v);
    void on_mSpinYellow_valueChanged(int v);
    void on_mSpinBlack_valueChanged(int v);

    void on_mButtonRed_toggled(bool c);
    void on_mButtonGreen_toggled(bool c);
    void on_mButtonBlue_toggled(bool c);
    void on_mButtonHue1_toggled(bool c);
    void on_mButtonSaturation1_toggled(bool c);
    void on_mButtonValue_toggled(bool c);
    void on_mButtonHue2_toggled(bool c);
    void on_mButtonSaturation2_toggled(bool c);
    void on_mButtonLightness_toggled(bool c);
    void on_mButtonCIEL_toggled(bool c);
    void on_mButtonCIEa_toggled(bool c);
    void on_mButtonCIEb_toggled(bool c);
    void on_mButtonCyan_toggled(bool c);
    void on_mButtonMagenta_toggled(bool c);
    void on_mButtonYellow_toggled(bool c);

    void on_mLineEditHex_textEdited(const QString & s);
};

}}

#endif // IBP_WIDGETS_COLORPICKER_H
