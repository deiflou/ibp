#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

#include "filter.h"

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
    void radiusChanged(int s);

public slots:
    void setRadius(int s);

private slots:
    void on_mSliderRadius_valueChanged(int value);
    void on_mSpinRadius_valueChanged(int arg1);
};

#endif // FILTERWIDGET_H
