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

#include <QScrollBar>
#include <QGraphicsOpacityEffect>
#include <QDebug>
#include <math.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../misc/configurationmanager.h"

#define VIEWEDITCONTAINERZOOMOPACITY .25

using namespace anitools::misc;

void MainWindow::viewEditLoad()
{
    // Opacity effects for the zoom containers
    mViewEditContainerInputZoomOpacityEffect = new QGraphicsOpacityEffect();
    mViewEditContainerInputZoomOpacityEffect->setOpacity(VIEWEDITCONTAINERZOOMOPACITY);
    ui->mViewEditContainerInputZoom->setGraphicsEffect(mViewEditContainerInputZoomOpacityEffect);
    mViewEditContainerOutputZoomOpacityEffect = new QGraphicsOpacityEffect();
    mViewEditContainerOutputZoomOpacityEffect->setOpacity(VIEWEDITCONTAINERZOOMOPACITY);
    ui->mViewEditContainerOutputZoom->setGraphicsEffect(mViewEditContainerOutputZoomOpacityEffect);

    // Imageviews look
    ui->mViewEditImagePreviewInput->setFrameShape(QFrame::NoFrame);
    ui->mViewEditImagePreviewOutput->setFrameShape(QFrame::NoFrame);

    // Progress Bar Processing Output
    ui->mViewEditProgressBarProcessingOutput->hide();

    // Input/Output Zoom Combos & Sliders
    QStringList li;
    li << "6.25%" << "12.5%" << "25%" << "33.3%" << "50%" << "66.6%" << "75%" << "100%" << "200%" << "300%" <<
          "400%" << "600%" << "800%" << "1600%";
    ui->mViewEditComboInputZoom->addItems(li);
    ui->mViewEditComboOutputZoom->addItems(li);
    ui->mViewEditComboInputZoom->setEnabled(false);
    ui->mViewEditComboOutputZoom->setEnabled(false);
    ui->mViewEditSliderInputZoom->setValue(ui->mViewEditComboInputZoom->currentIndex());
    ui->mViewEditSliderOutputZoom->setValue(ui->mViewEditComboOutputZoom->currentIndex());
    ui->mViewEditSliderInputZoom->setEnabled(false);
    ui->mViewEditSliderOutputZoom->setEnabled(false);

    ui->mViewEditComboInputZoom->setCurrentText("100%");
    ui->mViewEditComboOutputZoom->setCurrentText("100%");

    // Splitter Main
    ui->mViewEditSplitterMain->setStretchFactor(0, 1);
    ui->mViewEditSplitterMain->setStretchFactor(1, 0);

    // Event filters
    ui->mViewEditWidgetListScrollArea->installEventFilter(this);
    ui->mViewEditWidgetList->installEventFilter(this);

    // Image Filter List & Preview Thread
    connect(&mViewEditImageFilterList, SIGNAL(processingProgress(int)),
            this, SLOT(On_mViewEditImageFilterList_processingProgress(int)));
    connect(&mViewEditImageFilterList, SIGNAL(processingCompleted(QImage)),
            this, SLOT(On_mViewEditImageFilterList_processingCompleted(QImage)));
    mViewEditImageFilterList.setAutoRun(true);
    mViewEditImageFilterList.setUseCache(true);
    mViewEditImageFilterList.setPluginLoader(&mMainImageFilterPluginLoader);
    viewEditLoadImageFilterList(ConfigurationManager::folder() + "/imagebatchprocessor.ifl");
    mViewEditImageFilterListIsDirty = false;

    // Load last image
    viewEditLoadInputImage(ConfigurationManager::value("viewedit/inputimagefilename", "").toString());

}

