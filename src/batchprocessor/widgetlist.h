#ifndef WIDGETLIST_H
#define WIDGETLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QParallelAnimationGroup>

class WidgetList : public QWidget
{
    Q_OBJECT
public:
    static const int scrollingNeededDistance = 50;
    static const int animationsDuration = 200;

    explicit WidgetList(QWidget *parent = 0);

    QWidget * at(int index) const;
    void append(QWidget * w, const QString & title = QString());
    void insert(int index, QWidget * w, const QString & title = QString());
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

    QWidget * createWidgetContainer(const QString & title = QString());

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

private slots:
    void On_WidgetContainerExpandCheck_toggled(bool c);
    void On_WidgetContainerBypassCheck_toggled(bool c);
    void On_WidgetContainerCloseButton_clicked();

    void On_mDragTimer_timeout();
    void On_mAnimation_finished();
    void On_mAnimation_valueChanged(const QVariant & v);
};

#endif // WIDGETLIST_H
