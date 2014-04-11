#ifndef ANITOOLS_WIDGETS_COLORPICKER_H
#define ANITOOLS_WIDGETS_COLORPICKER_H

#include <QWidget>

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

private:
    Ui::ColorPicker *ui;

    bool mCanUpdate;

    void rgbChanged(unsigned int srcColor);
    void hsvChanged(unsigned int srcColor);
    void hslChanged(unsigned int srcColor);
    void labChanged(unsigned int srcColor);
    void cmykChanged(unsigned int srcColor);

private slots:
    void on_mSliderRed_valueChanged(int v);
    void on_mSliderGreen_valueChanged(int v);
    void on_mSliderBlue_valueChanged(int v);
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
};


} // namespace widgets
} // namespace anitools
#endif // ANITOOLS_WIDGETS_COLORPICKER_H
