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

#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMenu>
#include <QInputDialog>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../imgproc/freeimage.h"

void MainWindow::toolbarEditLoad()
{
    // Load Image Button
    ui->mToolbarEditButtonLoadImage->setText(tr("Load Image"));
    ui->mToolbarEditButtonLoadImage->setIcon(QIcon(":/batchprocessor/icons/loadImage"));
    ui->mToolbarEditButtonLoadImage->setIconSize(QSize(22, 22));
    ui->mToolbarEditButtonLoadImage->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->mToolbarEditButtonLoadImage->setAutoRaise(true);
    ui->mToolbarEditButtonLoadImage->setPopupMode(QToolButton::DelayedPopup);
    ui->mToolbarEditButtonLoadImage->setTextElideMode(Qt::ElideMiddle);
    ui->mToolbarEditButtonLoadImage->setMaximumTextWidth(100);

    // Save Image Button
    ui->mToolbarEditButtonSaveImage->setEnabled(!mViewEditOutputImage.isNull());

    // Image Folder List PopUp
    mToolbarEditImageFolderListPopUp = new ImageFolderListPopUp(this);
    mToolbarEditImageFolderListPopUp->hide();
    connect(mToolbarEditImageFolderListPopUp, SIGNAL(fileSelected(QString)),
            this, SLOT(On_mToolbarEditImageFolderListPopUp_fileSelected(QString)));
    connect(mToolbarEditImageFolderListPopUp, SIGNAL(contentsChanged()),
            this, SLOT(On_mToolbarEditImageFolderListPopUp_contentsChanged()));
    mToolbarEditImageFolderListPopUp->installEventFilter(this);

    // Add Filter Button
    ui->mToolbarEditButtonAddFilter->setText("");
    QIcon mToolbarEditButtonAddFilterIcon;
    mToolbarEditButtonAddFilterIcon.addFile(":/batchprocessor/icons/addImageFilter");
    mToolbarEditButtonAddFilterIcon.addFile(":/batchprocessor/icons/addImageFilterDisabled", QSize(), QIcon::Disabled);
    ui->mToolbarEditButtonAddFilter->setIcon(mToolbarEditButtonAddFilterIcon);
    ui->mToolbarEditButtonAddFilter->setIconSize(QSize(22, 22));
    ui->mToolbarEditButtonAddFilter->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->mToolbarEditButtonAddFilter->setAutoRaise(true);
    ui->mToolbarEditButtonAddFilter->setPopupMode(QToolButton::InstantPopup);
    if (mMainImageFilterPluginLoader.count() > 0)
        toolbarEditPopulateAddFilterMenu();
    else
        ui->mToolbarEditButtonAddFilter->setEnabled(false);

    // Load Filters Button
    ui->mToolbarEditButtonLoadFilters->setText("");
    QIcon mToolbarEditButtonLoadFiltersIcon;
    mToolbarEditButtonLoadFiltersIcon.addFile(":/batchprocessor/icons/loadImageFilters");
    mToolbarEditButtonLoadFiltersIcon.addFile(":/batchprocessor/icons/loadImageFiltersDisabled",
                                              QSize(), QIcon::Disabled);
    ui->mToolbarEditButtonLoadFilters->setIcon(mToolbarEditButtonLoadFiltersIcon);
    ui->mToolbarEditButtonLoadFilters->setIconSize(QSize(22, 22));
    ui->mToolbarEditButtonLoadFilters->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->mToolbarEditButtonLoadFilters->setAutoRaise(true);
    if (mMainImageFilterPluginLoader.count() == 0)
        ui->mToolbarEditButtonLoadFilters->setEnabled(false);
    toolbarEditReloadImageFilterListPresets();

    // Save Filters Button
    ui->mToolbarEditButtonSaveFilters->setEnabled(false);


}

void MainWindow::toolbarEditUnload()
{

}

void MainWindow::toolbarEditEventFilter(QObject *o, QEvent *e)
{
    if (o == this)
    {
        if (e->type() == QEvent::Resize)
        {
            QRect r;

            // Toolbar Edit
            r = rect();
            r.adjust(32, ui->mToolbarEditButtonLoadImage->mapTo(this,
                         ui->mToolbarEditButtonLoadImage->rect().bottomLeft()).y() - 4 + 1, -32, -32);
            mToolbarEditImageFolderListPopUp->move(r.topLeft());
            mToolbarEditImageFolderListPopUp->resize(r.size());
        }
    }

    if (e->type() == QEvent::MouseButtonPress)
    {
        if (mToolbarEditImageFolderListPopUp->isVisible())
        {
            QWidget * w = qobject_cast<QWidget*>(o);
            QMouseEvent * me = (QMouseEvent*)e;
            if (w && !mToolbarEditImageFolderListPopUp->rect().adjusted(0, ImageFolderListPopUp::arrowSize, 0, 0).
                    contains(w->mapTo(this, me->pos()) - mToolbarEditImageFolderListPopUp->pos()))
            {
                if (w == ui->mToolbarEditButtonLoadImage)
                {
                    QStyleOptionToolButton opt;
                    ui->mToolbarEditButtonLoadImage->initStyleOption(&opt);
                    QRect r = ui->mToolbarEditButtonLoadImage->style()->subControlRect(QStyle::CC_ToolButton,
                                                                                       &opt,
                                                                                       QStyle::SC_ToolButtonMenu,
                                                                                       this);
                    if (r.contains(me->pos()))
                        return;
                }
                mToolbarEditImageFolderListPopUp->hide();
            }
        }
    }

    if (o == mToolbarEditImageFolderListPopUp)
    {
        if (e->type() == QEvent::Show)
        {
            ui->mToolbarEditButtonLoadImage->setMenuButtonDown(true);
        }
        else if (e->type() == QEvent::Hide)
        {
            ui->mToolbarEditButtonLoadImage->setMenuButtonDown(false);
        }
    }
}

