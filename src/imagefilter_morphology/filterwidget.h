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
    void morphologyOpChanged(Filter::MorphologyOp mop);
    void kernelShapeChanged(Filter::KernelShape shape);
    void hRadiusChanged(int r);
    void vRadiusChanged(int r);
    void lockRadiusChanged(bool l);

public slots:
    void setMorphologyOp(Filter::MorphologyOp mop);
    void setKernelShape(Filter::KernelShape shape);
    void setHRadius(int r);
    void setVRadius(int r);
    void setLockRadius(bool l);

private slots:
    void on_mComboMorphologyOp_currentIndexChanged(int index);
    void on_mComboKernelShape_currentIndexChanged(int index);
    void on_mSliderHRadius_valueChanged(int value);
    void on_mSpinHRadius_valueChanged(int arg1);
    void on_mSliderVRadius_valueChanged(int value);
    void on_mSpinVRadius_valueChanged(int arg1);
    void on_mButtonLockRadius_toggled(bool checked);
};

#endif // FILTERWIDGET_H
