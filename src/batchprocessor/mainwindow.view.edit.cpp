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
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::viewEditLoad()
{
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

    // Widgets Dummy Fade
    mViewEditWidgetDummyFade1 = new QWidget(this);
    mViewEditWidgetDummyFade2 = new QWidget(this);
    mViewEditWidgetDummyFade1->setAttribute(Qt::WA_TransparentForMouseEvents);
    mViewEditWidgetDummyFade2->setAttribute(Qt::WA_TransparentForMouseEvents);
    mViewEditWidgetDummyFade1->setStyleSheet("QWidget { background-color:"
                                             "qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                             "stop:0 palette(button), stop:1 rgba(0, 0, 0, 0)); }");
    mViewEditWidgetDummyFade2->setStyleSheet("QWidget { background-color:"
                                             "qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                             "stop:0 rgba(0, 0, 0, 0), stop:1 palette(button)); }");
    mViewEditWidgetDummyFade1->setAutoFillBackground(true);
    mViewEditWidgetDummyFade2->setAutoFillBackground(true);


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
    viewEditLoadImageFilterList(QApplication::applicationDirPath() + "/settings/celprocessor.ifl");
    mViewEditImageFilterListIsDirty = false;
}

void MainWindow::viewEditUnload()
{
    mViewEditImageFilterList.setName(tr("Current Filter List"));
    mViewEditImageFilterList.setDescription(tr("This file contains the filter list that celprocessor "
                                               "was using the last time it was closed."));
    mViewEditImageFilterList.save(QApplication::applicationDirPath() + "/settings/celprocessor.ifl");
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

        QRect r = ui->mViewEditWidgetList->rect();
        mViewEditWidgetDummyFade1->move(this->width() - ui->mViewEditWidgetListScrollArea->width(),
                                        ui->mMainContainerViews->y());
        mViewEditWidgetDummyFade1->resize(r.width(), 24);

        mViewEditWidgetDummyFade2->move(this->width() - ui->mViewEditWidgetListScrollArea->width(),
                                        this->height() - 24);
        mViewEditWidgetDummyFade2->resize(r.width(), 24);

        return;
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
    ui->mToolbarEditButtonLoadImage->setText(fi.fileName());
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
    QMutex m;
    m.lock();
    ui->mViewEditProgressBarProcessingOutput->setValue(p);
    ui->mViewEditProgressBarProcessingOutput->show();
    m.unlock();
}

void MainWindow::On_mViewEditImageFilterList_processingCompleted(const QImage &i)
{
    QMutex m;
    m.lock();
    mViewEditOutputImage = i.copy();
    ui->mViewEditComboOutputZoom->setEnabled(true);
    ui->mViewEditSliderOutputZoom->setEnabled(true);
    ui->mViewEditImagePreviewOutput->setImage(mViewEditOutputImage);
    ui->mToolbarEditButtonSaveImage->setEnabled(!mViewEditOutputImage.isNull());
    ui->mViewEditProgressBarProcessingOutput->hide();
    m.unlock();
}

void MainWindow::on_mViewEditImagePreviewInput_zoomIndexChanged(int index)
{
    ui->mViewEditComboInputZoom->setCurrentIndex(index);
}

void MainWindow::on_mViewEditImagePreviewOutput_zoomIndexChanged(int index)
{
    ui->mViewEditComboOutputZoom->setCurrentIndex(index);
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
