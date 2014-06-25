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
#include <QDebug>

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
    mViewEditImageFilterList.wait();
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

void MainWindow::On_mMainWatcherImageFilterListPresets_directoryChanged(const QString &path)
{
    Q_UNUSED(path);

    mainReloadImageFilterListPresets();
    toolbarEditReloadImageFilterListPresets();
}
