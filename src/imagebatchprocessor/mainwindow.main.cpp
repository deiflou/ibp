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

#include <QFile>
#include <QFileInfo>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QWindowStateChangeEvent>
#include <QGraphicsDropShadowEffect>
//#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../misc/configurationmanager.h"

using namespace anitools::misc;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    // Main
    mMainWidgetDummyFade1(0),
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

    toolbarEditEventFilter((QObject*)this, (QEvent*)e);
    viewEditEventFilter((QObject*)this, (QEvent*)e);

    // Dummy fade widget
    mMainWidgetDummyFade1->move(0, ui->mMainContainerTop->height());
    mMainWidgetDummyFade1->resize(this->width(), 16);

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
    toolbarEditEventFilter(o, e);
    viewEditEventFilter(o, e);

    return QWidget::eventFilter(o, e);
}

void MainWindow::mainLoad()
{
    // Widgets Dummy Fade
    mMainWidgetDummyFade1 = new QWidget(this);
    mMainWidgetDummyFade1->setAttribute(Qt::WA_TransparentForMouseEvents);
    mMainWidgetDummyFade1->setStyleSheet("QWidget { background-color:"
                                         "qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,"
                                         "stop:0 rgba(0, 0, 0, .4),"
                                         "stop:.2 rgba(0, 0, 0, .32),"
                                         "stop:.4 rgba(0, 0, 0, .16),"
                                         "stop:.6 rgba(0, 0, 0, .05),"
                                         "stop:.8 rgba(0, 0, 0, .01),"
                                         "stop:1 rgba(0, 0, 0, .0)); }");
    mMainWidgetDummyFade1->setAutoFillBackground(true);

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

void MainWindow::On_mMainWatcherImageFilterListPresets_directoryChanged(const QString &path)
{
    Q_UNUSED(path);

    mainReloadImageFilterListPresets();
    toolbarEditReloadImageFilterListPresets();
}
