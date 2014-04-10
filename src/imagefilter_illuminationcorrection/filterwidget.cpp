#include <QFileDialog>
#include <QMessageBox>

#include <math.h>

#include "filterwidget.h"
#include "ui_filterwidget.h"
#include "../imgproc/freeimage.h"
#include "../widgets/toolbuttonex.h"

using namespace anitools::widgets;

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true)
{
    ui->setupUi(this);
    ui->mContainerImage->hide();
    ui->mButtonImage->setExtraDataType(ToolButtonEx::Image);
    ui->mButtonImage->setExtraDataFlags(ToolButtonEx::ExtraDataFlags(ToolButtonEx::ImageStretched |
                                                                     ToolButtonEx::ImageKeepAspectRatio |
                                                                     ToolButtonEx::ImageStretchOnlyIfBiggerThanButton |
                                                                     ToolButtonEx::CheckerboardBackground));
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setMode(int m)
{
    if ((m == 0 && ui->mButtonAuto->isChecked()) || (m == 1 && ui->mButtonFromImage->isChecked()))
        return;
    mEmitSignals = false;
    if (m == 0)
        ui->mButtonAuto->setChecked(true);
    else
        ui->mButtonFromImage->setChecked(true);
    mEmitSignals = true;
    emit modeChanged(m);
}

void FilterWidget::setImage(const QImage &i)
{
    if (i == ui->mButtonImage->image())
        return;
    mEmitSignals = false;
    ui->mButtonImage->setImage(i);
    mEmitSignals = true;
    emit imageChanged(i);
}

void FilterWidget::on_mButtonAuto_toggled(bool c)
{
    if (!c)
        return;
    ui->mContainerImage->hide();
    if (mEmitSignals)
        emit modeChanged(0);
}

void FilterWidget::on_mButtonFromImage_toggled(bool c)
{
    if (!c)
        return;
    ui->mContainerImage->show();
    if (mEmitSignals)
        emit modeChanged(1);
}

void FilterWidget::on_mButtonImage_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, QString(), QString(), freeimageGetOpenFilterString());
    if (fileName.isEmpty()) return;

    QImage img = freeimageLoadAs32Bits(fileName, true);
    if (img.isNull())
    {
        QMessageBox::information(this, QString(), tr("The selected file has an unsupported format."));
        return;
    }

    setImage(img);
}
