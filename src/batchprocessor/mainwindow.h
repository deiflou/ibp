#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFileSystemWatcher>
#include <QMenu>

#include "../imgproc/freeimage.h"
#include "../imgproc/imagefilterlist.h"
#include "../plugins/imagefilterpluginloader.h"
#if defined(Q_OS_WIN32) && defined(WIN_AERO)
#include "../misc/nativeeventfilter.h"
#endif

#include "imagefolderlistpopup.h"
#include "widgetlist.h"

using namespace anitools::misc;
using namespace anitools::plugins;

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
#ifdef Q_OS_WIN32
    NativeEventFilter mMainNativeEventFilter;
    QTimer mMainStyleChangedTimer;
#endif

    void mainLoad();
    void mainUnload();

    void mainReloadImageFilterListPresets();

    // Toolbar Edit
    ImageFolderListPopUp * mToolbarEditImageFolderListPopUp;
    QMenu * mToolbarEditButtonLoadFiltersMenu;

    void toolbarEditLoad();
    void toolbarEditUnload();
    void toolbarEditEventFilter(QObject *o, QEvent *e);

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
    QWidget * mViewEditWidgetDummyFade1, * mViewEditWidgetDummyFade2;
    bool mViewEditIsLoadingImageFilterList;
    bool mViewEditImageFilterListIsDirty;

    void viewEditLoad();
    void viewEditUnload();
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
#ifdef Q_OS_WIN32
    void On_mMainNativeEventFilter_nativeEvent(void * message, long * result);
    void On_mMainStyleChangedTimer_timeout();
#endif

    void On_mMainWatcherImageFilterListPresets_directoryChanged(const QString & path);

    // Toolbar Edit
    void on_mToolbarEditButtonLoadImage_clicked();
    void on_mToolbarEditButtonLoadImage_menuButtonPressed();
    void on_mToolbarEditButtonSaveImage_clicked();

    void on_mToolbarEditButtonVSplitter_toggled(bool checked);
    void on_mToolbarEditButtonHSplitter_toggled(bool checked);

    void on_mToolbarEditButtonLoadFilters_clicked();
    void On_mToolbarEditButtonLoadFiltersAction_triggered();
    bool on_mToolbarEditButtonSaveFilters_clicked();

    void On_mToolbarEditImageFolderListPopUp_fileSelected(const QString & fileName);
    void On_mToolbarEditImageFolderListPopUp_contentsChanged();

    void On_mToolbarEditButtonAddFilterAction_triggered();

    // View Edit
    void On_mViewEditImageFilterList_processingProgress(int p);
    void On_mViewEditImageFilterList_processingCompleted(const QImage & i);

    void on_mViewEditImagePreviewInput_zoomIndexChanged(int index);
    void on_mViewEditImagePreviewOutput_zoomIndexChanged(int index);
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
