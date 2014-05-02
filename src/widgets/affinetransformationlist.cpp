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

#include <QMenu>
#include <QDoubleSpinBox>
#include <QDebug>

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
    QAction * a = new QAction(tr("Translate"), this);
    a->setProperty("type", "translate");
    connect(a, SIGNAL(triggered()), this, SLOT(On_mButtonAppend_menuActionClicked()));
    m->addAction(a);
    a = new QAction(tr("Scale"), this);
    a->setProperty("type", "scale");
    connect(a, SIGNAL(triggered()), this, SLOT(On_mButtonAppend_menuActionClicked()));
    m->addAction(a);
    a = new QAction(tr("Rotate"), this);
    a->setProperty("type", "rotate");
    connect(a, SIGNAL(triggered()), this, SLOT(On_mButtonAppend_menuActionClicked()));
    m->addAction(a);
    a = new QAction(tr("Shear"), this);
    a->setProperty("type", "shear");
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

void AffineTransformationList::On_mButtonAppend_menuActionClicked()
{
    QString type = sender()->property("type").toString();

    QString title;
    QWidget * mainWidget = new QWidget(ui->mWidgetList);
    QHBoxLayout * mainLayout = new QHBoxLayout(mainWidget);
    QDoubleSpinBox * spin1, * spin2;
    RotationGauge * rot;
    Transformation tfm;

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);
    mainLayout->addStretch(1);

    ui->mWidgetList->append(mainWidget);

    if (type == "scale")
        tfm.v1 = tfm.v2 = 100;
    else
        tfm.v1 = tfm.v2 = 0;
    tfm.bypass = false;

    if (type == "rotate")
    {
        rot = new RotationGauge(mainWidget);
        rot->setFocusPolicy(Qt::StrongFocus);
        spin1 = new QDoubleSpinBox(mainWidget);
        spin1->setRange(0, 360);
        spin1->setSuffix("°");
        mainLayout->addWidget(rot);
        mainLayout->addWidget(spin1);
        connect(rot, SIGNAL(angleChanged(double)), spin1, SLOT(setValue(double)));
        connect(spin1, SIGNAL(valueChanged(double)), rot, SLOT(setAngle(double)));
        connect(spin1, SIGNAL(valueChanged(double)), this, SLOT(On_valueChanged(double)));
    }
    else
    {
        spin1 = new QDoubleSpinBox(mainWidget);
        spin2 = new QDoubleSpinBox(mainWidget);
        spin1->setRange(-1000, 1000);
        spin2->setRange(-1000, 1000);
        spin1->setPrefix("X: ");
        spin2->setPrefix("Y: ");
        spin1->setSuffix(type == "translate" ? "px" : "%");
        spin2->setSuffix(type == "translate" ? "px" : "%");
        spin1->setValue(type == "scale" ? 100 : 0);
        spin2->setValue(type == "scale" ? 100 : 0);
        mainLayout->addWidget(spin1);
        mainLayout->addWidget(spin2);
        connect(spin1, SIGNAL(valueChanged(double)), this, SLOT(On_valueChanged(double)));
        connect(spin2, SIGNAL(valueChanged(double)), this, SLOT(On_valueChanged(double)));
    }

    if (type == "translate")
    {
        title = tr("Translate");
        tfm.type = Translation;
    }
    else if (type == "scale")
    {
        title = tr("Scale");
        tfm.type = Scaling;
    }
    else if (type == "rotate")
    {
        title = tr("Rotate");
        tfm.type = Rotation;
    }
    else
    {
        title = tr("Shear");
        tfm.type = Shearing;
    }

    mainLayout->addStretch(1);
    mTransformations.append(tfm);
    ui->mWidgetList->setTitle(ui->mWidgetList->count() - 1, title);
}

QDebug operator<<(QDebug d, const AffineTransformationList::Transformation &tfm)
{
    d << tfm.type << " - " << tfm.v1 << " - " << tfm.v2 << " - " << tfm.bypass;
    return d;
}

void AffineTransformationList::On_valueChanged(double v)
{
    QWidget * s = qobject_cast<QWidget *>(sender());
    QWidget * sp = qobject_cast<QWidget *>(s->parent());
    int i, j;

    for (i = 0; i < ui->mWidgetList->count(); i++)
        if (ui->mWidgetList->at(i) == sp)
            break;

    j = sp->layout()->indexOf(s) - (mTransformations.at(i).type == Rotation ? 2 : 1);
    if (j == 0)
        mTransformations[i].v1 = v;
    else
        mTransformations[i].v2 = v;

    emit transformationChanged(mTransformations);
}

}}

