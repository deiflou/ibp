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

#ifndef IBP_WIDGETS_TOOLBUTTONEX_H
#define IBP_WIDGETS_TOOLBUTTONEX_H

#include <QToolButton>

namespace ibp {
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

#endif // IBP_WIDGETS_TOOLBUTTONEX_H
