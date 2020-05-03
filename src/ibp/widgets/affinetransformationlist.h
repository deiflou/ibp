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

#ifndef IBP_WIDGETS_AFFINETRANSFORMATIONLIST_H
#define IBP_WIDGETS_AFFINETRANSFORMATIONLIST_H

#include <QWidget>

#include "../imgproc/types.h"

namespace ibp {
namespace widgets {

using namespace ibp::imgproc;

namespace Ui {
class AffineTransformationList;
}

class AffineTransformationList : public QWidget
{
    Q_OBJECT

public:
    explicit AffineTransformationList(QWidget *parent = 0);
    ~AffineTransformationList();

    QList<AffineTransformation> transformations() const;
    QList<bool> bypasses() const;

private:
    Ui::AffineTransformationList *ui;

    QList<AffineTransformation> mTransformations;
    QList<bool> mBypasses;

    void appendTransformation(const AffineTransformation & t);

signals:
    void transformationsChanged();

public slots:
    void setTransformations(const QList<AffineTransformation> & t);
    void setBypasses(const QList<bool> &b);

private slots:
    void On_mButtonAppend_menuActionClicked();
    void On_valueChanged(double v);

    void on_mWidgetList_widgetInserted(int i);
    void on_mWidgetList_widgetMoved(int from, int to);
    void on_mWidgetList_widgetRemoved(int i);
    void on_mWidgetList_widgetBypassStatusChanged(int i, bool c);
};

}}

#endif // IBP_WIDGETS_AFFINETRANSFORMATIONLIST_H
