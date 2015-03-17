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

#ifndef ANITOOLS_WIDGETS_COLORBUTTON_H
#define ANITOOLS_WIDGETS_COLORBUTTON_H

#include "toolbuttonex.h"
#include "colorpicker.h"

namespace anitools {
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

#endif // ANITOOLS_WIDGETS_COLORBUTTON_H
