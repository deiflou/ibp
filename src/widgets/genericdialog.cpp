#include "genericdialog.h"
#include "ui_genericdialog.h"

#ifdef Q_OS_WIN32
#include <QtWinExtras>
#include <winuser.h>
#include <dwmapi.h>
#endif

namespace anitools {
namespace widgets {

GenericDialog::GenericDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericDialog),
    mWidget(0)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN32
    mNativeEventFilter.registerMessage(WM_THEMECHANGED);
    mNativeEventFilter.registerMessage(WM_SYSCOLORCHANGE);
    mNativeEventFilter.registerMessage(WM_DWMCOMPOSITIONCHANGED);
    mNativeEventFilter.registerMessage(WM_DWMCOLORIZATIONCOLORCHANGED);
    mNativeEventFilter.registerMessage(WM_DWMNCRENDERINGCHANGED);
    connect(&mNativeEventFilter, SIGNAL(nativeEvent(void*,long*)),
            this, SLOT(On_mNativeEventFilter_nativeEvent(void*,long*)));
    On_mNativeEventFilter_nativeEvent(0, 0);
#endif
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

#ifdef Q_OS_WIN32
void GenericDialog::On_mNativeEventFilter_nativeEvent(void *message, long *result)
{
    Q_UNUSED(result)

    MSG * m = (MSG *)message;
    unsigned short lw;
    if (m)
        lw = LOWORD(m->message);
    else
        return;

    switch (lw)
    {
    default:
    {
#ifdef WIN_AERO
        if (QtWin::isCompositionEnabled())
        {
            ui->mSeparator->hide();
            ui->mContainerWidget->setAutoFillBackground(true);
            ui->mContainerButtons->layout()->setContentsMargins(5, 10, 5, 5);

            setAttribute(Qt::WA_TranslucentBackground);
            QtWin::extendFrameIntoClientArea(this, 0, 0, 0, ui->mContainerButtons->minimumSizeHint().height());
        }
        else
        {
            ui->mSeparator->show();
            ui->mContainerWidget->setAutoFillBackground(false);
            ui->mContainerButtons->layout()->setContentsMargins(10, 10, 10, 10);

            setAttribute(Qt::WA_TranslucentBackground, false);
            setAttribute(Qt::WA_NoSystemBackground, false);
            QtWin::extendFrameIntoClientArea(this, 0, 0, 0, 0);
        }
#endif
        break;
    }
    }
}
#endif

}}
