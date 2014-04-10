#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>

#include "../imgproc/imagefilter.h"
#include "../imgproc/types.h"

using namespace anitools::imgproc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    enum Position
    {
        Front, Behind, Inside
    };

    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    QColor mColor;
    Position mPosition;
    BlendMode mBlendMode;
    int mOpacity;

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
};

#endif // FILTER_H
