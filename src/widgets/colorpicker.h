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

#ifndef ANITOOLS_WIDGETS_COLORPICKER_H
#define ANITOOLS_WIDGETS_COLORPICKER_H

#include <QWidget>

#include "colorslider.h"

namespace anitools {
namespace widgets {

namespace Ui {
class ColorPicker;
}

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPicker(QWidget *parent = 0);
    ~ColorPicker();

    QColor color() const;

private:
    Ui::ColorPicker *ui;

    bool mCanUpdate;
    ColorSlider * mMainSlider;

    void rgbChanged(unsigned int srcColor);
    void hsvChanged(unsigned int srcColor);
    void hslChanged(unsigned int srcColor);
    void labChanged(unsigned int srcColor);
    void cmykChanged(unsigned int srcColor);

signals:
    void colorChanged(const QColor & color);

public slots:
    void setColor(const QColor & color);

private slots:
    void on_mSliderBig_valueChanged(int v);

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

    void on_mBoxColor_colorChanged();
};

}}

#endif // ANITOOLS_WIDGETS_COLORPICKER_H
