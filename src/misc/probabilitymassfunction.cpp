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

#include <math.h>

#include "probabilitymassfunction.h"

namespace anitools {
namespace misc {

ProbabilityMassFunction::ProbabilityMassFunction() :
    mLimitA(0.), mLimitB(0.), mUseCustomLimits(false)
{
    clearResponses();
}

ProbabilityMassFunction::~ProbabilityMassFunction()
{
}

void ProbabilityMassFunction::addResponse(double r, double w)
{
    if (w < 0. || qFuzzyIsNull(w))
        return;
    if (mResponses.contains(r))
        mResponses[r] += w;
    else
    {
        mResponses.insert(r, w);
        if (!mUseCustomLimits)
        {
            if (mResponses.size() == 1)
                mLimitA = mLimitB = r;
            else if (r < mLimitA)
                mLimitA = r;
            else if (r > mLimitB)
                mLimitB = r;
        }
    }
    mNResponses += w;
    mIsDirty = true;
}

void ProbabilityMassFunction::removeResponse(double r, double w)
{
    if (w < 0. || qFuzzyIsNull(w))
        return;
    if (mResponses.contains(r))
    {
        mResponses[r] -= w;
        double v = mResponses[r];
        if (v < 0. || qFuzzyIsNull(v))
        {
            mResponses.remove(r);
            if (!mUseCustomLimits)
            {
                if (mResponses.size() == 1)
                    mLimitA = mLimitB = mResponses.firstKey();
                else if (mLimitA < mResponses.firstKey())
                    mLimitA = mResponses.firstKey();
                else if (mLimitB > mResponses.lastKey())
                    mLimitB = mResponses.lastKey();
            }
        }
        mNResponses -= w;
        mIsDirty = true;
    }
}

void ProbabilityMassFunction::clearResponses()
{
    mResponses.clear();
    mProbabilities.clear();
    mMoments.clear();
    mCentralMoments.clear();
    mStandardisedMoments.clear();
    mNResponses = 0.;
    mStandardDeviation = mAverageDeviation = mExcessKurtosis = mMedian = mMode = qQNaN();
    mIsDirty = true;
}


double ProbabilityMassFunction::responses(double r)
{
    if (mResponses.contains(r))
        return mResponses.value(r);
    return 0.;
}

double ProbabilityMassFunction::probability(double r)
{
    if (mResponses.contains(r))
    {
        if (mIsDirty)
            update();
        return mProbabilities.value(r);
    }
    return 0.;
}

double ProbabilityMassFunction::size()
{
    return mNResponses;
}

double ProbabilityMassFunction::mean()
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (mIsDirty)
        update();
    return mMoments.value(1);
}

double ProbabilityMassFunction::variance()
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (mIsDirty)
        update();
    return mCentralMoments.value(2);
}

double ProbabilityMassFunction::standardDeviation()
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (mIsDirty)
        update();
    return mStandardDeviation;
}

double ProbabilityMassFunction::averageDeviation()
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (mIsDirty)
        update();
    return mAverageDeviation;
}

double ProbabilityMassFunction::skewness()
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (mIsDirty)
        update();
    return mStandardisedMoments.value(3);
}

double ProbabilityMassFunction::kurtosis()
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (mIsDirty)
        update();
    return mStandardisedMoments.value(4);
}

double ProbabilityMassFunction::excessKurtosis()
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (mIsDirty)
        update();
    return mExcessKurtosis;
}

double ProbabilityMassFunction::median()
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (mIsDirty)
        update();
    return mMedian;
}

double ProbabilityMassFunction::mode()
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (mIsDirty)
        update();
    return mMode;
}

double ProbabilityMassFunction::moment(int n)
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (n < 0 || n > kMaximumMoment)
        return qQNaN();
    if (mIsDirty)
        update();
    if (mMoments.contains(n))
        return mMoments.value(n);

    double m = 0.;
    QMapIterator<double, double> i(mResponses);
    while (i.hasNext())
    {
        i.next();
        m += pow(i.key(), n) * mProbabilities.value(i.key());
    }
    mMoments.insert(n, m);
    return m;
}

