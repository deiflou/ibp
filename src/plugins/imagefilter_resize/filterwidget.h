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

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QSlider>
#include <QSpinBox>

#include "filter.h"
#include "../widgets/anchorpositionbox.h"

namespace Ui {
class FilterWidget;
}

class FilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget *parent = 0);
    ~FilterWidget();

private:
    Ui::FilterWidget *ui;
    bool mEmitSignals;

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

private slots:
    void on_mSpinWidth_valueChanged(int arg1);
    void on_mSpinHeight_valueChanged(int arg1);
    void on_mComboWidthMode_currentIndexChanged(int index);
    void on_mComboHeightMode_currentIndexChanged(int index);
    void on_mButtonResizeModeAbsolute_toggled(bool c);
    void on_mButtonResizeModeRelative_toggled(bool c);
    void on_mAnchorPositionBox_anchorPositionChanged(ibp::misc::Alignment ap);
    void on_mButtonBackgroundColor_colorChanged(const QColor & c);

};

#endif // FILTERWIDGET_H
