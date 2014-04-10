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
};


} // namespace widgets
} // namespace anitools
#endif // ANITOOLS_WIDGETS_COLORPICKER_H
