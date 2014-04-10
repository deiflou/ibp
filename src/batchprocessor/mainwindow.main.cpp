#include <QFile>
#include <QFileInfo>
#include <QDebug>

#ifdef Q_OS_WIN32
#include <QtWinExtras>
#include <winuser.h>
#include <dwmapi.h>
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    // Main
    mMainWatcherImageFilterListPresets(0),
    // Toolbar Edit
    mToolbarEditImageFolderListPopUp(0),
    mToolbarEditButtonLoadFiltersMenu(0),
    // View Edit
    mViewEditInputImage(),
    mViewEditOutputImage(),
    mViewEditInputImageFI(0),
    mViewEditInputImageFilename(),
    mViewEditWidgetDummyFade1(0),
    mViewEditWidgetDummyFade2(0),
    mViewEditIsLoadingImageFilterList(false),
    mViewEditImageFilterListIsDirty(false)

{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    QtWin::extendFrameIntoClientArea(this, 0, ui->mToolbarEdit->minimumSizeHint().height() + 15, 0, 0);
    mainLoad();
    toolbarEditLoad();
    toolbarBatchLoad();
    viewEditLoad();
    viewBatchLoad();
}

MainWindow::~MainWindow()
{
    mainUnload();
    toolbarEditUnload();
    toolbarBatchUnload();
    viewEditUnload();
    viewBatchUnload();

    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    toolbarEditEventFilter((QObject*)this, (QEvent*)e);
    viewEditEventFilter((QObject*)this, (QEvent*)e);
}

bool MainWindow::eventFilter(QObject *o, QEvent *e)
{
    toolbarEditEventFilter(o, e);
    viewEditEventFilter(o, e);

    return QWidget::eventFilter(o, e);
}

void MainWindow::mainLoad()
{
    // Windows Native Event Filter
#ifdef Q_OS_WIN32
    mMainStyleChangedTimer.setSingleShot(true);
    connect(&mMainStyleChangedTimer, SIGNAL(timeout()),
            this, SLOT(On_mMainStyleChangedTimer_timeout()));
    mMainNativeEventFilter.registerMessage(WM_THEMECHANGED);
    mMainNativeEventFilter.registerMessage(WM_SYSCOLORCHANGE);
    mMainNativeEventFilter.registerMessage(WM_DWMCOMPOSITIONCHANGED);
    mMainNativeEventFilter.registerMessage(WM_DWMCOLORIZATIONCOLORCHANGED);
    mMainNativeEventFilter.registerMessage(WM_DWMNCRENDERINGCHANGED);
#ifdef WIN_AERO
    mMainNativeEventFilter.registerMessage(WM_NCHITTEST, true);
#endif
    connect(&mMainNativeEventFilter, SIGNAL(nativeEvent(void*,long*)),
            this, SLOT(On_mMainNativeEventFilter_nativeEvent(void*,long*)));
    On_mMainNativeEventFilter_nativeEvent(0, 0);
#endif

    // Image Filter Plugin Loader
    mMainImageFilterPluginLoader.load(QApplication::applicationDirPath() + "/plugins");
    // General
    qApp->installEventFilter(this);
    // File System Watcher Image Filter List Pressets
    mMainWatcherImageFilterListPresets = new QFileSystemWatcher(this);
    mMainWatcherImageFilterListPresets->addPath(QApplication::applicationDirPath() + "/presets/image filter list");
    connect(mMainWatcherImageFilterListPresets, SIGNAL(directoryChanged(QString)),
            this, SLOT(On_mMainWatcherImageFilterListPresets_directoryChanged(QString)));
    // Image Filter List Pressets
    mainReloadImageFilterListPresets();
}

void MainWindow::mainUnload()
{
}

void MainWindow::mainReloadImageFilterListPresets()
{
    QDir dir(QApplication::applicationDirPath() + "/presets/image filter list");
    if (!dir.exists())
        return;

    QStringList list = dir.entryList(QDir::Files);
    QString fileType;
    MainImageFilterListPresetsInfo info;
    mMainImageFilterListPresets.clear();
    for (int i = 0; i < list.size(); i++)
    {
        QSettings s(dir.path() + QDir::separator() + list.at(i), QSettings::IniFormat);
        s.beginGroup("info");

        fileType = s.value("fileType", QString()).toString();
        if (fileType != "anitools.imagefilterlist")
            continue;

        info.name = s.value("name", QString()).toString();
        if (info.name.isEmpty())
            info.name = tr("Unnamed Image Filter List") + " [" + list.at(i) + "]";
        info.description = s.value("description").toString();
        info.fileName = s.fileName();

        mMainImageFilterListPresets.append(info);

        s.endGroup();
    }
}

