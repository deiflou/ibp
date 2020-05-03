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

#include <QMouseEvent>
#include <QStyle>
#include <QStyleOptionToolButton>
#include <QApplication>
#include <QStylePainter>
#include <QDebug>

#include "toolbuttonex.h"
#include "../misc/util.h"

namespace ibp {
namespace widgets {

using namespace ibp::misc;

ToolButtonEx::ToolButtonEx(QWidget *parent) :
    QToolButton(parent),
    mMaximumTextWidth(0),
    mRealText(""),
    mTextelideMode(Qt::ElideNone),
    mExtraDatatype(None),
    mExtraDataFlags(),
    mProgress(0.0),
    mColor(255, 0, 0),
    mGradientStops(),
    mImage(),
    mMenuButtonDown(false),
    mBackgroundImage(2, 2, QImage::Format_RGB888)
{
}

void ToolButtonEx::mousePressEvent(QMouseEvent *e)
{
    if ((e->buttons() & Qt::LeftButton) && popupMode() == QToolButton::MenuButtonPopup)
    {
        QStyleOptionToolButton opt;

        initStyleOption(&opt);
        QRect r = style()->subControlRect(QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButtonMenu, this);
        if (r.contains(e->pos()))
        {
            emit menuButtonPressed();
        }
    }
    QToolButton::mousePressEvent(e);
}

void ToolButtonEx::paintEvent(QPaintEvent *e)
{
    //QToolButton::paintEvent(e);
    //return;

    QStylePainter p(this);
    QStyleOptionToolButton toolbutton;
    initStyleOption(&toolbutton);
    QStyle::ComplexControl cc = QStyle::CC_ToolButton;

    QRect button, menuarea;
    button = style()->subControlRect(cc, &toolbutton, QStyle::SC_ToolButton, this);
    menuarea = style()->subControlRect(cc, &toolbutton, QStyle::SC_ToolButtonMenu, this);
    QRect extraDataRect = button.adjusted(2, 2, -2, -2);

    QColor outline = (!palette().window().texture().isNull()) ? QColor(0, 0, 0, 160) :
                      palette().background().color().darker(140);
    QColor innerContrastLine = QColor(255, 255, 255, 30);
    QColor highlight = palette().color(QPalette::Highlight);

    QStyle::State flags = toolbutton.state;
    QStyle::State bflags = flags & ~QStyle::State_Sunken;
    bool autoRaise = flags & QStyle::State_AutoRaise;
    bool bData = mExtraDatatype == Progress || mExtraDatatype == Color ||
                 mExtraDatatype == GradientStops || mExtraDatatype == Image;

    if (autoRaise) {
        if (!(bflags & QStyle::State_MouseOver) || !(bflags & QStyle::State_Enabled)) {
            bflags &= ~QStyle::State_Raised;
        }
    }
    QStyle::State mflags = bflags;
    if (flags & QStyle::State_Sunken) {
        if (toolbutton.activeSubControls & QStyle::SC_ToolButton)
        {
            bflags |= QStyle::State_Sunken;
            mflags |= QStyle::State_MouseOver;
        }
        else if (toolbutton.activeSubControls & QStyle::SC_ToolButtonMenu)
        {
            mflags |= QStyle::State_Sunken;
            bflags |= QStyle::State_MouseOver;
        }
    }

    if (mMenuButtonDown)
    {
        mflags |= QStyle::State_Sunken;
        bflags |= QStyle::State_MouseOver;
    }

    QStyleOption tool(0);
    tool.palette = toolbutton.palette;
    if (toolbutton.subControls & QStyle::SC_ToolButton)
    {
        if ((bflags & (QStyle::State_MouseOver | QStyle::State_Sunken |
                 QStyle::State_On | QStyle::State_Raised)) || !autoRaise)
        {
            tool.rect = this->rect();
            tool.state = bflags;
            p.setClipRect(button);
            p.drawPrimitive(QStyle::PE_PanelButtonTool, tool);
            if (!(bflags & QStyle::State_Sunken) && (bflags & QStyle::State_Enabled))
            {
                p.setPen(outline);
                p.drawLine(button.topRight() + QPoint(0, 1), button.bottomRight() - QPoint(0, 1));
            }
        }
        if (bData)
        {
            p.setPen(Qt::NoPen);
            p.setRenderHint(QPainter::Antialiasing);
            p.setRenderHint(QPainter::SmoothPixmapTransform);
            QBrush checkerboardBrush;
            if (mExtraDatatype != Progress && (mExtraDataFlags & CheckerboardBackground))
            {
                int checkerboardColor1 = palette().color(QPalette::Light).rgb();
                int checkerboardColor2 = palette().color(QPalette::Midlight).rgb();
                mBackgroundImage.setPixel(0, 0, checkerboardColor1);
                mBackgroundImage.setPixel(1, 1, checkerboardColor1);
                mBackgroundImage.setPixel(0, 1, checkerboardColor2);
                mBackgroundImage.setPixel(1, 0, checkerboardColor2);
                p.setBrushOrigin(extraDataRect.topLeft());
                checkerboardBrush = QBrush(mBackgroundImage);
                checkerboardBrush.setTransform(QTransform(6, 0, 0, 0, 6, 0, 0, 0, 1));
            }
            switch (mExtraDatatype)
            {
            case Progress:
                if (bflags & (QStyle::State_Sunken | QStyle::State_On))
                    p.setBrush(highlight.darker(110));
                else
                    p.setBrush(highlight);
                p.setClipRect(extraDataRect.adjusted(0, 0, -((1.0 - mProgress) * extraDataRect.width()), 0));
                p.drawRoundedRect(extraDataRect, 2.0, 2.0);
                break;
            case Color:
                if (mExtraDataFlags & CheckerboardBackground)
                {
                    p.setBrush(checkerboardBrush);
                    p.drawRoundedRect(extraDataRect, 2.0, 2.0);
                }
                if (!mColor.isValid())
                    break;
                p.setBrush(mColor);
                p.drawRoundedRect(extraDataRect, 2.0, 2.0);
                break;
            case GradientStops:
            {
                if (mExtraDataFlags & CheckerboardBackground)
                {
                    p.setBrush(checkerboardBrush);
                    p.drawRoundedRect(extraDataRect, 2.0, 2.0);
                }
                if (mGradientStops.size() < 2)
                    break;
                QLinearGradient grd = QLinearGradient(extraDataRect.topLeft(), extraDataRect.topRight());
                grd.setStops(mGradientStops);
                p.setBrush(grd);
                p.drawRoundedRect(extraDataRect, 2.0, 2.0);
                break;
            }
            case Image:
            {
                if (mExtraDataFlags & CheckerboardBackground)
                {
                    p.setBrush(checkerboardBrush);
                    p.drawRoundedRect(extraDataRect, 2.0, 2.0);
                }
                if (mImage.isNull())
                    break;
                int x, y, w, h;
                QBrush imageBrush(mImage);
                if (mExtraDataFlags & ImageStretched)
                {
                    int dx, dy;
                    dx = extraDataRect.width() - mImage.width();
                    dy = extraDataRect.height() - mImage.height();
                    if (mExtraDataFlags & ImageKeepAspectRatio)
                    {
                        if (dx >= 0 && dy >= 0 && (mExtraDataFlags & ImageStretchOnlyIfBiggerThanButton))
                        {
                            w = mImage.width();
                            h = mImage.height();
                        }
                        else
                        {
                            double resizeFactor;
                            QRect fittedRect = fitRectWithin(mImage.rect(), extraDataRect, &resizeFactor);
                            w = fittedRect.width();
                            h = fittedRect.height();
                            imageBrush.setTransform(QTransform(resizeFactor, 0, 0,
                                                               0, resizeFactor, 0,
                                                               0, 0, 1));
                        }
                    }
                    else
                    {
                        if (dx >= 0 && dy >= 0 && (mExtraDataFlags & ImageStretchOnlyIfBiggerThanButton))
                        {
                            w = mImage.width();
                            h = mImage.height();
                        }
                        else
                        {
                            p.drawImage(extraDataRect, mImage);
                            break;
                        }
                    }
                }
                else
                {
                    w = mImage.width();
                    h = mImage.height();
                }
                x = ((extraDataRect.width() - w) >> 1) + extraDataRect.x();
                y = ((extraDataRect.height() - h) >> 1) + extraDataRect.y();

                p.setBrushOrigin(x, y);
                p.setBrush(imageBrush);
                if (!(mExtraDataFlags & ImageTiled))
                    p.setClipRect(x, y, w, h);
                p.drawRoundedRect(extraDataRect, 2.0, 2.0);

                break;
            }
            default:
                break;
            }
        }
        p.setClipping(false);
    }

    if (flags & QStyle::State_HasFocus) {
        QStyleOptionFocusRect fr;
        fr.QStyleOption::operator=(toolbutton);
        fr.rect.adjust(3, 3, -3, -3);
        if (toolbutton.features & QStyleOptionToolButton::MenuButtonPopup)
            fr.rect.adjust(0, 0, -style()->pixelMetric(QStyle::PM_MenuButtonIndicator,
                                              &toolbutton, this), 0);
        p.drawPrimitive(QStyle::PE_FrameFocusRect, fr);
    }

    QStyleOptionToolButton label = toolbutton;
    label.state = bflags;
    int fw = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &toolbutton, this);
    label.rect = button.adjusted(fw, fw, -fw, -fw);
    if ((toolbutton.features & QStyleOptionToolButton::HasMenu) &&
        !(toolbutton.features & QStyleOptionToolButton::MenuButtonPopup))
        label.rect.adjust(0, 0, -style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &toolbutton, this), 0);

