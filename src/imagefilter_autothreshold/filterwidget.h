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

signals:
    void thresholdModeChanged(int m);
    void rgbModeChanged(int m);
    void affectedChannelChanged(int c, bool a);

public slots:
    void setThresholdMode(int m);
    void setRGBMode(int m);
    void setAffectedChannel(int c, bool a);

private slots:
    void on_mButtonThresholdModeGlobal_toggled(bool checked);
    void on_mButtonThresholdModeLocal_toggled(bool checked);
    void on_mButtonRGBModeTight_toggled(bool checked);
    void on_mButtonRGBModeIndependent_toggled(bool checked);
    void on_mButtonAffectedChannelsRGB_toggled(bool checked);
    void on_mButtonAffectedChannelsRed_toggled(bool checked);
    void on_mButtonAffectedChannelsGreen_toggled(bool checked);
    void on_mButtonAffectedChannelsBlue_toggled(bool checked);
    void on_mButtonAffectedChannelsAlpha_toggled(bool checked);
};

#endif // FILTERWIDGET_H
