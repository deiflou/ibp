/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
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

#ifndef ANITOOLS_WIDGETS_WIDGETLIST_H
#define ANITOOLS_WIDGETS_WIDGETLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QParallelAnimationGroup>

namespace anitools {
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

#endif // ANITOOLS_WIDGETS_WIDGETLIST_H
