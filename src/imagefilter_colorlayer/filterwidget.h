#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

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
    void colorChanged(const QColor & v);
    void positionChanged(Filter::Position v);
    void blendModeChanged(BlendMode v);
    void opacityChanged(int v);

public slots:
    void setColor(const QColor & v);
    void setPosition(Filter::Position v);
    void setBlendMode(BlendMode v);
    void setOpacity(int v);

private slots:
    void on_mButtonColor_colorChanged(const QColor & c);
    void on_mButtonPositionFront_toggled(bool c);
    void on_mButtonPositionBehind_toggled(bool c);
    void on_mButtonPositionInside_toggled(bool c);
    void on_mComboBlendMode_blendModeChanged(BlendMode m);
    void on_mSliderOpacity_valueChanged(int value);
    void on_mSpinOpacity_valueChanged(int arg1);
};

#endif // FILTERWIDGET_H
