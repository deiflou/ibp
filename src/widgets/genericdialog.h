#ifndef ANITOOLS_WIDGETS_GENERICDIALOG_H
#define ANITOOLS_WIDGETS_GENERICDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

namespace anitools {
namespace widgets {

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
};


}}
#endif // ANITOOLS_WIDGETS_GENERICDIALOG_H