void MainWindow::viewEditShow()
{
    // Load view configuration
    int splitterPos = ConfigurationManager::value("viewedit/imagefilterlist/width", 0).toInt();
    ui->mViewEditSplitterMain->setSizes(QList<int>() << ui->mViewEditSplitterMain->width() - splitterPos -
                                        ui->mViewEditSplitterMain->handleWidth() << splitterPos);

    if (ConfigurationManager::value("viewedit/preview/splitterorientation", Qt::Vertical).toInt() == Qt::Horizontal)
        ui->mToolbarEditButtonHSplitter->setChecked(true);

    splitterPos = round(ConfigurationManager::value("viewedit/preview/splitterposition", 0).toDouble() *
                        (ui->mViewEditSplitterPreview->orientation() == Qt::Vertical ?
                        ui->mViewEditSplitterPreview->height() :
                        ui->mViewEditSplitterPreview->width()) / 100.);
    ui->mViewEditSplitterPreview->setSizes(QList<int>() << splitterPos <<
                                           (ui->mViewEditSplitterPreview->orientation() == Qt::Vertical ?
                                           ui->mViewEditSplitterPreview->height() :
                                           ui->mViewEditSplitterPreview->width()) -
                                           splitterPos - ui->mViewEditSplitterPreview->handleWidth());

    ui->mViewEditComboInputZoom->setCurrentIndex(
                ConfigurationManager::value("viewedit/preview/inputimagezoom", 7).toInt());
    ui->mViewEditComboOutputZoom->setCurrentIndex(
                ConfigurationManager::value("viewedit/preview/outputimagezoom", 7).toInt());

    QPoint pos = ConfigurationManager::value("viewedit/preview/inputimageposition", QPoint(0, 0)).toPoint();
    ui->mViewEditImagePreviewInput->horizontalScrollBar()->setValue(pos.x());
    ui->mViewEditImagePreviewInput->verticalScrollBar()->setValue(pos.y());
    pos = ConfigurationManager::value("viewedit/preview/outputimageposition", QPoint(0, 0)).toPoint();
    ui->mViewEditImagePreviewOutput->horizontalScrollBar()->setValue(pos.x());
    ui->mViewEditImagePreviewOutput->verticalScrollBar()->setValue(pos.y());
}

void MainWindow::viewEditUnload()
{
    mViewEditImageFilterList.setName(tr("Current Filter List"));
    mViewEditImageFilterList.setDescription(tr("This file contains the filter list that image batch processor "
                                               "was using the last time it was closed."));
    mViewEditImageFilterList.save(ConfigurationManager::folder() + "/imagebatchprocessor.ifl");

    // Save configuration
    ConfigurationManager::setValue("viewedit/inputimagefilename", mViewEditInputImageFilename);
    ConfigurationManager::setValue("viewedit/imagefilterlist/width", ui->mViewEditSplitterMain->sizes().at(1));

    ConfigurationManager::setValue("viewedit/preview/splitterorientation",
                                   ui->mViewEditSplitterPreview->orientation());
    ConfigurationManager::setValue("viewedit/preview/splitterposition",
                                   ui->mViewEditSplitterPreview->sizes().at(0) * 100. /
                                   (ui->mViewEditSplitterPreview->orientation() == Qt::Vertical ?
                                   ui->mViewEditSplitterPreview->height() :
                                   ui->mViewEditSplitterPreview->width()));

    ConfigurationManager::setValue("viewedit/preview/inputimagezoom", ui->mViewEditImagePreviewInput->zoomIndex());
    ConfigurationManager::setValue("viewedit/preview/outputimagezoom", ui->mViewEditImagePreviewOutput->zoomIndex());

    ConfigurationManager::setValue("viewedit/preview/inputimageposition", QPoint(
                                       ui->mViewEditImagePreviewInput->horizontalScrollBar()->value(),
                                       ui->mViewEditImagePreviewInput->verticalScrollBar()->value()));
    ConfigurationManager::setValue("viewedit/preview/outputimageposition", QPoint(
                                       ui->mViewEditImagePreviewOutput->horizontalScrollBar()->value(),
                                       ui->mViewEditImagePreviewOutput->verticalScrollBar()->value()));

}

