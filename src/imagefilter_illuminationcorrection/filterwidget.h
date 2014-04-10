#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QImage>

#include "filter.h"

namespace Ui {
class FilterWidget;
}

class FilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget *parent = 0);
    ~FilterWidget();

private:
    Ui::FilterWidget *ui;
    bool mEmitSignals;

signals:
    void modeChanged(int m);
    void imageChanged(const QImage & i);

public slots:
    void setMode(int m);
    void setImage(const QImage & i);

private slots:
    void on_mButtonAuto_toggled(bool c);
    void on_mButtonFromImage_toggled(bool c);
    void on_mButtonImage_clicked();
};

#endif // FILTERWIDGET_H