bool toolbarEditAddFilterActionSorter(QAction * a1, QAction * a2)
{
    QString s1 = a1->text();
    QString s2 = a2->text();
    return s1.toLower() < s2.toLower();
}
void MainWindow::toolbarEditPopulateAddFilterMenu()
{
    int i, j, k;
    QString id;
    QStringList ids = mMainImageFilterPluginLoader.ids();
    QHash<QString, QString> info;
    QAction * action;
    QList<QAction *> actions;
    QStringList tags, filterTags;
    for (i = 0; i < ids.size(); i++)
    {
        id = ids.at(i);
        info = mMainImageFilterPluginLoader.info(id);

        action = new QAction(this);
        action->setText(info.value("name"));
        action->setProperty("id", id);
        action->setProperty("tags", info.value("tags"));
        action->setProperty("type", "action");
        action->setToolTip("<h4>" + info.value("name") + "</h4>" +
                           "<p>" + info.value("description") + "</p>");
        connect(action, SIGNAL(triggered()), this, SLOT(On_mToolbarEditButtonAddFilterAction_triggered()));
        actions.append(action);
        filterTags = info.value("tags").split(QRegExp("\\s*,\\s*"), QString::SkipEmptyParts);
        for (j = 0; j < filterTags.size(); j++)
            if (!tags.contains(filterTags.at(j), Qt::CaseInsensitive))
                tags.append(filterTags.at(j));
    }
    if (actions.isEmpty())
        return;

    qSort(actions.begin(), actions.end(), toolbarEditAddFilterActionSorter);
    tags.sort(Qt::CaseInsensitive);

    QMenu * mainMenu = new QMenu(this);
    ui->mToolbarEditButtonAddFilter->setMenu(mainMenu);
    for (i = 0; i < tags.size(); i++)
        mainMenu->addMenu(tags.at(i));
    if (!tags.isEmpty())
        mainMenu->addSeparator();
    QMenu * allFiltersMenu = mainMenu->addMenu(tr("All Filters"));

    for (i = 0; i < actions.size(); i++)
    {
        filterTags = actions.at(i)->property("tags").toString().split(QRegExp("\\s*,\\s*"), QString::SkipEmptyParts);
        for (j = 0; j < filterTags.size(); j++)
        {
            for (k = 0; k < tags.size(); k++)
                if (mainMenu->actions().at(k)->text().compare(filterTags.at(j), Qt::CaseInsensitive) == 0)
                {
                    mainMenu->actions().at(k)->menu()->addAction(actions.at(i));
                    break;
                }
        }
        allFiltersMenu->addAction(actions.at(i));
    }

}

void MainWindow::toolbarEditReloadImageFilterListPresets()
{
    if (mMainImageFilterListPresets.size() == 0 || mMainImageFilterPluginLoader.count() == 0)
    {
        ui->mToolbarEditButtonLoadFilters->setPopupMode(QToolButton::DelayedPopup);
        return;
    }
    ui->mToolbarEditButtonLoadFilters->setPopupMode(QToolButton::MenuButtonPopup);

    QMenu * menu = new QMenu(this);
    QAction * action;
    for (int i = 0; i < mMainImageFilterListPresets.size(); i++)
    {
        action = new QAction(QString(mMainImageFilterListPresets.at(i).name).replace("&", "&&"), menu);
        action->setProperty("fileName", mMainImageFilterListPresets.at(i).fileName);
        connect(action, SIGNAL(triggered()), this, SLOT(On_mToolbarEditButtonLoadFiltersAction_triggered()));
        menu->addAction(action);
    }

    ui->mToolbarEditButtonLoadFilters->setMenu(menu);

    if (mToolbarEditButtonLoadFiltersMenu)
        delete mToolbarEditButtonLoadFiltersMenu;
    mToolbarEditButtonLoadFiltersMenu = menu;
}

void MainWindow::on_mToolbarEditButtonLoadImage_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, QString(), QString(), freeimageGetOpenFilterString());
    if (fileName.isEmpty()) return;

    if (!viewEditLoadInputImage(fileName))
    {
        QMessageBox::information(this, QString(), tr("The selected file has an unsupported format."));
        return;
    }

    mToolbarEditImageFolderListPopUp->setFolder(QFileInfo(fileName).path());
}

