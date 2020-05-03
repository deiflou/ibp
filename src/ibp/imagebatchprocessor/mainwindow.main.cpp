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

#include <QFile>
#include <QFileInfo>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QWindowStateChangeEvent>
#include <QGraphicsDropShadowEffect>
#include <QDir>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../misc/configurationmanager.h"

using namespace ibp::misc;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    // Main
    mMainWatcherImageFilterListPresets(0),
    // Toolbar Edit
    mToolbarEditButtonLoadFiltersMenu(0),
    // View Edit
    mViewEditInputImage(),
    mViewEditOutputImage(),
    mViewEditInputImageFI(0),
    mViewEditInputImageFilename(),
    mViewEditIsLoadingImageFilterList(false),
    mViewEditImageFilterListIsDirty(false)

{
    ui->setupUi(this);

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
    Q_UNUSED(e);

    viewEditEventFilter((QObject*)this, (QEvent*)e);

    // Window size & state
    if (this->windowState() == Qt::WindowNoState)
    {
        mMainWindowSize = e->size();
        mMainOldWindowSize = e->oldSize();
    }
    //qDebug() << "resize" << this->windowState() << mMainWindowSize << mMainOldWindowSize;
}

void MainWindow::moveEvent(QMoveEvent *e)
{
    Q_UNUSED(e);

    if (this->windowState() == Qt::WindowNoState)
    {
        mMainWindowPos = this->pos();
        mMainOldWindowPos = e->oldPos() - (e->pos() - this->pos());
    }
    //qDebug() << "move" << mMainWindowPos << mMainOldWindowPos;
}

void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent * event = (QWindowStateChangeEvent *)e;

        if (event->oldState() == Qt::WindowNoState && this->windowState() == Qt::WindowMaximized && !mMainIsMaximized)
        {
            mMainWindowSize = mMainOldWindowSize;
            mMainWindowPos = mMainOldWindowPos;
            mMainIsMaximized = true;
            //qDebug() << "max" << mMainWindowPos << mMainOldWindowPos << mMainWindowSize << mMainOldWindowSize;
        }
        else if (event->oldState() == Qt::WindowMaximized && this->windowState() == Qt::WindowNoState)
        {
            mMainIsMaximized = false;
            //qDebug() << "normal" << mMainWindowPos << mMainOldWindowPos << mMainWindowSize << mMainOldWindowSize;
        }
    }
    QWidget::changeEvent(e);
}

void MainWindow::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);

    // Resize, move, show
    mMainWindowSize = mMainOldWindowSize = ConfigurationManager::value("mainwindow/size", QSize()).toSize();
    mMainWindowPos = mMainOldWindowPos = ConfigurationManager::value("mainwindow/position", QPoint()).toPoint();
    mMainIsMaximized = ConfigurationManager::value("mainwindow/ismaximized", false).toBool();
    //qDebug() << "show" << mMainWindowPos << mMainOldWindowPos << mMainWindowSize << mMainOldWindowSize;
    this->resize(mMainWindowSize);
    this->move(mMainWindowPos);


    if (mMainIsMaximized)
        this->showMaximized();

    viewEditShow();

    // View change
    if (ConfigurationManager::value("mainwindow/currentview", "edit").toString() == "batch")
        ui->mMainButtonBatch->setChecked(true);

    QWidget::showEvent(e);
}

bool MainWindow::eventFilter(QObject *o, QEvent *e)
{
    viewEditEventFilter(o, e);

    return QWidget::eventFilter(o, e);
}

void MainWindow::mainLoad()
{
    // Image Filter Plugin Loader
    mMainImageFilterPluginLoader.load(QApplication::applicationDirPath() + "/plugins");

    // General
    qApp->installEventFilter(this);

    // File System Watcher Image Filter List Pressets
    mMainWatcherImageFilterListPresets = new QFileSystemWatcher(this);
    mMainWatcherImageFilterListPresets->addPath(QApplication::applicationDirPath() + "/presets/imagefilterlists");
    connect(mMainWatcherImageFilterListPresets, SIGNAL(directoryChanged(QString)),
            this, SLOT(On_mMainWatcherImageFilterListPresets_directoryChanged(QString)));

    // Image Filter List Pressets
    mainReloadImageFilterListPresets();
}

void MainWindow::mainUnload()
{
    mViewEditImageFilterList.wait();

    ConfigurationManager::setValue("mainwindow/size", mMainWindowSize);
    ConfigurationManager::setValue("mainwindow/position", mMainWindowPos);
    ConfigurationManager::setValue("mainwindow/ismaximized", mMainIsMaximized);

    QString currentView = ui->mMainButtonBatch->isChecked() ? "batch" : "edit";
    ConfigurationManager::setValue("mainwindow/currentview", currentView);
}

void MainWindow::mainReloadImageFilterListPresets()
{
    QDir dir(QApplication::applicationDirPath() + "/presets/imagefilterlists");
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
        if (fileType != "ibp.imagefilterlist")
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
    ui->mMainContainerToolbars->setFixedHeight(ui->mToolbarEdit->minimumSizeHint().height());
    ui->mMainContainerToolbars->setCurrentWidget(ui->mToolbarEdit);
    ui->mMainContainerViews->setCurrentWidget(ui->mViewEdit);
}

void MainWindow::on_mMainButtonBatch_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mMainContainerToolbars->setFixedHeight(ui->mToolbarBatch->minimumSizeHint().height());
    ui->mMainContainerToolbars->setCurrentWidget(ui->mToolbarBatch);
    ui->mMainContainerViews->setCurrentWidget(ui->mViewBatch);
}

void MainWindow::On_mMainWatcherImageFilterListPresets_directoryChanged(const QString &path)
{
    Q_UNUSED(path);

    mainReloadImageFilterListPresets();
    toolbarEditReloadImageFilterListPresets();
}