void MainWindow::on_mMainButtonEdit_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mMainContainerToolbars->setFixedWidth(ui->mToolbarEdit->minimumSizeHint().width());
    ui->mMainContainerToolbars->setCurrentWidget(ui->mToolbarEdit);
    ui->mMainContainerViews->setCurrentWidget(ui->mViewEdit);
}

void MainWindow::on_mMainButtonBatch_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mMainContainerToolbars->setFixedWidth(ui->mToolbarBatch->minimumSizeHint().width());
    ui->mMainContainerToolbars->setCurrentWidget(ui->mToolbarBatch);
    ui->mMainContainerViews->setCurrentWidget(ui->mViewBatch);
}

#ifdef Q_OS_WIN32
void MainWindow::On_mMainNativeEventFilter_nativeEvent(void *message, long *result)
{
    MSG * m = (MSG *)message;
    unsigned short lw;
    if (m)
        lw = LOWORD(m->message);
    else
        return;

    switch (lw)
    {
#ifdef WIN_AERO
    case WM_NCHITTEST:
    {
        if (m->hwnd == (HWND)this->effectiveWinId() && QtWin::isCompositionEnabled())
        {
            POINTS wpt = MAKEPOINTS(m->lParam);
            QPoint pt = ui->mToolbarMoveHandle->mapFromGlobal(QPoint(wpt.x, wpt.y));
            if (ui->mToolbarMoveHandle->rect().contains(pt))
            {
                *result = HTCAPTION;
                break;
            }
        }

        *result = DefWindowProc(m->hwnd, m->message, m->wParam, m->lParam);
        break;
    }
#endif
    default:
    {
#ifdef WIN_AERO
        if (QtWin::isCompositionEnabled())
        {
            ui->mMainSeparatorToolbars->hide();
            ui->mMainContainerViews->setAutoFillBackground(true);
            ui->mMainContainer->setAutoFillBackground(false);
            ui->mToolbarEditSeparator01->setStyleSheet("QFrame{"
                                                       "border-left: 1px solid rgba(0, 0, 0, 64);"
                                                       "border-top: 0px;"
                                                       "border-right: 1px solid rgba(255, 255, 255, 64);"
                                                       "border-bottom: 0px;"
                                                       "}");
            ui->mToolbarEditSeparator02->setStyleSheet(ui->mToolbarEditSeparator01->styleSheet());
            ui->mToolbarEdit->layout()->setContentsMargins(5, 5, 0, 10);
            ui->mToolbarBatch->layout()->setContentsMargins(5, 5, 0, 10);
            ui->mMainContainerChangeSection->layout()->setContentsMargins(0, 5, 5, 10);

            setAttribute(Qt::WA_TranslucentBackground);
            QtWin::extendFrameIntoClientArea(this, 0, ui->mToolbarEdit->minimumSizeHint().height(), 0, 0);
        }
        else
        {
            ui->mMainSeparatorToolbars->show();
            ui->mMainContainerViews->setAutoFillBackground(false);
            ui->mMainContainer->setAutoFillBackground(true);
            ui->mToolbarEditSeparator01->setStyleSheet("");
            ui->mToolbarEditSeparator02->setStyleSheet("");
            ui->mToolbarEdit->layout()->setContentsMargins(10, 10, 0, 10);
            ui->mToolbarBatch->layout()->setContentsMargins(10, 10, 0, 10);
            ui->mMainContainerChangeSection->layout()->setContentsMargins(0, 10, 10, 10);

            setAttribute(Qt::WA_TranslucentBackground, false);
            setAttribute(Qt::WA_NoSystemBackground, false);
            QtWin::extendFrameIntoClientArea(this, 0, 0, 0, 0);
        }
#endif
        if (lw == WM_SYSCOLORCHANGE)
            mMainStyleChangedTimer.start(500);

        break;
    }
    }
}

void MainWindow::On_mMainStyleChangedTimer_timeout()
{
    extern void setDarkPalette();
    setDarkPalette();
}
#endif

void MainWindow::On_mMainWatcherImageFilterListPresets_directoryChanged(const QString &path)
{
    Q_UNUSED(path);

    mainReloadImageFilterListPresets();
    toolbarEditReloadImageFilterListPresets();
}
