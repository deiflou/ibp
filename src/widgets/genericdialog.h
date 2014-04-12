#ifndef ANITOOLS_WIDGETS_GENERICDIALOG_H
#define ANITOOLS_WIDGETS_GENERICDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

#ifdef Q_OS_WIN32
#include "../misc/nativeeventfilter.h"
#endif

namespace anitools {
namespace widgets {

#ifdef Q_OS_WIN32
using namespace anitools::misc;
#endif

namespace Ui {
class GenericDialog;
}

class GenericDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GenericDialog(QWidget *parent = 0);
    ~GenericDialog();

    QDialogButtonBox * buttonBox() const;
    QWidget * widget() const;
    void setWidget(QWidget * w);

private:
    Ui::GenericDialog *ui;
    QWidget * mWidget;

#ifdef Q_OS_WIN32
    NativeEventFilter mNativeEventFilter;
#endif

private slots:
#ifdef Q_OS_WIN32
    void On_mNativeEventFilter_nativeEvent(void * message, long * result);
#endif

};


}}
#endif // ANITOOLS_WIDGETS_GENERICDIALOG_H