    if (bData && mExtraDatatype == Progress)
    {
        label.palette.setColor(QPalette::ButtonText, palette().color(QPalette::HighlightedText));
        p.setClipRect(extraDataRect.adjusted(0, 0, -((1.0 - mProgress) * extraDataRect.width()), 0));
        p.drawControl(QStyle::CE_ToolButtonLabel, label);
        label.palette.setColor(QPalette::ButtonText, palette().color(QPalette::ButtonText));
        p.setClipRect(extraDataRect.adjusted(mProgress * extraDataRect.width() + 1, 0, 0, 0));
        p.drawControl(QStyle::CE_ToolButtonLabel, label);
    }
    else
    {
        p.drawControl(QStyle::CE_ToolButtonLabel, label);
    }

    if (toolbutton.subControls & QStyle::SC_ToolButtonMenu) {
        tool.rect = this->rect();
        tool.state = mflags;
        p.setClipRect(menuarea);
        if (mflags & (QStyle::State_MouseOver | QStyle::State_Sunken | QStyle::State_On | QStyle::State_Raised))
        {
            p.drawPrimitive(QStyle::PE_PanelButtonTool, tool);
            if (!(mflags & QStyle::State_Sunken))
            {
                p.setPen(innerContrastLine);
                p.drawLine(menuarea.topLeft() + QPoint(0, 1), menuarea.bottomLeft() - QPoint(0, 1));
            }
        }
        tool.rect = menuarea.adjusted(3, 0, -4, 0);
        p.drawPrimitive(QStyle::PE_IndicatorArrowDown, tool);
    } else if (toolbutton.features & QStyleOptionToolButton::HasMenu) {
        tool.rect = this->rect().adjusted(this->width() - style()->pixelMetric(QStyle::PM_MenuButtonIndicator,
                                                                   &toolbutton, this) + 2, 0, -5, 0);
        p.drawPrimitive(QStyle::PE_IndicatorArrowDown, tool);
    }

