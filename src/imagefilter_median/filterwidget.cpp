#include <math.h>

#include "filterwidget.h"
#include "ui_filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true)
{
    ui->setupUi(this);
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setRadius(int s)
{
    if (ui->mSpinRadius->value() == s)
        return;
    mEmitSignals = false;
    ui->mSpinRadius->setValue(s);
    mEmitSignals = true;
    emit radiusChanged(s);
}

void FilterWidget::on_mSliderRadius_valueChanged(int value)
{
    ui->mSpinRadius->setValue(value);
    if (mEmitSignals)
        emit radiusChanged(value);
}

void FilterWidget::on_mSpinRadius_valueChanged(int arg1)
{
    ui->mSliderRadius->setValue(arg1);
    if (mEmitSignals)
        emit radiusChanged(arg1);
}