double ProbabilityMassFunction::centralMoment(int n)
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (n < 0 || n > kMaximumMoment)
        return qQNaN();
    if (mIsDirty)
        update();
    if (mCentralMoments.contains(n))
        return mCentralMoments.value(n);

    double m = 0.;
    QMapIterator<double, double> i(mResponses);
    const double moment1 = mMoments.value(1);
    while (i.hasNext())
    {
        i.next();
        m += pow(i.key() - moment1, n) * mProbabilities.value(i.key());
    }
    mCentralMoments.insert(n, m);
    return m;
}

double ProbabilityMassFunction::standardisedMoment(int n)
{
    if (qFuzzyIsNull(mNResponses))
        return qQNaN();
    if (n < 0 || n > kMaximumMoment)
        return qQNaN();
    if (mIsDirty)
        update();
    if (mStandardisedMoments.contains(n))
        return mStandardisedMoments.value(n);

    double m = 0.;
    QMapIterator<double, double> i(mResponses);
    const double moment1 = mMoments.value(1);
    while (i.hasNext())
    {
        i.next();
        m += pow(i.key() - moment1, n) * mProbabilities.value(i.key());
    }
    mCentralMoments.insert(n, m);
    m /= pow(mStandardDeviation, n);
    mStandardisedMoments.insert(n, m);
    return m;
}

void ProbabilityMassFunction::setLimits(double a, double b)
{
    mUseCustomLimits = true;
    if (qFuzzyCompare(a, mLimitA) && qFuzzyCompare(b, mLimitB))
        return;
    mLimitA = a;
    mLimitB = b;
    mIsDirty = true;
}

void ProbabilityMassFunction::setLimit(ProbabilityMassFunction::Limit l, double v)
{
    mUseCustomLimits = true;
    if ((l == A && qFuzzyCompare(mLimitA, v)) || (l == B && qFuzzyCompare(mLimitB, v)))
        return;
    if (l == A)
        mLimitA = v;
    else
        mLimitB = v;
    mIsDirty = true;
}

double ProbabilityMassFunction::limit(ProbabilityMassFunction::Limit l)
{
    return l == A ? mLimitA : mLimitB;
}

void ProbabilityMassFunction::clearLimits()
{
    mUseCustomLimits = false;
    if (mResponses.size() > 0)
    {
        if (qFuzzyCompare(mResponses.firstKey(), mLimitA) &&
            qFuzzyCompare(mResponses.lastKey(), mLimitB))
            return;
        mLimitA = mResponses.firstKey();
        mLimitB = mResponses.lastKey();
        mIsDirty = true;
    }
}

double ProbabilityMassFunction::integrate()
{
    return integrate(mLimitA, mLimitB);
}

double ProbabilityMassFunction::integrate(double a, double b)
{
    if (mNResponses == 0 || qFuzzyCompare(a, b))
        return 0.;

    // swap
    if (a > b)
    {
        double c = a;
        a = b;
        b = c;
    }

    // integrate
    if (a > mResponses.lastKey() || b < mResponses.firstKey())
        return 0.;

    if (mIsDirty)
        update();

    QMap<double, double>::const_iterator aI = mResponses.lowerBound(a);
    QMap<double, double>::const_iterator bI = mResponses.upperBound(b);
    double result = 0.;
    while (aI != bI)
    {
        result += mProbabilities.value(aI.key());
        aI++;
    }

    return result;
}

double ProbabilityMassFunction::integrate2(double b)
{
    return integrate2(b < 0. ? mLimitB : mLimitA, b);
}