void MainWindow::viewEditEventFilter(QObject *o, QEvent *e)
{
    if (o == ui->mViewEditSplitterPreview && e->type() == QEvent::Resize)
    {
        int s1 = ui->mViewEditLayoutPreview->contentsMargins().left() +
                 ui->mViewEditLayoutPreview->contentsMargins().right() +
                 ui->mViewEditSplitterPreview->width();
        int s2 = ui->mViewEdit->width() - s1 - ui->mViewEditSplitterMain->handleWidth();
        ui->mViewEditSplitterMain->setSizes(QList<int>() << s1 << s2);
        return;
    }
    if ((o == ui->mViewEditWidgetList || o == ui->mViewEditWidgetListScrollArea || o == this)
            && e->type() == QEvent::Resize)
    {
        if (o == ui->mViewEditWidgetList)
        {
            ui->mViewEditWidgetListScrollArea->setMinimumWidth(ui->mViewEditWidgetList->minimumSizeHint().width() + 20 +
                                               (ui->mViewEditWidgetListScrollArea->verticalScrollBar()->isVisible() ?
                                                ui->mViewEditWidgetListScrollArea->verticalScrollBar()->width() : 0));
        }

        return;
    }
    if (o == ui->mViewEditContainerInputZoom)
    {
        if (e->type() == QEvent::Enter)
            mViewEditContainerInputZoomOpacityEffect->setOpacity(1.);
        else if (e->type() == QEvent::Leave)
            mViewEditContainerInputZoomOpacityEffect->setOpacity(VIEWEDITCONTAINERZOOMOPACITY);
    }
    if (o == ui->mViewEditContainerOutputZoom)
    {
        if (e->type() == QEvent::Enter)
            mViewEditContainerOutputZoomOpacityEffect->setOpacity(1.);
        else if (e->type() == QEvent::Leave)
            mViewEditContainerOutputZoomOpacityEffect->setOpacity(VIEWEDITCONTAINERZOOMOPACITY);
    }
}

bool MainWindow::viewEditLoadInputImage(const QString &fileName)
{
    if (fileName.isEmpty() || !QFile::exists(fileName))
        return false;

    FIBITMAP * bm = 0;
    QImage img = freeimageLoadAs32Bits(fileName, false, &bm);
    if (img.isNull())
        return false;

    if (mViewEditInputImageFI)
        FreeImage_Unload(mViewEditInputImageFI);

    mViewEditInputImageFI = bm;
    mViewEditInputImage = img;
    mViewEditInputImageFilename = fileName;

    QFileInfo fi(fileName);
    ui->mToolbarEditButtonLoadImage->setToolTip("<p style='white-space:pre;'>" +
                                                tr("Load Image") + "<br/><i>" +
                                                tr("Current") + ": " + fi.fileName() + "</i></p>");


    ui->mViewEditImagePreviewInput->setImage(mViewEditInputImage);
    ui->mViewEditSliderInputZoom->setEnabled(true);
    ui->mViewEditComboInputZoom->setEnabled(true);
    mViewEditImageFilterList.setInputImage(mViewEditInputImage);

    return true;
}

bool MainWindow::viewEditSaveOutputImage(const QString &fileName, const QString & filter)
{
    QMutex m;
    m.lock();

    if (mViewEditOutputImage.isNull())
    {
        m.unlock();
        return false;
    }

    if (!freeimageSave32Bits(mViewEditOutputImage, fileName, filter))
    {
        m.unlock();
        return false;
    }

    m.unlock();

    return true;
}

bool MainWindow::viewEditLoadImageFilterList(const QString &fileName)
{
    mViewEditIsLoadingImageFilterList = true;

    if (!mViewEditImageFilterList.load(fileName))
    {
        mViewEditIsLoadingImageFilterList = false;
        return false;
    }

    ui->mViewEditWidgetList->setAnimate(false);
    ui->mViewEditWidgetList->clear();
    ImageFilter * filter;
    for (int i = 0; i < mViewEditImageFilterList.count(); i++)
    {
        filter = (ImageFilter *)mViewEditImageFilterList.at(i);
        if (!filter)
        {
            ui->mViewEditWidgetList->append(0);
            ui->mViewEditWidgetList->setWidgetBypass(i, mViewEditImageFilterList.bypass(i));
            ui->mViewEditWidgetList->setTitle(i, "<i>[" + tr("Broken Filter") + "]</i>");
        }
        else
        {
            ui->mViewEditWidgetList->append(filter->widget(this));
            ui->mViewEditWidgetList->setWidgetBypass(i, mViewEditImageFilterList.bypass(i));
            ui->mViewEditWidgetList->setTitle(i, filter->info().value("name"), filter->info().value("description"));
        }
    }
    ui->mViewEditWidgetList->setAnimate(true);

    mViewEditIsLoadingImageFilterList = false;

    return true;
}

void MainWindow::On_mViewEditImageFilterList_processingProgress(int p)
{
    ui->mViewEditProgressBarProcessingOutput->setValue(p);
    ui->mViewEditProgressBarProcessingOutput->show();
}

