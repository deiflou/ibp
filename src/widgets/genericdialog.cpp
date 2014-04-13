#include "genericdialog.h"
#include "ui_genericdialog.h"

namespace anitools {
namespace widgets {

GenericDialog::GenericDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericDialog),
    mWidget(0)
{
    ui->setupUi(this);
}

GenericDialog::~GenericDialog()
{
    delete ui;

    if (mWidget)
        delete mWidget;
}

QDialogButtonBox *GenericDialog::buttonBox() const
{
    return ui->mButtonBox;
}

QWidget *GenericDialog::widget() const
{
    return mWidget;
}

void GenericDialog::setWidget(QWidget *w)
{
    if (mWidget)
        delete mWidget;
    mWidget = w;
    if (!mWidget)
        return;
    mWidget->setParent(this);
    ui->mContainerWidget->layout()->addWidget(mWidget);
    setWindowTitle(mWidget->windowTitle());
}

}}
