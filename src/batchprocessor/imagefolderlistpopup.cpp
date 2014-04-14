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

#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsEffect>
#include <QScrollBar>
#include <QDebug>

#include "imagefolderlistpopup.h"
#include "ui_imagefolderlistpopup.h"
#include "../imgproc/freeimage.h"

using namespace anitools::imgproc;

ImageFolderListPopUp::ImageFolderListPopUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageFolderListPopUp),
    mFolder(),
    mIsLoaded(false),
    mTempFolder(),
    mArrowPos(10)
{
    ui->setupUi(this);

    QMargins margins = layout()->contentsMargins();
    margins.setTop(margins.top() + arrowSize);
    layout()->setContentsMargins(margins);

    QString c;
    QColor color = palette().color(QPalette::Base);
    c = QString::number(color.red()) + "," +  QString::number(color.green()) + "," +
             QString::number(color.blue()) + "," + QString::number(alpha);
    setStyleSheet(QString("") +
                  "#mContainer, #mListView{background-color:rgba(" + c + ");}" +
                  "#mListView{border-bottom-left-radius:2; border-bottom-right-radius:2;}" +
                  "#mContainer{border-top-left-radius:2; border-top-right-radius:2;}" +
                  "QAbstractScrollArea{padding:0 2 2 2;}" +
                  "#mSeparator{color:palette(mid);margin:0 2 0 2;}" +
                  "");

    mFolderSystemModel = new QFileSystemModel(this);
    mFolderSystemModel->setNameFilters(freeimageGetOpenFilterExtensions(true));
    mFolderSystemModel->setNameFilterDisables(false);

    ui->mListView->setModel(mFolderSystemModel);
    ui->mListView->verticalScrollBar()->setSingleStep(32);
    ui->mListView->horizontalScrollBar()->setSingleStep(32);

    mTimer = new QTimer(this);

    connect(mFolderSystemModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(On_mFolderSystemModel_rowsChanged(QModelIndex,int,int)));
    connect(mFolderSystemModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(On_mFolderSystemModel_rowsChanged(QModelIndex,int,int)));

    connect(mTimer, SIGNAL(timeout()), this, SLOT(On_mTimer_timeout()));

    QGraphicsDropShadowEffect * shadow = new QGraphicsDropShadowEffect();
    shadow->setOffset(0, 8);
    shadow->setBlurRadius(48);
    color = palette().color(QPalette::Shadow);
    color.setAlpha(255);
    shadow->setColor(color);
    setGraphicsEffect(shadow);

    mTimer->start(200);
}

ImageFolderListPopUp::~ImageFolderListPopUp()
{
    delete ui;
}

int ImageFolderListPopUp::countAvailableFiles()
{
    if (!QFileInfo::exists(mFolder))
        return 0;
    return mFolderSystemModel->rowCount(ui->mListView->rootIndex());
}

void ImageFolderListPopUp::setFolder(const QString &folder)
{
    QDir dir(folder);
    if (folder.isEmpty() || !dir.exists())
        return;

    mIsLoaded = false;
    ui->mListView->setRootIndex(mFolderSystemModel->setRootPath(folder));
    mFolderSystemModel->setFilter(QDir::Dirs);
    mFolderSystemModel->setFilter(QDir::Files);

    ui->mLabelFolder->setText(folder);
    mIsLoaded = true;

    mFolder = folder;
}

void ImageFolderListPopUp::setArrowPos(int pos)
{
    if (mArrowPos == pos) return;
    mArrowPos = pos;
    repaint();
}

void ImageFolderListPopUp::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter p(this);

    QColor c = palette().color(QPalette::Base);
    c.setAlpha(alpha);

    QPainterPath pp(QPointF(mArrowPos, 0));
    pp.lineTo(mArrowPos + arrowSize - 1, arrowSize);
    pp.lineTo(mArrowPos - arrowSize - 1, arrowSize);
    pp.closeSubpath();

    p.fillPath(pp, c);
}

void ImageFolderListPopUp::On_mFolderSystemModel_rowsChanged(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(start);
    Q_UNUSED(end);

    if (!parent.isValid() || !mIsLoaded)
        return;
    emit contentsChanged();
}

void ImageFolderListPopUp::On_mTimer_timeout()
{
    if (QFileInfo::exists(mFolder))
    {
        if (mFolder == mTempFolder || mFolderSystemModel->rowCount(ui->mListView->rootIndex()) == 0)
            return;

        mIsLoaded = false;
        ui->mListView->setRootIndex(mFolderSystemModel->setRootPath(QString()));
        mIsLoaded = true;

        setFolder(mFolder);
        mTempFolder = mFolder;
        emit contentsChanged();
    }
    else
    {
        if (mTempFolder.isEmpty())
            return;
        mTempFolder = "";
        emit contentsChanged();
    }

}

void ImageFolderListPopUp::on_mListView_activated(const QModelIndex &index)
{
    emit fileSelected(mFolderSystemModel->filePath(index));
}

void ImageFolderListPopUp::on_mSliderZoom_valueChanged(int value)
{
    QFontMetrics fm(ui->mListView->font());

    ui->mListView->setGridSize(QSize(qMax<int>(value, 100), value + fm.height() * 2));
}