    e->accept();
}

void ToolButtonEx::setPopupMode(ToolButtonPopupMode mode)
{
    if (popupMode() == mode)
        return;
    QToolButton::setPopupMode(mode);
    updateGeometry();
    if (isVisible())
    {
        update();
    }
}

QString ToolButtonEx::text() const
{
    return mRealText;
}

void ToolButtonEx::setText(const QString &text)
{
    mRealText = text;
    if (mMaximumTextWidth == 0 || mTextelideMode == Qt::ElideNone)
    {
        QToolButton::setText(text);
        return;
    }
    QFontMetrics fm = fontMetrics();
    QToolButton::setText(fontMetrics().elidedText(text, mTextelideMode, mMaximumTextWidth, Qt::TextShowMnemonic));
}

QSize ToolButtonEx::sizeHint() const
{
    ensurePolished();

    int w = 0, h = 0;
    QStyleOptionToolButton opt;
    initStyleOption(&opt);

    QFontMetrics fm = fontMetrics();
    if (opt.toolButtonStyle != Qt::ToolButtonTextOnly)
    {
        QSize icon = opt.iconSize;
        w = icon.width();
        h = icon.height();
    }

    if (opt.toolButtonStyle != Qt::ToolButtonIconOnly)
    {
        QSize textSize = fm.size(Qt::TextShowMnemonic, QToolButton::text());
        textSize.setWidth(textSize.width() + fm.width(QLatin1Char(' '))*2);
        if (opt.toolButtonStyle == Qt::ToolButtonTextUnderIcon)
        {
            h += 4 + textSize.height();
            if (textSize.width() > w)
                w = textSize.width();
        }
        else if (opt.toolButtonStyle == Qt::ToolButtonTextBesideIcon)
        {
            w += 4 + textSize.width();
            if (textSize.height() > h)
                h = textSize.height();
        }
        else
        { // TextOnly
            w = textSize.width();
            h = textSize.height();
        }
    }

    opt.rect.setSize(QSize(w, h)); // PM_MenuButtonIndicator depends on the height
    if (popupMode() == MenuButtonPopup || (opt.features & QStyleOptionToolButton::HasMenu))
        w += style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this);

    QSize tSizeHint = style()->sizeFromContents(QStyle::CT_ToolButton, &opt, QSize(w, h), this).
                  expandedTo(QApplication::globalStrut());

    return tSizeHint;
}

