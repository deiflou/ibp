//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef IBP_WIDGETS_HSLCOLORREPLACEMENTCURVESPAINTDELEGATE_H
#define IBP_WIDGETS_HSLCOLORREPLACEMENTCURVESPAINTDELEGATE_H

#include <QImage>
#include "curves.h"
#include "../imgproc/types.h"

using namespace ibp::imgproc;

namespace ibp {
namespace widgets {

class HSLColorReplacementCurvesPaintDelegate : public CurvesPaintDelegate
{
    Q_OBJECT

public:
    explicit HSLColorReplacementCurvesPaintDelegate(QObject *parent = 0);
    ~HSLColorReplacementCurvesPaintDelegate();

    void update(UpdateEvent e, const Curves * w, const QRect & r);
    void paint(QPainter & p, const Curves * w, const QRect & r,
               QStyle::State widgetState, const QPolygonF & graph,
               const QVector<QPointF> & knotPos, const QVector<QStyle::State> & knotStates,
               const QSize & knotSize);
    QRect graphRect(const QRect & r) const;

    HSL color() const;
    int correctedValue(ColorChannel channel) const;
    ColorChannel channel() const;
    bool isInverted() const;
    bool colorize() const;
    void setColor(const HSL & color);
    void setCorrectedValues(int h, int s, int l);
    void setCorrectedValue(ColorChannel channel, int v);
    void setChannel(ColorChannel channel);
    void setInverted(bool v);
    void setColorize(bool v);


private:
    QImage mCheckerboardImage, mGradientImage, mCorrectedGradientImage;
    HSL mColor;
    int mCorrectedHue, mCorrectedSaturation, mCorrectedLightness;
    ColorChannel mChannel;
    bool mIsInverted;
    bool mColorize;

    void paintBackground(QPainter &p, const Curves * w, const QRect & r, QStyle::State widgetState);
    void paintGraph(const QPolygonF & g, QPainter & p, const Curves * w, const QRect & r,
                    QStyle::State widgetState);
    void paintKnots(const QVector<QPointF> & pts, const QVector<QStyle::State> & sts,
                    const QSize & s, QPainter & p, const Curves * w, const QRect & r,
                    QStyle::State widgetState);

    void makeGradientImage();
};

}}

#endif // IBP_WIDGETS_HSLCOLORREPLACEMENTCURVESPAINTDELEGATE_H