void MainWindow::On_mViewEditImageFilterList_processingCompleted(const QImage &i)
{
    mViewEditOutputImage = i.copy();
    ui->mViewEditComboOutputZoom->setEnabled(true);
    ui->mViewEditSliderOutputZoom->setEnabled(true);
    ui->mViewEditImagePreviewOutput->setImage(mViewEditOutputImage);
    ui->mToolbarEditButtonSaveImage->setEnabled(!mViewEditOutputImage.isNull());
    ui->mViewEditProgressBarProcessingOutput->hide();
}

void MainWindow::on_mViewEditImagePreviewInput_zoomIndexChanged(int index)
{
    ui->mViewEditComboInputZoom->setCurrentIndex(index);
}

void MainWindow::on_mViewEditImagePreviewInput_viewportResized(const QRect &r)
{
    ui->mViewEditContainerInputZoom->move(r.width() - ui->mViewEditContainerInputZoom->width() - 10,
                                          r.height() - ui->mViewEditContainerInputZoom->height() -10);
}

void MainWindow::on_mViewEditImagePreviewOutput_zoomIndexChanged(int index)
{
    ui->mViewEditComboOutputZoom->setCurrentIndex(index);
}

void MainWindow::on_mViewEditImagePreviewOutput_viewportResized(const QRect &r)
{
    ui->mViewEditProgressBarProcessingOutput->move(0, r.height()
                                                   - ui->mViewEditProgressBarProcessingOutput->height() + 1);
    ui->mViewEditProgressBarProcessingOutput->resize(r.width(), 0);

    ui->mViewEditContainerOutputZoom->move(r.width() - ui->mViewEditContainerOutputZoom->width() - 10,
                                           r.height() - ui->mViewEditContainerOutputZoom->height() -10);
}

void MainWindow::on_mViewEditSliderInputZoom_valueChanged(int value)
{
    ui->mViewEditComboInputZoom->setCurrentIndex(value);
}

void MainWindow::on_mViewEditComboInputZoom_currentIndexChanged(int index)
{
    ui->mViewEditSliderInputZoom->setValue(index);
    ui->mViewEditImagePreviewInput->setZoomIndex(index);
}

void MainWindow::on_mViewEditSliderOutputZoom_valueChanged(int value)
{
    ui->mViewEditComboOutputZoom->setCurrentIndex(value);
}

void MainWindow::on_mViewEditComboOutputZoom_currentIndexChanged(int index)
{
    ui->mViewEditSliderOutputZoom->setValue(index);
    ui->mViewEditImagePreviewOutput->setZoomIndex(index);
}

void MainWindow::on_mViewEditWidgetList_widgetInserted(int i)
{
    Q_UNUSED(i);

    bool v = ui->mViewEditWidgetList->count() != 0;
    ui->mToolbarEditButtonSaveFilters->setEnabled(v);
    mViewEditImageFilterListIsDirty = true;
}

void MainWindow::on_mViewEditWidgetList_widgetRemoved(int i)
{
    bool v = ui->mViewEditWidgetList->count() != 0;
    ui->mToolbarEditButtonSaveFilters->setEnabled(v);


    if (!mViewEditIsLoadingImageFilterList)
        mViewEditImageFilterList.removeAt(i);

    ui->mViewEditWidgetListScrollArea->setMinimumWidth(ui->mViewEditWidgetList->minimumSizeHint().width() + 20 +
                                       (ui->mViewEditWidgetListScrollArea->verticalScrollBar()->isVisible() ?
                                        ui->mViewEditWidgetListScrollArea->verticalScrollBar()->width() : 0));
    if (mViewEditImageFilterList.count() > 0)
        mViewEditImageFilterListIsDirty = true;
    else
        mViewEditImageFilterListIsDirty = false;
}

void MainWindow::on_mViewEditWidgetList_scrolligNeeded(int dir)
{
    ui->mViewEditWidgetListScrollArea->verticalScrollBar()->setValue(
                ui->mViewEditWidgetListScrollArea->verticalScrollBar()->value() +
                (dir * 5 / WidgetList::scrollingNeededDistance));
}

void MainWindow::on_mViewEditWidgetList_widgetMoved(int from, int to)
{
    mViewEditImageFilterList.move(from, to);
    mViewEditImageFilterListIsDirty = true;
}

void MainWindow::on_mViewEditWidgetList_widgetBypassStatusChanged(int i, bool c)
{
    mViewEditImageFilterList.setBypass(i, c);
    mViewEditImageFilterListIsDirty = true;
}
