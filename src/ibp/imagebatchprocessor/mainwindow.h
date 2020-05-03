//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

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

using namespace ibp::plugins;
using namespace ibp::imgproc;
using namespace ibp::widgets;

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
