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

#include <QMenu>
#include <QDoubleSpinBox>

#include "affinetransformationlist.h"
#include "ui_affinetransformationlist.h"
#include "rotationgauge.h"

namespace anitools {
namespace widgets {

AffineTransformationList::AffineTransformationList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AffineTransformationList)
{
    ui->setupUi(this);

    ui->mWidgetList->setItemMargins(QMargins(0, 0, 0, 0));
    ui->mWidgetList->setItemWidgetMargins(QMargins(5, 0, 0, 5));
    ui->mWidgetList->setEmptyMessage("");
    ui->mWidgetList->setPlaceholderVisible(false);
    ui->mWidgetList->setItemContentsFrameVisible(false);
    ui->mWidgetList->setItemShadowVisible(false);

    QMenu * m = new QMenu(this);
    QAction * a = new QAction(tr("Translation"), this);
    a->setProperty("type", (int)Translation);
    connect(a, SIGNAL(triggered()), this, SLOT(On_mButtonAppend_menuActionClicked()));
    m->addAction(a);
    a = new QAction(tr("Scaling"), this);
    a->setProperty("type", (int)Scaling);
    connect(a, SIGNAL(triggered()), this, SLOT(On_mButtonAppend_menuActionClicked()));
    m->addAction(a);
    a = new QAction(tr("Rotation"), this);
    a->setProperty("type", (int)Rotation);
    connect(a, SIGNAL(triggered()), this, SLOT(On_mButtonAppend_menuActionClicked()));
    m->addAction(a);
    a = new QAction(tr("Shearing"), this);
    a->setProperty("type", (int)Shearing);
    connect(a, SIGNAL(triggered()), this, SLOT(On_mButtonAppend_menuActionClicked()));
    m->addAction(a);

    ui->mButtonAppend->setText(tr("Append New Transformation"));
    ui->mButtonAppend->setPopupMode(QToolButton::InstantPopup);
    ui->mButtonAppend->setMenu(m);
    ui->mButtonAppend->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

AffineTransformationList::~AffineTransformationList()
{
    delete ui;
}

QList<AffineTransformation> AffineTransformationList::transformations() const
{
    return mTransformations;
}

QList<bool> AffineTransformationList::bypasses() const
{
    return mBypasses;
}

void AffineTransformationList::appendTransformation(const AffineTransformation &t)
{
    QString title;
    QWidget * mainWidget = new QWidget(ui->mWidgetList);
    QHBoxLayout * mainLayout = new QHBoxLayout(mainWidget);
    QDoubleSpinBox * spin1, * spin2;
    RotationGauge * rot;

    mainWidget->setProperty("type", (int)t.type);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);
    mainLayout->addStretch(1);

    if (t.type == Rotation)
    {
        title = tr("Rotate");
        rot = new RotationGauge(mainWidget);
        rot->setFocusPolicy(Qt::StrongFocus);
        spin1 = new QDoubleSpinBox(mainWidget);
        spin1->setRange(0, 360);
        spin1->setSuffix("°");
        spin1->setValue(t.z);
        mainLayout->addWidget(rot);
        mainLayout->addWidget(spin1);
        connect(rot, SIGNAL(angleChanged(double)), spin1, SLOT(setValue(double)));
        connect(spin1, SIGNAL(valueChanged(double)), rot, SLOT(setAngle(double)));
        connect(spin1, SIGNAL(valueChanged(double)), this, SLOT(On_valueChanged(double)));
    }
    else
    {
        if (t.type == Translation)
            title = tr("Translate");
        else if (t.type == Scaling)
            title = tr("Scale");
        else
            title = tr("Shear");
        spin1 = new QDoubleSpinBox(mainWidget);
        spin2 = new QDoubleSpinBox(mainWidget);
        spin1->setRange(-1000, 1000);
        spin2->setRange(-1000, 1000);
        spin1->setPrefix("X: ");
        spin2->setPrefix("Y: ");
        spin1->setSuffix(t.type == Translation ? "px" : "%");
        spin2->setSuffix(t.type == Translation ? "px" : "%");
        spin1->setValue(t.x);
        spin2->setValue(t.y);
        mainLayout->addWidget(spin1);
        mainLayout->addWidget(spin2);
        connect(spin1, SIGNAL(valueChanged(double)), this, SLOT(On_valueChanged(double)));
        connect(spin2, SIGNAL(valueChanged(double)), this, SLOT(On_valueChanged(double)));
    }

    mainLayout->addStretch(1);

    ui->mWidgetList->append(mainWidget);
    ui->mWidgetList->setTitle(ui->mWidgetList->count() - 1, title);
}

void AffineTransformationList::setTransformations(const QList<AffineTransformation> &t)
{
    ui->mWidgetList->clear();
    for (int i = 0; i < t.size(); i++)
        appendTransformation(t.at(i));
}

void AffineTransformationList::setBypasses(const QList<bool> &b)
{
    if (b.size() != mTransformations.size())
        return;
    for (int i = 0; i < b.size(); i++)
        ui->mWidgetList->setWidgetBypass(i, b.at(i));
}

void AffineTransformationList::On_mButtonAppend_menuActionClicked()
{
    AffineTransformationType type = (AffineTransformationType)sender()->property("type").toInt();

    if (type == Translation)
        appendTransformation(AffineTransformation(Translation, 0, 0, 0));
    else if (type == Scaling)
        appendTransformation(AffineTransformation(Scaling, 100, 100, 100));
    else if (type == Rotation)
        appendTransformation(AffineTransformation(Rotation, 0, 0, 0));
    else
        appendTransformation(AffineTransformation(Shearing, 0, 0, 0));
}

void AffineTransformationList::On_valueChanged(double v)
{
    QWidget * s = qobject_cast<QWidget *>(sender());
    QWidget * sp = qobject_cast<QWidget *>(s->parent());
    int i, j;

    for (i = 0; i < ui->mWidgetList->count(); i++)
        if (ui->mWidgetList->at(i) == sp)
            break;

    j = sp->layout()->indexOf(s) - (mTransformations.at(i).type == Rotation ? 0 : 1);
    if (j == 0)
        mTransformations[i].x = v;
    else if (j == 1)
        mTransformations[i].y = v;
    else
        mTransformations[i].z = v;

    emit transformationsChanged();
}

void AffineTransformationList::on_mWidgetList_widgetInserted(int i)
{
    AffineTransformationType type = (AffineTransformationType)ui->mWidgetList->at(i)->property("type").toInt();
    AffineTransformation tfm(type, 0, 0, 0);
    QDoubleSpinBox * spin;

    if (type == Rotation)
    {
        spin = (QDoubleSpinBox *)ui->mWidgetList->at(i)->layout()->itemAt(2)->widget();
        tfm.z = spin->value();
    }
    else
    {
        spin = (QDoubleSpinBox *)ui->mWidgetList->at(i)->layout()->itemAt(1)->widget();
        tfm.x = spin->value();
        spin = (QDoubleSpinBox *)ui->mWidgetList->at(i)->layout()->itemAt(2)->widget();
        tfm.y = spin->value();
    }

    mTransformations.insert(i, tfm);
    mBypasses.insert(i, ui->mWidgetList->widgetBypass(i));

    emit transformationsChanged();
}

void AffineTransformationList::on_mWidgetList_widgetMoved(int from, int to)
{
    mTransformations.move(from, to);
    mBypasses.move(from, to);

    emit transformationsChanged();
}

void AffineTransformationList::on_mWidgetList_widgetRemoved(int i)
{
    mTransformations.removeAt(i);
    mBypasses.removeAt(i);

    emit transformationsChanged();
}

void AffineTransformationList::on_mWidgetList_widgetBypassStatusChanged(int i, bool c)
{
    mBypasses[i] = c;

    emit transformationsChanged();
}

}}

