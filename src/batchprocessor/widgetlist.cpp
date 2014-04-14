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

#include <QCheckBox>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVariant>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QDebug>

#include "widgetlist.h"
#include "../misc/util.h"

using namespace anitools::misc;

WidgetList::WidgetList(QWidget *parent) :
    QWidget(parent),
    mLayout(0),
    mAnchorPoint(),
    mIsHolded(false),
    mIsMooving(false),
    mIsEmpty(true),
    mDraggedIndex(-1),
    mIsAnimating(0),
    mAnimate(true),
    mWidgetPlaceholder(0),
    mDragTimer(0),
    mScrolligNeeded(0),
    mWorkingWidget(0),
    mLabelEmpty(0),
    mAnimation(0)
{
    mLayout = new QVBoxLayout(this);
    mLayout->addStretch(1);
    mLayout->setContentsMargins(0, 0, 0, 0);
    mLayout->setSpacing(0);

    mWidgetPlaceholder = new QWidget(this);
    mWidgetPlaceholder->hide();

    mLabelEmpty = new QLabel(this);
    mLabelEmpty->setText(tr("No effects in the list"));
    mLabelEmpty->setMinimumHeight(30);
    mLabelEmpty->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    mLayout->insertWidget(0, mLabelEmpty);

    mDragTimer = new QTimer(this);
    connect(mDragTimer, SIGNAL(timeout()), this, SLOT(On_mDragTimer_timeout()));

    mAnimation = new QParallelAnimationGroup(this);
    connect(mAnimation, SIGNAL(finished()), this, SLOT(On_mAnimation_finished()));
}

QWidget *WidgetList::at(int index) const
{
    if (mIsEmpty || index < 0 || index >= mLayout->count() - 1)
        return 0;
    QWidget * p = mLayout->itemAt(index)->widget()->layout()->itemAt(0)->widget()->layout()->itemAt(1)->widget();
    QWidget * w = p->layout()->itemAt(0)->widget();
    return w;
}

void WidgetList::append(QWidget *w, const QString &title)
{
    insert(count(), w, title);
}

void WidgetList::insert(int index, QWidget *w, const QString &title)
{
    if (mIsEmpty)
    {
        if (index != 0)
            return;
        mLayout->removeWidget(mLabelEmpty);
        mLabelEmpty->hide();
        mIsEmpty = false;
    }
    else
    {
        if (index < 0 || index > mLayout->count() - 1)
            return;
    }

    QString filterName;
    if (!title.isEmpty())
        filterName = title;
    else
        filterName = "<i>[" + tr("Unnamed Filter") + "]</i>";

    bool mustCollapse = false;
    if (!w)
    {
        QLabel * l = new QLabel(tr("No interface available."));
        l->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
        l->setStyleSheet("QLabel{color:palette(light);font-style:italic;}");
        mustCollapse = true;
        w = l;
    }

    QWidget * w1 = createWidgetContainer(filterName);
    QWidget * w4 = w1->layout()->itemAt(0)->widget()->layout()->itemAt(1)->widget();
    QVBoxLayout * w4Layout = (QVBoxLayout *)(w4->layout());

    w->setParent(w4);
    w4Layout->addWidget(w);

    w1->setParent(this);
    mLayout->insertWidget(index, w1);

    bool ani = mAnimate;
    mAnimate = false;
    if (mustCollapse)
        setWidgetExpanded(index, false);
    mAnimate = ani;

    adjustSize();

    emit widgetInserted(index);
}

int WidgetList::count() const
{
    if (mIsEmpty)
        return 0;
    return mLayout->count() - 1;
}

bool WidgetList::isEmpty() const
{
    return mIsEmpty;
}

void WidgetList::move(int from, int to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);

    if (mIsEmpty)
        return;
}