Qt::TextElideMode ToolButtonEx::textElideMode() const
{
    return mTextelideMode;
}

ToolButtonEx::ExtraDataType ToolButtonEx::extraDataType() const
{
    return mExtraDatatype;
}

ToolButtonEx::ExtraDataFlags ToolButtonEx::extraDataFlags() const
{
    return mExtraDataFlags;
}

double ToolButtonEx::progress() const
{
    return mProgress;
}

QColor ToolButtonEx::color() const
{
    return mColor;
}

QGradientStops ToolButtonEx::gradientStops() const
{
    return mGradientStops;
}

QImage ToolButtonEx::image() const
{
    return mImage;
}

bool ToolButtonEx::menuButtonDown() const
{
    return mMenuButtonDown;
}

void ToolButtonEx::initStyleOption(QStyleOptionToolButton *o) const
{
    QToolButton::initStyleOption(o);
}

int ToolButtonEx::maximumTextWidth() const
{
    return mMaximumTextWidth;
}

void ToolButtonEx::setMaximumTextWidth(int tw)
{
    if (tw < 0)
        return;
    mMaximumTextWidth = tw;
    setText(mRealText);
}

void ToolButtonEx::setTextElideMode(Qt::TextElideMode tem)
{
    if (tem < Qt::ElideLeft || tem > Qt::ElideNone)
        return;
    mTextelideMode = tem;
    setText(mRealText);
}

void ToolButtonEx::setExtraDataType(ToolButtonEx::ExtraDataType t)
{
    mExtraDatatype = t;
    update();
}

void ToolButtonEx::setExtraDataFlags(ExtraDataFlags flags)
{
    if (flags == mExtraDataFlags)
        return;
    mExtraDataFlags = flags;
    update();
}

void ToolButtonEx::setProgress(double p)
{
    if (qFuzzyCompare(p, mProgress))
        return;
    mProgress = qBound<double>(0.0, p, 1.0);
    if (mExtraDatatype == Progress)
        update();
}

void ToolButtonEx::setColor(const QColor &c)
{
    if (c == mColor)
        return;
    mColor = c;
    if (mExtraDatatype == Color)
        update();
}

void ToolButtonEx::setGradientStops(const QGradientStops &s)
{
    if (s == mGradientStops)
        return;
    mGradientStops = s;
    if (mExtraDatatype == GradientStops)
        update();
}

void ToolButtonEx::setImage(const QImage &i)
{
    if (i == mImage)
        return;
    mImage = i;
    if (mExtraDatatype == Image)
        update();
}

void ToolButtonEx::setMenuButtonDown(bool d)
{
    if (d == mMenuButtonDown)
        return;
    mMenuButtonDown = d;
    update();
}

}}
