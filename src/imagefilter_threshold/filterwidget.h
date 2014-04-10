#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QSlider>
#include <QSpinBox>

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

    QToolButton * mButtonAffectedChannel[5];
    QSpinBox * mSpinThreshold[5];

signals:
    void rgbModeChanged(int m);
    void affectedChannelChanged(int c, bool a);
    void thresholdChanged(int c, int t);

public slots:
    void setRGBMode(int m);
    void setAffectedChannel(int c, bool a);
    void setThreshold(int c, int t);

private slots:
    void on_mButtonRGBModeTight_toggled(bool checked);
    void on_mButtonRGBModeIndependent_toggled(bool checked);
    void on_mButtonAffectedChannelsRGB_toggled(bool checked);
    void on_mButtonAffectedChannelsRed_toggled(bool checked);
    void on_mButtonAffectedChannelsGreen_toggled(bool checked);
    void on_mButtonAffectedChannelsBlue_toggled(bool checked);
    void on_mButtonAffectedChannelsAlpha_toggled(bool checked);
    void on_mSliderRGB_valueChanged(int value);
    void on_mSpinRGB_valueChanged(int arg1);
    void on_mSliderRed_valueChanged(int value);
    void on_mSpinRed_valueChanged(int arg1);
    void on_mSliderGreen_valueChanged(int value);
    void on_mSpinGreen_valueChanged(int arg1);
    void on_mSliderBlue_valueChanged(int value);
    void on_mSpinBlue_valueChanged(int arg1);
    void on_mSliderAlpha_valueChanged(int value);
    void on_mSpinAlpha_valueChanged(int arg1);
};

#endif // FILTERWIDGET_H