void WidgetList::removeAt(int i)
{
    while (mIsAnimating != 0);

    if (mIsEmpty)
        return;
    else
        if (i < 0 || i >= mLayout->count() - 1)
            return;

    if (mAnimate)
    {
        mIsAnimating = 2;

        mWorkingWidget = mLayout->itemAt(i)->widget();

        mAnimation->clear();
        QPropertyAnimation * ani1 = new QPropertyAnimation(this);
        ani1->setTargetObject(mWorkingWidget);
        ani1->setPropertyName("minimumHeight");
        ani1->setStartValue(mWorkingWidget->height());
        ani1->setEndValue(0);
        ani1->setEasingCurve(QEasingCurve::OutCubic);
        ani1->setDuration(animationsDuration);
        mAnimation->addAnimation(ani1);
        QPropertyAnimation * ani2 = new QPropertyAnimation(this);
        ani2->setTargetObject(mWorkingWidget);
        ani2->setPropertyName("maximumHeight");
        ani2->setStartValue(mWorkingWidget->height());
        ani2->setEndValue(0);
        ani2->setEasingCurve(QEasingCurve::OutCubic);
        ani2->setDuration(animationsDuration);
        mAnimation->addAnimation(ani2);

        mAnimation->start();
    }
    else
    {
        QWidget * w = mLayout->itemAt(i)->widget();
        mLayout->removeWidget(w);
        delete w;

        if (mLayout->count() == 1)
        {
            mLayout->insertWidget(0, mLabelEmpty);
            mLabelEmpty->show();
            mIsEmpty = true;
        }

        emit widgetRemoved(i);
    }
}

void WidgetList::clear()
{
    mAnimate = false;
    while(!mIsEmpty)
        removeAt(count() - 1);
    mAnimate = true;
}

