/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFileSystemWatcher>
#include <QMenu>
#include <QGraphicsOpacityEffect>

#include "../imgproc/freeimage.h"
#include "../imgproc/imagefilterlist.h"
#include "../plugins/imagefilterpluginloader.h"

#include "../widgets/widgetlist.h"

using namespace anitools::plugins;
using namespace anitools::imgproc;
using namespace anitools::widgets;

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    // Main
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    // Main
    void resizeEvent(QResizeEvent *e);
    void moveEvent(QMoveEvent *e);
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

private:
    Ui::MainWindow *ui;

    // Main
    ImageFilterPluginLoader mMainImageFilterPluginLoader;
    QFileSystemWatcher * mMainWatcherImageFilterListPresets;
    struct MainImageFilterListPresetsInfo
    {
        QString fileName, name, description;
    };
    QList<MainImageFilterListPresetsInfo> mMainImageFilterListPresets;

    QSize mMainWindowSize, mMainOldWindowSize;
    QPoint mMainWindowPos, mMainOldWindowPos;
    bool mMainIsMaximized;

    void mainLoad();
    void mainUnload();

    void mainReloadImageFilterListPresets();

    // Toolbar Edit
    QMenu * mToolbarEditButtonLoadFiltersMenu;

    void toolbarEditLoad();
    void toolbarEditUnload();

    void toolbarEditPopulateAddFilterMenu();

    void toolbarEditReloadImageFilterListPresets();

    // Toolbar Batch
    void toolbarBatchLoad();
    void toolbarBatchUnload();

    // View Edit
    QImage mViewEditInputImage, mViewEditOutputImage;
    FIBITMAP * mViewEditInputImageFI;
    QString mViewEditInputImageFilename;
    ImageFilterList mViewEditImageFilterList;
    bool mViewEditIsLoadingImageFilterList;
    bool mViewEditImageFilterListIsDirty;
    QGraphicsOpacityEffect * mViewEditContainerInputZoomOpacityEffect, * mViewEditContainerOutputZoomOpacityEffect;

    void viewEditLoad();
    void viewEditUnload();
    void viewEditShow();
    void viewEditEventFilter(QObject *o, QEvent *e);

    bool viewEditLoadInputImage(const QString & fileName);
    bool viewEditSaveOutputImage(const QString & fileName, const QString &filter);

    bool viewEditLoadImageFilterList(const QString & fileName);

    // View Batch
    void viewBatchLoad();
    void viewBatchUnload();

private slots:
    // Main
    void on_mMainButtonEdit_toggled(bool checked);
    void on_mMainButtonBatch_toggled(bool checked);

    void On_mMainWatcherImageFilterListPresets_directoryChanged(const QString & path);

    // Toolbar Edit
    void on_mToolbarEditButtonLoadImage_clicked();
    void on_mToolbarEditButtonSaveImage_clicked();

    void on_mToolbarEditButtonVSplitter_toggled(bool checked);
    void on_mToolbarEditButtonHSplitter_toggled(bool checked);

    void on_mToolbarEditButtonLoadFilters_clicked();
    void On_mToolbarEditButtonLoadFiltersAction_triggered();
    bool on_mToolbarEditButtonSaveFilters_clicked();

    void On_mToolbarEditButtonAddFilterAction_triggered();

    // View Edit
    void On_mViewEditImageFilterList_processingProgress(int p);
    void On_mViewEditImageFilterList_processingCompleted(const QImage & i);

    void on_mViewEditImagePreviewInput_zoomIndexChanged(int index);
    void on_mViewEditImagePreviewInput_viewportResized(const QRect & r);
    void on_mViewEditImagePreviewOutput_zoomIndexChanged(int index);
    void on_mViewEditImagePreviewOutput_viewportResized(const QRect & r);
    void on_mViewEditSliderInputZoom_valueChanged(int value);
    void on_mViewEditComboInputZoom_currentIndexChanged(int index);
    void on_mViewEditSliderOutputZoom_valueChanged(int value);
    void on_mViewEditComboOutputZoom_currentIndexChanged(int index);

    void on_mViewEditWidgetList_widgetInserted(int i);
    void on_mViewEditWidgetList_widgetRemoved(int i);
    void on_mViewEditWidgetList_scrolligNeeded(int dir);
    void on_mViewEditWidgetList_widgetMoved(int from, int to);
    void on_mViewEditWidgetList_widgetBypassStatusChanged(int i, bool c);
};

#endif // MAINWINDOW_H
