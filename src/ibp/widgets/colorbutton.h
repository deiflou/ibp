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

#ifndef IBP_WIDGETS_COLORBUTTON_H
#define IBP_WIDGETS_COLORBUTTON_H

#include "toolbuttonex.h"
#include "colorpicker.h"

namespace ibp {
namespace widgets {

class ColorButton : public ToolButtonEx
{
    Q_OBJECT
public:
    explicit ColorButton(QWidget *parent = 0);

    ColorPicker::ColorPickerFlags colorPickerFlags() const;
    void setColorPickerFlags(ColorPicker::ColorPickerFlags f);

private:
    ColorPicker::ColorPickerFlags mColorPickerFlags;

    inline void setPopupMode(ToolButtonPopupMode mode)
    {
        ToolButtonEx::setPopupMode(mode);
    }
    inline QString text() const
    {
        return ToolButtonEx::text();
    }
    inline int maximumTextWidth() const
    {
        return ToolButtonEx::maximumTextWidth();
    }
    inline Qt::TextElideMode textElideMode() const
    {
        return ToolButtonEx::textElideMode();
    }
    inline ExtraDataType extraDataType() const
    {
        return ToolButtonEx::extraDataType();
    }
    inline double progress() const
    {
        return ToolButtonEx::progress();
    }
    inline QGradientStops gradientStops() const
    {
        return ToolButtonEx::gradientStops();
    }
    inline QImage image() const
    {
        return ToolButtonEx::image();
    }
    inline void initStyleOption(QStyleOptionToolButton * o) const
    {
        ToolButtonEx::initStyleOption(o);
    }

signals:
    void colorChanged(const QColor & c);

public slots:
    void setColor(const QColor &c);

private slots:
    inline void setText(const QString & text)
    {
        ToolButtonEx::setText(text);
    }
    inline void setMaximumTextWidth(int tw)
    {
        ToolButtonEx::setMaximumTextWidth(tw);
    }
    inline void setTextElideMode(Qt::TextElideMode tem)
    {
        ToolButtonEx::setTextElideMode(tem);
    }
    inline void setExtraDataType(ExtraDataType t)
    {
        ToolButtonEx::setExtraDataType(t);
    }
    inline void setProgress(double p)
    {
        ToolButtonEx::setProgress(p);
    }
    inline void setGradientStops(const QGradientStops & s)
    {
        ToolButtonEx::setGradientStops(s);
    }
    inline void setImage(const QImage & i)
    {
        ToolButtonEx::setImage(i);
    }

    void On_Clicked();
};

}}

#endif // IBP_WIDGETS_COLORBUTTON_H
