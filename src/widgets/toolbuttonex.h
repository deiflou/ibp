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

#ifndef ANITOOLS_WIDGETS_TOOLBUTTONEX_H
#define ANITOOLS_WIDGETS_TOOLBUTTONEX_H

#include <QToolButton>

namespace anitools {
namespace widgets {

class ToolButtonEx : public QToolButton
{
    Q_OBJECT
public:
    enum ExtraDataType
    {
        None,
        Progress,
        Color,
        GradientStops,
        Image
    };
    enum ExtraDataFlag
    {
        CheckerboardBackground = 1,
        ImageTiled = 2,
        ImageStretched = 4,
        ImageKeepAspectRatio = 8,
        ImageStretchOnlyIfBiggerThanButton = 16
    };
    Q_DECLARE_FLAGS(ExtraDataFlags, ExtraDataFlag)

    explicit ToolButtonEx(QWidget *parent = 0);

    void setPopupMode(ToolButtonPopupMode mode);
    QString	text() const;
    int maximumTextWidth() const;
    QSize sizeHint() const;
    Qt::TextElideMode textElideMode() const;
    ExtraDataType extraDataType() const;
    ExtraDataFlags extraDataFlags() const;
    double progress() const;
    QColor color() const;
    QGradientStops gradientStops() const;
    QImage image() const;
    bool menuButtonDown() const;

    void initStyleOption(QStyleOptionToolButton * o) const;

public slots:
    void setText(const QString & text);
    void setMaximumTextWidth(int tw);
    void setTextElideMode(Qt::TextElideMode tem);
    void setExtraDataType(ExtraDataType t);
    void setExtraDataFlags(ExtraDataFlags flags);
    void setProgress(double p);
    void setColor(const QColor & c);
    void setGradientStops(const QGradientStops & s);
    void setImage(const QImage & i);
    void setMenuButtonDown(bool d);

protected:
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent * e);

signals:
    void menuButtonPressed();

private:
    int mMaximumTextWidth;
    QString mRealText;
    Qt::TextElideMode mTextelideMode;

    ExtraDataType mExtraDatatype;
    ExtraDataFlags mExtraDataFlags;

    double mProgress;
    QColor mColor;
    QGradientStops mGradientStops;
    QImage mImage;

    bool mMenuButtonDown;

    QImage mBackgroundImage;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ToolButtonEx::ExtraDataFlags)

}}

#endif // ANITOOLS_WIDGETS_TOOLBUTTONEX_H