void WidgetList::moveEvent(QMoveEvent *e)
{
    if (!mIsMooving)
        return;

    QMouseEvent me(QEvent::MouseMove, mAnchorPoint + (e->oldPos() - e->pos()),
                   Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    eventFilter(mWorkingWidget, &me);
    e->accept();
}

bool WidgetList::eventFilter(QObject *o, QEvent *e)
{
    if (mIsAnimating > 0)
        return QWidget::eventFilter(o, e);

    QWidget * w = qobject_cast<QWidget *>(o);
    QWidget * w2 = qobject_cast<QWidget *>(w->parent()->parent());
    QWidget * w1 = qobject_cast<QWidget *>(w2->parent());
    QWidget * tp = qobject_cast<QWidget *>(parent());
    QMouseEvent * me;
    int i;

    if (e->type() == QEvent::MouseButtonPress)
    {
        me = (QMouseEvent *)e;

        if (me->button() != Qt::LeftButton || mIsMooving || mIsHolded)
            return QWidget::eventFilter(o, e);

        mAnchorPoint = me->pos();
        mWorkingWidget = w;
        mIsHolded = true;

        return true;
    }

    if (e->type() == QEvent::MouseButtonRelease)
    {
        me = (QMouseEvent *)e;

        if (me->button() != Qt::LeftButton || !mIsMooving || !mIsHolded)
            return QWidget::eventFilter(o, e);

        mDragTimer->stop();

        if (mAnimate)
        {
            mIsAnimating = 1;
            w1->setGraphicsEffect(0);
            w2->setStyleSheet("");
            w2->setGraphicsEffect(0);
            mAnimation->clear();
            QPropertyAnimation * ani = new QPropertyAnimation(this);
            ani->setTargetObject(w1);
            ani->setPropertyName("pos");
            ani->setStartValue(w1->pos());
            ani->setEndValue(mWidgetPlaceholder->pos());
            ani->setEasingCurve(QEasingCurve::OutCubic);
            ani->setDuration(animationsDuration);
            mAnimation->addAnimation(ani);
            mAnimation->start();
        }
        else
        {
            w1->setGraphicsEffect(0);
            w2->setStyleSheet("");
            w2->setGraphicsEffect(0);
            mLayout->insertWidget(mLayout->indexOf(mWidgetPlaceholder), w1);
            mLayout->removeWidget(mWidgetPlaceholder);
            mWidgetPlaceholder->hide();

            mIsHolded = false;
            mIsMooving = false;

            emit widgetMoved(mDraggedIndex, mLayout->indexOf(w1));
        }

        return true;
    }

    if (e->type() == QEvent::MouseMove)
    {
        me = (QMouseEvent *)e;

        if (!(me->buttons() & Qt::LeftButton) || !mIsHolded)
            return QWidget::eventFilter(o, e);

        QPoint pt;
        QRect r = QRect(0, qMax<int>(-y(), 0), width(), qMin<int>(height(), tp->height()));
        if (mLayout->itemAt(mLayout->count() - 1)->geometry().height() > 0)
            r.setBottom(r.bottom() -
                        mLayout->itemAt(mLayout->count() - 1)->geometry().height() -
                        mLayout->contentsMargins().bottom());
        if (r.top() == 0)
            r.setTop(mLayout->contentsMargins().top());
        if (r.bottom() == rect().bottom())
            r.setBottom(rect().bottom() - mLayout->contentsMargins().bottom());

        if (mIsMooving)
        {
            pt = w1->pos() + me->pos() - mAnchorPoint;
            if (pt.y() < r.top())
                pt.setY(r.top());
            if (pt.y() > r.bottom() + 1 - w1->sizeHint().height())
                pt.setY(r.bottom() + 1 - w1->sizeHint().height());
            pt.setX(mLayout->contentsMargins().left());
            w1->move(pt);

            mLayout->removeWidget(mWidgetPlaceholder);
            int y = pt.y();
            int y2 = 0;
            for (i = 0; i < mLayout->count() - 1; i++)
            {
                QWidget * iw = mLayout->itemAt(i)->widget();
                if (y < y2 + iw->height())
                {
                    if (y < y2 + (iw->height() >> 1))
                        break;
                    else
                    {
                        i++;
                        break;
                    }
                }
                y2 += iw->height();
            }
            mLayout->insertWidget(i, mWidgetPlaceholder);

            if (pt.y() < r.top() + scrollingNeededDistance)
            {
                mScrolligNeeded = -scrollingNeededDistance + (pt.y() - r.top());
                if (!mDragTimer->isActive())
                    mDragTimer->start(16);
            }
            else if (pt.y() > r.bottom() + 1 - w1->sizeHint().height() - scrollingNeededDistance)
            {
                mScrolligNeeded = pt.y() - (r.bottom() + 1 - w1->sizeHint().height() - scrollingNeededDistance);
                if (!mDragTimer->isActive())
                    mDragTimer->start(16);
            }
            else
            {
                mDragTimer->stop();
            }
        }
        else
        {
            if (euclideanDistance(mAnchorPoint, me->pos()) < QApplication::startDragDistance())
                return true;

            i = mLayout->indexOf(w1);
            mWidgetPlaceholder->setFixedHeight(w1->sizeHint().height());
            mLayout->insertWidget(i, mWidgetPlaceholder);
            mLayout->removeWidget(w1);
            double so = 10.0 / w1->height();
            QString c = palette().color(QPalette::Mid).darker(120).name();
            mWidgetPlaceholder->setStyleSheet("QWidget{"
                                              "background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
                                              "stop: 0 " + c + ", "
                                              "stop: " + QString::number(so) + " palette(mid), "
                                              "stop: " + QString::number(1.0 - so) + " palette(mid), "
                                              "stop: 1 " + c + ");"
                                              "margin: 0 0; "
                                              "border-top: 1px solid palette(midlight); "
                                              "border-bottom: 1px solid palette(midlight);"
                                              "}");
            mWidgetPlaceholder->show();
            w1->raise();

            QGraphicsOpacityEffect * ge1 = new QGraphicsOpacityEffect(this);
            ge1->setOpacity(0.8);
            w2->setGraphicsEffect(ge1);
            w2->setStyleSheet(".cViewEditWidgetListItemContainer{background-color:palette(window);"
                              "border-radius:2; border:1px solid palette(mid);}");

            QGraphicsDropShadowEffect * ge2 = new QGraphicsDropShadowEffect(this);
            ge2->setColor(QColor(0, 0, 0));
            ge2->setBlurRadius(50);
            ge2->setOffset(0, 1);
            w1->setGraphicsEffect(ge2);

            mIsMooving = true;
            mDraggedIndex = i;

            return eventFilter(o, e);
        }

        return true;
    }

    return QWidget::eventFilter(o, e);
}

void WidgetList::On_WidgetContainerExpandCheck_toggled(bool c)
{
    while (mIsAnimating != 0 && mIsAnimating != 3);

    QWidget * w2 = qobject_cast<QWidget *>(sender()->parent()->parent());
    QWidget * w1 = qobject_cast<QWidget *>(w2->parent());
    QWidget * w4 = w2->layout()->itemAt(1)->widget();
    int i = mLayout->indexOf(w1);

    if (mAnimate)
    {
        mIsAnimating = 3;

        mWorkingWidget = w1;

        int startValue, endValue;
        startValue = w4->height();
        if (c)
            endValue = w4->minimumSizeHint().height();
        else
            endValue = 0;

        mAnimation->clear();
        QPropertyAnimation * ani1 = new QPropertyAnimation(this);
        ani1->setTargetObject(w4);
        ani1->setPropertyName("minimumHeight");
        ani1->setStartValue(startValue);
        ani1->setEndValue(endValue);
        ani1->setEasingCurve(QEasingCurve::OutCubic);
        ani1->setDuration(animationsDuration);
        mAnimation->addAnimation(ani1);
        QPropertyAnimation * ani2 = new QPropertyAnimation(this);
        ani2->setTargetObject(w4);
        ani2->setPropertyName("maximumHeight");
        ani2->setStartValue(startValue);
        ani2->setEndValue(endValue);
        ani2->setEasingCurve(QEasingCurve::OutCubic);
        ani2->setDuration(animationsDuration);
        mAnimation->addAnimation(ani2);

        mAnimation->start();
    }
    else
    {
        if (c)
        {
            w4->setMinimumHeight(0);
            w4->setMaximumHeight(QWIDGETSIZE_MAX);
        }
        else
            w4->setFixedHeight(0);
    }

    emit widgetExpandStatusChanged(i, c);
}

void WidgetList::On_WidgetContainerBypassCheck_toggled(bool c)
{
    QWidget * w1 = qobject_cast<QWidget *>(sender()->parent()->parent()->parent());
    int i = mLayout->indexOf(w1);
    emit widgetBypassStatusChanged(i, c);
}

void WidgetList::On_WidgetContainerCloseButton_clicked()
{
    if (mIsAnimating != 0)
        return;

    QWidget * w1 = qobject_cast<QWidget *>(sender()->parent()->parent()->parent());
    int i = mLayout->indexOf(w1);
    emit widgetAboutToClose(i);
    removeAt(i);
}

void WidgetList::On_mDragTimer_timeout()
{
    emit scrolligNeeded(mScrolligNeeded);
}

void WidgetList::On_mAnimation_finished()
{
    int i;
    QWidget * w1, * w2, * w3, * w4;
    QCheckBox * cb;
    if (mIsAnimating == 1)
    {
        w2 = qobject_cast<QWidget *>(mWorkingWidget->parent()->parent());
        w1 = qobject_cast<QWidget *>(w2->parent());
        mLayout->insertWidget(mLayout->indexOf(mWidgetPlaceholder), w1);
        mLayout->removeWidget(mWidgetPlaceholder);
        mWidgetPlaceholder->hide();
//        w1->setGraphicsEffect(0);
//        w2->setStyleSheet("");
//        w2->setGraphicsEffect(0);


        mIsHolded = false;
        mIsMooving = false;
        mIsAnimating = 0;

        emit widgetMoved(mDraggedIndex, mLayout->indexOf(w1));
        return;
    }

    if (mIsAnimating == 2)
    {
        i = mLayout->indexOf(mWorkingWidget);
        mLayout->removeWidget(mWorkingWidget);
        delete mWorkingWidget;

        mIsAnimating = 0;

        if (mLayout->count() == 1)
        {
            mLayout->insertWidget(0, mLabelEmpty);
            mLabelEmpty->show();
            mIsEmpty = true;
        }

        emit widgetRemoved(i);
        return;
    }

    if (mIsAnimating == 3)
    {
        w1 = mWorkingWidget;
        w3 = w1->layout()->itemAt(0)->widget()->layout()->itemAt(0)->widget();
        w4 = w1->layout()->itemAt(0)->widget()->layout()->itemAt(1)->widget();
        cb = qobject_cast<QCheckBox *>(w3->layout()->itemAt(0)->widget());
        if (cb->isChecked())
        {
            w4->setMinimumHeight(0);
            w4->setMaximumHeight(QWIDGETSIZE_MAX);
        }
        mIsAnimating = 0;
        return;
    }
}

void WidgetList::On_mAnimation_valueChanged(const QVariant &v)
{
    Q_UNUSED(v);

    if (mIsAnimating == 3)
    {
        adjustSize();
        return;
    }
}

QWidget *WidgetList::createWidgetContainer(const QString &title)
{
    QWidget * w1 = new QWidget(this);
    QWidget * w2 = new QWidget(w1);
    QWidget * w3 = new QWidget(w2);
    QWidget * w4 = new QWidget(w2);
    QVBoxLayout * w1Layout = new QVBoxLayout();
    QVBoxLayout * w2Layout = new QVBoxLayout();
    QHBoxLayout * w3Layout = new QHBoxLayout();
    QVBoxLayout * w4Layout = new QVBoxLayout();
    QCheckBox * expandCheckBox = new QCheckBox(w3);
    QCheckBox * bypassCheckBox = new QCheckBox(w3);
    QLabel * titleLabel = new QLabel(title, w3);
    QToolButton * closeButton = new QToolButton(w3);

    w1Layout->setContentsMargins(0, 0, 0, 0);
    w2Layout->setContentsMargins(10, 5, 10, 5);
    w2Layout->setSpacing(0);
    w3Layout->setContentsMargins(0, 0, 0, 0);
    w3Layout->setSpacing(5);
    w4Layout->setContentsMargins(10, 10, 10, 10);

    w2->setAutoFillBackground(true);
    w2->setProperty("class", "cViewEditWidgetListItemContainer");
    w3->setProperty("class", "cViewEditWidgetListItemCaptionContainer");
    w4->setProperty("class", "cViewEditWidgetListItemWidgetContainer");
    expandCheckBox->setChecked(true);
    expandCheckBox->setToolTip(tr("Expand/collapse filter controls."));
    expandCheckBox->setProperty("class", "cViewEditWidgetListItemCheckExpand");
    bypassCheckBox->setProperty("class", "cViewEditWidgetListItemCheckBypass");
    bypassCheckBox->setToolTip(tr("Bypass this filter."));
    titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    closeButton->setIcon(QIcon(":/anitools/icons/close"));
    closeButton->setToolTip(tr("Remove this filter."));
    closeButton->setAutoRaise(true);

    w1->setLayout(w1Layout);
    w2->setLayout(w2Layout);
    w3->setLayout(w3Layout);
    w4->setLayout(w4Layout);

    w1Layout->addWidget(w2);

    w2Layout->addWidget(w3);
    w2Layout->addWidget(w4);
    w2Layout->addStretch(1);

    w3Layout->addWidget(expandCheckBox);
    w3Layout->addWidget(bypassCheckBox);
    w3Layout->addWidget(titleLabel, 1);
    w3Layout->addWidget(closeButton);
    w3->setFixedHeight(w3->minimumSizeHint().height());

    connect(expandCheckBox, SIGNAL(toggled(bool)), this, SLOT(On_WidgetContainerExpandCheck_toggled(bool)));
    connect(bypassCheckBox, SIGNAL(toggled(bool)), this, SLOT(On_WidgetContainerBypassCheck_toggled(bool)));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(On_WidgetContainerCloseButton_clicked()));

    titleLabel->installEventFilter(this);

    return w1;
}

void WidgetList::setWidgetExpanded(int i, bool e)
{
    if (mIsEmpty || i < 0 || i >= mLayout->count() - 1)
        return;

    QWidget * w2 = mLayout->itemAt(i)->widget()->layout()->itemAt(0)->widget();
    QWidget * w3 = w2->layout()->itemAt(0)->widget();
    QCheckBox * cb = qobject_cast<QCheckBox *>(w3->layout()->itemAt(0)->widget());
    if (!cb)
        return;

    cb->setChecked(e);
}

void WidgetList::setWidgetBypass(int i, bool b)
{
    if (mIsEmpty || i < 0 || i >= mLayout->count() - 1)
        return;

    QWidget * w3 = mLayout->itemAt(i)->widget()->layout()->itemAt(0)->widget()->layout()->itemAt(0)->widget();
    QCheckBox * cb = qobject_cast<QCheckBox *>(w3->layout()->itemAt(1)->widget());
    if (!cb)
        return;
    cb->setChecked(b);
}

void WidgetList::setAnimate(bool a)
{
    mAnimate = a;
}
