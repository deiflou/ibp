#include <math.h>

#include "filterwidget.h"
#include "ui_filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true)
{
    ui->setupUi(this);

    ui->mButtonAffectedChannelsRed->hide();
    ui->mButtonAffectedChannelsGreen->hide();
    ui->mButtonAffectedChannelsBlue->hide();

    ui->mContainerRGB->hide();
    ui->mContainerRed->hide();
    ui->mContainerGreen->hide();
    ui->mContainerBlue->hide();
    ui->mContainerAlpha->hide();

    mButtonAffectedChannel[0] = ui->mButtonAffectedChannelsRGB;
    mButtonAffectedChannel[1] = ui->mButtonAffectedChannelsRed;
    mButtonAffectedChannel[2] = ui->mButtonAffectedChannelsGreen;
    mButtonAffectedChannel[3] = ui->mButtonAffectedChannelsBlue;
    mButtonAffectedChannel[4] = ui->mButtonAffectedChannelsAlpha;

    mSpinThreshold[0] = ui->mSpinRGB;
    mSpinThreshold[1] = ui->mSpinRed;
    mSpinThreshold[2] = ui->mSpinGreen;
    mSpinThreshold[3] = ui->mSpinBlue;
    mSpinThreshold[4] = ui->mSpinAlpha;
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setRGBMode(int m)
{
    if ((m == 0 && ui->mButtonRGBModeTight->isChecked()) ||
        (m == 1 && ui->mButtonRGBModeIndependent->isChecked()))
        return;

    mEmitSignals = false;
    if (m == 0)
        ui->mButtonRGBModeTight->setChecked(true);
    else
        ui->mButtonRGBModeIndependent->setChecked(true);
    mEmitSignals = true;

    emit rgbModeChanged(m);
}

void FilterWidget::setAffectedChannel(int c, bool a)
{
    if (mButtonAffectedChannel[c]->isChecked() == a)
        return;

    mEmitSignals = false;
    mButtonAffectedChannel[c]->setChecked(a);
    mEmitSignals = true;

    emit affectedChannelChanged(c, a);
}

void FilterWidget::setThreshold(int c, int t)
{
    if (mSpinThreshold[c]->value() == t)
        return;

    mEmitSignals = false;
    mSpinThreshold[c]->setValue(t);
    mEmitSignals = true;

    emit thresholdChanged(c, t);
}

void FilterWidget::on_mButtonRGBModeTight_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mButtonAffectedChannelsRGB->show();
    ui->mButtonAffectedChannelsRed->hide();
    ui->mButtonAffectedChannelsGreen->hide();
    ui->mButtonAffectedChannelsBlue->hide();
    ui->mContainerRGB->setVisible(ui->mButtonAffectedChannelsRGB->isChecked());
    ui->mContainerRed->hide();
    ui->mContainerGreen->hide();
    ui->mContainerBlue->hide();
    if (mEmitSignals)
        emit rgbModeChanged(0);
}

void FilterWidget::on_mButtonRGBModeIndependent_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mButtonAffectedChannelsRGB->hide();
    ui->mButtonAffectedChannelsRed->show();
    ui->mButtonAffectedChannelsGreen->show();
    ui->mButtonAffectedChannelsBlue->show();
    ui->mContainerRGB->hide();
    ui->mContainerRed->setVisible(ui->mButtonAffectedChannelsRed->isChecked());
    ui->mContainerGreen->setVisible(ui->mButtonAffectedChannelsGreen->isChecked());
    ui->mContainerBlue->setVisible(ui->mButtonAffectedChannelsBlue->isChecked());
    if (mEmitSignals)
        emit rgbModeChanged(1);
}

void FilterWidget::on_mButtonAffectedChannelsRGB_toggled(bool checked)
{
    ui->mContainerRGB->setVisible(checked && ui->mButtonRGBModeTight->isChecked());
    if (mEmitSignals)
        emit affectedChannelChanged(0, checked);
}

void FilterWidget::on_mButtonAffectedChannelsRed_toggled(bool checked)
{
    ui->mContainerRed->setVisible(checked && ui->mButtonRGBModeIndependent->isChecked());
    if (mEmitSignals)
        emit affectedChannelChanged(1, checked);
}

void FilterWidget::on_mButtonAffectedChannelsGreen_toggled(bool checked)
{
    ui->mContainerGreen->setVisible(checked && ui->mButtonRGBModeIndependent->isChecked());
    if (mEmitSignals)
        emit affectedChannelChanged(2, checked);
}

void FilterWidget::on_mButtonAffectedChannelsBlue_toggled(bool checked)
{
    ui->mContainerBlue->setVisible(checked && ui->mButtonRGBModeIndependent->isChecked());
    if (mEmitSignals)
        emit affectedChannelChanged(3, checked);
}

void FilterWidget::on_mButtonAffectedChannelsAlpha_toggled(bool checked)
{
    ui->mContainerAlpha->setVisible(checked);
    if (mEmitSignals)
        emit affectedChannelChanged(4, checked);
}

void FilterWidget::on_mSliderRGB_valueChanged(int value)
{
    ui->mSpinRGB->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(0, value);
}

void FilterWidget::on_mSpinRGB_valueChanged(int arg1)
{
    ui->mSliderRGB->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(0, arg1);
}

void FilterWidget::on_mSliderRed_valueChanged(int value)
{
    ui->mSpinRed->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(1, value);
}

void FilterWidget::on_mSpinRed_valueChanged(int arg1)
{
    ui->mSliderRed->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(1, arg1);
}

void FilterWidget::on_mSliderGreen_valueChanged(int value)
{
    ui->mSpinGreen->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(2, value);
}

void FilterWidget::on_mSpinGreen_valueChanged(int arg1)
{
    ui->mSliderGreen->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(2, arg1);
}

void FilterWidget::on_mSliderBlue_valueChanged(int value)
{
    ui->mSpinBlue->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(3, value);
}

void FilterWidget::on_mSpinBlue_valueChanged(int arg1)
{
    ui->mSliderBlue->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(3, arg1);
}

void FilterWidget::on_mSliderAlpha_valueChanged(int value)
{
    ui->mSpinAlpha->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(4, value);
}

void FilterWidget::on_mSpinAlpha_valueChanged(int arg1)
{
    ui->mSliderAlpha->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(4, arg1);
}