double ProbabilityMassFunction::integrate2(double a, double b)
{
    if (mNResponses == 0)
        return qQNaN();

    // integrate
    if ((a < mResponses.firstKey() && b < 0.) || (a > mResponses.lastKey() && b > 0.) ||
        (qFuzzyIsNull(b)) || (fabs(b) > 1.))
        return qQNaN();

    if (mIsDirty)
        update();

    double result = qQNaN(), c = 0.;
    if (b > 0.)
    {
        QMap<double, double>::const_iterator aI = mResponses.lowerBound(a);
        while (aI != mResponses.end())
        {
            c += mProbabilities.value(aI.key());
            if (c > b)
                break;
            result = aI.key();
            aI++;
        }
        if (qIsNaN(result))
            result = a;
    }
    else
    {
        QMap<double, double>::const_iterator aI = mResponses.upperBound(a);
        aI--;
        while (true)
        {
            c -= mProbabilities.value(aI.key());
            if (c < b)
                break;
            result = aI.key();
            if (aI == mResponses.begin())
                break;
            aI--;
        }
        if (qIsNaN(result))
            result = a;
    }

    return result;
}

void ProbabilityMassFunction::update()
{
    // This function computes the probabilities, the 1st moment (mean),
    // the median, the mode, the 1st and 2nd (variance) central moments,
    // the standard deviation, the average deviation, the 1st, 2nd, 3rd
    // (skewness), 4th (kurtosis) standardised moments and the excess
    // kurtosis

    if (qFuzzyIsNull(mNResponses))
        return;

    QMap<double, double>::const_iterator begin = mResponses.begin();
    QMap<double, double>::const_iterator end = mResponses.end();
    QMap<double, double>::const_iterator lA = mResponses.lowerBound(qMin<double>(mLimitA, mLimitB));
    QMap<double, double>::const_iterator lB = mResponses.lowerBound(qMax<double>(mLimitA, mLimitB));
    QMap<double, double>::const_iterator lB2 = mResponses.upperBound(qMax<double>(mLimitA, mLimitB));
    if (lB == end)
        lB--;
    QMap<double, double>::const_iterator i = begin;

    mProbabilities.clear();
    mMoments.clear();
    mCentralMoments.clear();
    mStandardisedMoments.clear();
    mStandardDeviation = mAverageDeviation = mExcessKurtosis = mMedian;
    mMode = mResponses.firstKey();

    mMoments.insert(1, 0.);
    mCentralMoments.insert(1, 0.);
    mCentralMoments.insert(2, 0.);
    mStandardisedMoments.insert(1, 0.);
    mStandardisedMoments.insert(2, 1.);
    mStandardisedMoments.insert(3, 0.);
    mStandardisedMoments.insert(4, 0.);

    // Probabilities
    double p, p2 = 0., cum = 0.;
    while (i != end)
    {
        // probabilities
        p = i.value() / mNResponses;
        mProbabilities.insert(i.key(), p);

        if (i.key() >= lA.key() && i.key() <= lB.key())
            cum += i.value();

        i++;
    }

    i = lA;
    // mean, mode, median
    bool medianSet = false;
    mMode = lA.key();
    while (i != lB2)
    {
        p = i.value() / cum;
        // mean
        mMoments[1] += i.key() * p;
        // mode
        if (i.value() > mResponses.value(mMode))
            mMode = i.key();
        // median
        if (!medianSet)
        {
            p2 += p;
            if (p2 >= .5)
            {
                mMedian = i.key();
                medianSet = true;
            }
        }
        i++;
    }

    // variance, standard deviation, skewness, kurtosis,
    // excess kurtosis, average deviation
    double delta;
    const double m = mMoments.value(1);
    i = lA;
    while (i != lB2)
    {
        delta = i.key() - m;
        p = i.value() / cum;
        //p = mProbabilities.value(i.key()) / cum;
        // variance
        mCentralMoments[2] += delta * delta * p;
        // skewness
        mStandardisedMoments[3] += mCentralMoments.value(2) * delta;
        // kurtosis
        mStandardisedMoments[4] += mStandardisedMoments.value(3) * delta;
        // average deviation
        mAverageDeviation += fabs(delta) * p;
        i++;
    }
    mStandardDeviation = sqrt(mCentralMoments.value(2));
    mStandardisedMoments[3] /= (mCentralMoments.value(2) * mStandardDeviation);
    mStandardisedMoments[4] /= (mCentralMoments.value(2) * mCentralMoments.value(2));
    mExcessKurtosis = mStandardisedMoments.value(4) - 3.;

    mIsDirty = false;
}

}}
