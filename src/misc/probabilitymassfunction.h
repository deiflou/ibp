/****************************************************************************
**
** Copyright (C) 2015 Deif Lou
**
** This file is part of Anitools
**
** Anitools is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef ANITOOLS_MISC_PROBABILITYMASSFUNCTION_H
#define ANITOOLS_MISC_PROBABILITYMASSFUNCTION_H

#include <QMap>

namespace anitools {
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
#endif // ANITOOLS_MISC_PROBABILITYMASSFUNCTION_H
