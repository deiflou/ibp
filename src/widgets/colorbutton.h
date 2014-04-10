#ifndef ANITOOLS_WIDGETS_COLORBUTTON_H
#define ANITOOLS_WIDGETS_COLORBUTTON_H

#include "toolbuttonex.h"

namespace anitools {
namespace widgets {

class ColorButton : public ToolButtonEx
{
    Q_OBJECT
public:
    explicit ColorButton(QWidget *parent = 0);

private:
    inline void setPopupMode(ToolButtonPopupMode mode)
    {
        ToolButtonEx::setPopupMode(mode);
    }
    inline QString	text() const
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
    inline ExtraDataFlags extraDataFlags() const
    {
        return ToolButtonEx::extraDataFlags();
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
    inline void setExtraDataFlags(ExtraDataFlags flags)
    {
        ToolButtonEx::setExtraDataFlags(flags);
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
