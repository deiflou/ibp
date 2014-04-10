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

void FilterWidget::setColor(const QColor & v)
{
    if (ui->mButtonColor->color() == v)
        return;
    mEmitSignals = false;
    ui->mButtonColor->setColor(v);
    mEmitSignals = true;
    emit colorChanged(v);
}

void FilterWidget::setPosition(Filter::Position v)
{
    Filter::Position p = ui->mButtonPositionFront->isChecked() ? Filter::Front :
                         ui->mButtonPositionBehind->isChecked() ? Filter::Behind :
                         Filter::Inside;
    if (p == v)
        return;
    mEmitSignals = false;
    if (v == Filter::Front)
        ui->mButtonPositionFront->setChecked(true);
    else if (v == Filter::Behind)
        ui->mButtonPositionBehind->setChecked(true);
    else
        ui->mButtonPositionInside->setChecked(true);
    mEmitSignals = true;
    emit positionChanged(v);
}

void FilterWidget::setBlendMode(BlendMode v)
{
    if (ui->mComboBlendMode->blendMode() == v)
        return;
    mEmitSignals = false;
    ui->mComboBlendMode->setBlendMode(v);
    mEmitSignals = true;
    emit blendModeChanged(v);
}

void FilterWidget::setOpacity(int v)
{
    if (ui->mSpinOpacity->value() == v)
        return;
    mEmitSignals = false;
    ui->mSpinOpacity->setValue(v);
    mEmitSignals = true;
    emit opacityChanged(v);
}

void FilterWidget::on_mButtonColor_colorChanged(const QColor & c)
{
    if (mEmitSignals)
        emit colorChanged(c);
}

void FilterWidget::on_mButtonPositionFront_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit positionChanged(Filter::Front);
}

void FilterWidget::on_mButtonPositionBehind_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit positionChanged(Filter::Behind);
}

void FilterWidget::on_mButtonPositionInside_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit positionChanged(Filter::Inside);
}

void FilterWidget::on_mComboBlendMode_blendModeChanged(BlendMode m)
{
    if (mEmitSignals)
        emit blendModeChanged(m);
}

void FilterWidget::on_mSliderOpacity_valueChanged(int value)
{
    ui->mSpinOpacity->setValue(value);
    if (mEmitSignals)
        emit opacityChanged(value);
}

void FilterWidget::on_mSpinOpacity_valueChanged(int arg1)
{
    ui->mSliderOpacity->setValue(arg1);
    if (mEmitSignals)
        emit opacityChanged(arg1);
}
