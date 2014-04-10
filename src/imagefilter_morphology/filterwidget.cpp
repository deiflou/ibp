#include "filterwidget.h"
#include "ui_filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true)
{
    ui->setupUi(this);

    ui->mComboMorphologyOp->addItems(QStringList() <<
                                     tr("Dilation") <<
                                     tr("Erosion") <<
                                     tr("Closing") <<
                                     tr("Opening"));
    ui->mComboMorphologyOp->setCurrentIndex(0);

    ui->mComboKernelShape->addItems(QStringList() <<
                                    tr("Ellipse") <<
                                    tr("Rectangle") <<
                                    tr("Diamond") <<
                                    tr("Octagon") <<
                                    tr("Plus") <<
                                    tr("Cross") <<
                                    tr("Ring"));
    ui->mComboKernelShape->setCurrentIndex(0);

    ui->mButtonLockRadius->setIcon(QIcon(":/anitools/icons/lock"));
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setMorphologyOp(Filter::MorphologyOp mop)
{
    if (mop == (Filter::MorphologyOp)ui->mComboMorphologyOp->currentIndex())
        return;
    ui->mComboMorphologyOp->setCurrentIndex(mop);
}

void FilterWidget::setKernelShape(Filter::KernelShape shape)
{
    if (shape == (Filter::KernelShape)ui->mComboKernelShape->currentIndex())
        return;
    ui->mComboKernelShape->setCurrentIndex(shape);
}

void FilterWidget::setHRadius(int r)
{
    if (ui->mSpinHRadius->value() == r)
        return;
    mEmitSignals = false;
    ui->mSpinHRadius->setValue(r);
    mEmitSignals = true;
    emit hRadiusChanged(r);
}

void FilterWidget::setVRadius(int r)
{
    if (ui->mSpinVRadius->value() == r)
        return;
    mEmitSignals = false;
    ui->mSpinVRadius->setValue(r);
    mEmitSignals = true;
    emit vRadiusChanged(r);
}

void FilterWidget::setLockRadius(bool l)
{
    if (ui->mButtonLockRadius->isChecked() == l)
        return;
    mEmitSignals = false;
    ui->mButtonLockRadius->setChecked(l);
    mEmitSignals = true;
    emit lockRadiusChanged(l);
}

void FilterWidget::on_mComboMorphologyOp_currentIndexChanged(int index)
{
    if (mEmitSignals)
        emit morphologyOpChanged((Filter::MorphologyOp)index);
}

void FilterWidget::on_mComboKernelShape_currentIndexChanged(int index)
{
    if (mEmitSignals)
        emit kernelShapeChanged((Filter::KernelShape)index);
}

void FilterWidget::on_mSliderHRadius_valueChanged(int value)
{
    ui->mSpinHRadius->setValue(value);
    if (mEmitSignals)
        emit hRadiusChanged(value);
}

void FilterWidget::on_mSpinHRadius_valueChanged(int arg1)
{
    ui->mSliderHRadius->setValue(arg1);
    if (ui->mButtonLockRadius->isChecked())
        ui->mSpinVRadius->setValue(arg1);
    if (mEmitSignals)
        emit hRadiusChanged(arg1);
}

void FilterWidget::on_mSliderVRadius_valueChanged(int value)
{
    ui->mSpinVRadius->setValue(value);
    if (mEmitSignals)
        emit vRadiusChanged(value);
}

void FilterWidget::on_mSpinVRadius_valueChanged(int arg1)
{
    ui->mSliderVRadius->setValue(arg1);
    if (ui->mButtonLockRadius->isChecked())
        ui->mSpinHRadius->setValue(arg1);
    if (mEmitSignals)
        emit vRadiusChanged(arg1);
}

void FilterWidget::on_mButtonLockRadius_toggled(bool checked)
{
    if (checked)
        ui->mSpinVRadius->setValue(ui->mSpinHRadius->value());
    if (mEmitSignals)
        emit lockRadiusChanged(checked);
}
