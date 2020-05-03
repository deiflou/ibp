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

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>

#include <imgproc/imagefilter.h>
#include <misc/util.h>

using namespace ibp::imgproc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    enum SizeMode
    {
        Percent,
        Pixels,
        KeepAspectRatio
    };
    enum ResizeMode
    {
        Absolute,
        Relative
    };

    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    int mWidth, mHeight;
    SizeMode mWidthMode, mHeightMode;
    ResizeMode mResizeMode;
    ibp::misc::Alignment mAnchorPosition;
    QColor mBackgroundColor;

signals:
    void widthChanged(int w);
    void heightChanged(int h);
    void widthModeChanged(Filter::SizeMode wm);
    void heightModeChanged(Filter::SizeMode hm);
    void resizeModeChanged(Filter::ResizeMode rm);
    void anchorPositionChanged(ibp::misc::Alignment ap);
    void backgroundColorChanged(const QColor & c);

public slots:
    void setWidth(int w);
    void setHeight(int h);
    void setWidthMode(Filter::SizeMode wm);
    void setHeightMode(Filter::SizeMode hm);
    void setResizeMode(Filter::ResizeMode rm);
    void setAnchorPosition(ibp::misc::Alignment ap);
    void setBackgroundColor(const QColor & c);
};

#endif // FILTER_H
