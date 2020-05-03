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

#ifndef IBP_WIDGETS_WIDGETLIST_H
#define IBP_WIDGETS_WIDGETLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QParallelAnimationGroup>

namespace ibp {
namespace widgets {

class WidgetList : public QWidget
{
    Q_OBJECT
public:
    static const int scrollingNeededDistance = 50;
    static const int animationsDuration = 200;

    explicit WidgetList(QWidget *parent = 0);

    QWidget * at(int index) const;
    void append(QWidget * w);
    void insert(int index, QWidget * w);
    int count() const;
    bool isEmpty() const;
    void move(int from, int to);
    void removeAt(int i);
    void clear();

    bool widgetBypass(int i);
    bool widgetExpanded(int i);

protected:
    void moveEvent(QMoveEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

private:
    QVBoxLayout * mLayout;

    QPoint mAnchorPoint;
    bool mIsHolded, mIsMooving, mIsEmpty;
    int mDraggedIndex;
    int mIsAnimating;
    bool mAnimate;

    QWidget * mWidgetPlaceholder;
    QTimer * mDragTimer;
    int mScrolligNeeded;
    QWidget * mWorkingWidget;
    QLabel * mLabelEmpty;

    QParallelAnimationGroup * mAnimation;

    QMargins mItemMargins, mItemWidgetMargins;
    bool mIsPlaceholderVisible, mIsItemContentsFrameVisible, mIsItemShadowVisible;

    QWidget * createWidgetContainer();

signals:
    void widgetInserted(int i);
    void widgetMoved(int from, int to);
    void widgetRemoved(int i);
    void scrolligNeeded(int dir);

    void widgetExpandStatusChanged(int i, bool c);
    void widgetBypassStatusChanged(int i, bool c);
    void widgetAboutToClose(int i);

public slots:
    void setWidgetExpanded(int i, bool e);
    void setWidgetBypass(int i, bool b);
    void setAnimate(bool a);
    void setTitle(int i, const QString & title, const QString & tooltip = QString());
    void setItemMargins(const QMargins & margins);
    void setItemWidgetMargins(const QMargins & margins);
    void setEmptyMessage(const QString & text);
    void setPlaceholderVisible(bool v);
    void setItemContentsFrameVisible(bool v);
    void setItemShadowVisible(bool v);

private slots:
    void On_WidgetContainerExpandCheck_toggled(bool c);
    void On_WidgetContainerBypassCheck_toggled(bool c);
    void On_WidgetContainerCloseButton_clicked();

    void On_mDragTimer_timeout();
    void On_mAnimation_finished();
    void On_mAnimation_valueChanged(const QVariant & v);
};

}}

#endif // IBP_WIDGETS_WIDGETLIST_H