void MainWindow::on_mToolbarEditButtonLoadImage_menuButtonPressed()
{
    if (mToolbarEditImageFolderListPopUp->isVisible())
    {
        mToolbarEditImageFolderListPopUp->hide();
        return;
    }
    mToolbarEditImageFolderListPopUp->setArrowPos(
                ui->mToolbarEditButtonLoadImage->mapTo(this,
                QPoint(ui->mToolbarEditButtonLoadImage->width() >> 1, 0)).x() -
                mToolbarEditImageFolderListPopUp->x());
    mToolbarEditImageFolderListPopUp->raise();
    mToolbarEditImageFolderListPopUp->show();
}

void MainWindow::on_mToolbarEditButtonSaveImage_clicked()
{
    QString fileName, filter;
    fileName = QFileDialog::getSaveFileName(this, QString(), QString(), freeimageGetSaveFilterString(), &filter);
    if (fileName.isEmpty()) return;

    if (!viewEditSaveOutputImage(fileName, filter))
        QMessageBox::information(this, QString(), tr("An error has happen while saving the file to disk."));
}

void MainWindow::on_mToolbarEditButtonLoadFilters_clicked()
{
    if (mViewEditImageFilterListIsDirty)
        if (QMessageBox::question(this, QString(), tr("Do you want to save the current filter "
                                                      "list before loading the new one?")) == QMessageBox::Yes)
            if (!on_mToolbarEditButtonSaveFilters_clicked())
                return;

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, QString(), QString(),
                                            tr("Anitools Image Filter List (*.ifl);;") +
                                            tr("All Files (*)"));
    if (fileName.isEmpty()) return;


    if (!viewEditLoadImageFilterList(fileName))
    {
        QMessageBox::information(this, QString(), tr("The selected file has an unsupported format."));
        return;
    }

    mViewEditImageFilterListIsDirty = false;
}

void MainWindow::On_mToolbarEditButtonLoadFiltersAction_triggered()
{
    QString fileName = sender()->property("fileName").toString();

    if (!QFile::exists(fileName))
    {
        QMessageBox::information(this, QString(), tr("The selected file doesn't exist."));
        return;
    }

    if (mViewEditImageFilterListIsDirty)
        if (QMessageBox::question(this, QString(), tr("Do you want to save the current filter "
                                                      "list before loading the new one?")) == QMessageBox::Yes)
            if (!on_mToolbarEditButtonSaveFilters_clicked())
                return;

    if (!viewEditLoadImageFilterList(fileName))
    {
        QMessageBox::information(this, QString(), tr("The selected file has an unsupported format."));
        return;
    }

    mViewEditImageFilterListIsDirty = false;
}

bool MainWindow::on_mToolbarEditButtonSaveFilters_clicked()
{
    QString name, description, fileName;
    name = QInputDialog::getText(this, QString(), tr("Write a name for this image filter list."));
    if (name.isNull())
        return false;
    description = QInputDialog::getText(this, QString(), tr("Write a description for this image filter list."));
    if (description.isNull())
        return false;
    fileName = QFileDialog::getSaveFileName(this, QString(), QString(),
                                            tr("Anitools Image Filter List (*.ifl);;") +
                                            tr("All Files (*)"));
    if (fileName.isEmpty()) return false;

    mViewEditImageFilterList.setName(name);
    mViewEditImageFilterList.setDescription(description);
    if (!mViewEditImageFilterList.save(fileName))
    {
        QMessageBox::information(this, QString(), tr("An error has happen while saving the file to disk."));
        return false;
    }

    mViewEditImageFilterListIsDirty = false;
    return true;
}

void MainWindow::on_mToolbarEditButtonVSplitter_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mViewEditSplitterPreview->setOrientation(Qt::Vertical);
}

void MainWindow::on_mToolbarEditButtonHSplitter_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mViewEditSplitterPreview->setOrientation(Qt::Horizontal);
}

void MainWindow::On_mToolbarEditImageFolderListPopUp_fileSelected(const QString &fileName)
{
    mToolbarEditImageFolderListPopUp->hide();
    viewEditLoadInputImage(fileName);
}

void MainWindow::On_mToolbarEditImageFolderListPopUp_contentsChanged()
{
    if (mToolbarEditImageFolderListPopUp->countAvailableFiles() > 0)
        ui->mToolbarEditButtonLoadImage->setPopupMode(QToolButton::MenuButtonPopup);
    else
    {
        mToolbarEditImageFolderListPopUp->hide();
        ui->mToolbarEditButtonLoadImage->setPopupMode(QToolButton::DelayedPopup);
    }
}

void MainWindow::On_mToolbarEditButtonAddFilterAction_triggered()
{
    QString id = sender()->property("id").toString();
    ImageFilter * f = mMainImageFilterPluginLoader.instantiateFilter(id);
    if (!f)
    {
        return;
    }
    f->setParent(&mViewEditImageFilterList);
    mViewEditImageFilterList.append(f);

    ui->mViewEditWidgetList->append(f->widget(this));
    ui->mViewEditWidgetList->setTitle(ui->mViewEditWidgetList->count() - 1,
                                      f->info().value("name"), f->info().value("description"));
}
