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

#ifndef IBP_MISC_PROBABILITYMASSFUNCTION_H
#define IBP_MISC_PROBABILITYMASSFUNCTION_H

#include <QMap>

namespace ibp {
namespace misc {

class ProbabilityMassFunction
{
public:
    enum Limit
    {
        A, B
    };

    ProbabilityMassFunction();
    ~ProbabilityMassFunction();

    void addResponse(double r, double w = 1.);
    void removeResponse(double r, double w = 1.);
    void clearResponses();

    double responses(double r);
    double probability(double r);
    double size();

    double mean();
    double variance();
    double standardDeviation();
    double averageDeviation();
    double skewness();
    double kurtosis();
    double excessKurtosis();
    double median();
    double mode();
    double moment(int n);
    double centralMoment(int n);
    double standardisedMoment(int n);

    void setLimits(double a, double b);
    void setLimit(Limit l, double v);
    double limit(Limit l);
    void clearLimits();

    double integrate();
    double integrate(double a, double b);
    double integrate2(double b);
    double integrate2(double a, double b);

private:
    static const int kMaximumMoment = 5;

    double mNResponses;
    QMap<double, double> mResponses, mProbabilities;
    QMap<int, double> mMoments, mCentralMoments, mStandardisedMoments;
    double mStandardDeviation, mAverageDeviation, mExcessKurtosis, mMedian, mMode;
    double mLimitA, mLimitB;
    bool mUseCustomLimits;
    bool mIsDirty;

    void update();
};

}}
#endif // IBP_MISC_PROBABILITYMASSFUNCTION_H
