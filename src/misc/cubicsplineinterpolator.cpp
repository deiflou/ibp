/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
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

#include <Eigen/Sparse>
#include <vector>

#include "cubicsplineinterpolator.h"

namespace anitools {
namespace misc {

CubicSplineInterpolator::CubicSplineInterpolator() :
    BaseSplineInterpolator(),
    mFloorBoundaryConditions(BoundaryConditions_Natural),
    mCeilBoundaryConditions(BoundaryConditions_Natural),
    mFloorBoundaryConditionsValue(0.),
    mCeilBoundaryConditionsValue(0.),
    mIsDirty(true)
{
}

SplineInterpolator * CubicSplineInterpolator::clone() const
{
    CubicSplineInterpolator * si = new CubicSplineInterpolator();
    if (!si)
        return 0;
    si->mKnots = mKnots;
    si->mFloorExtrapolationMode = mFloorExtrapolationMode;
    si->mCeilExtrapolationMode = mCeilExtrapolationMode;
    si->mFloorExtrapolationValue = mFloorExtrapolationValue;
    si->mCeilExtrapolationValue = mCeilExtrapolationValue;
    si->mFloorBoundaryConditions = mFloorBoundaryConditions;
    si->mCeilBoundaryConditions = mCeilBoundaryConditions;
    si->mFloorBoundaryConditionsValue = mFloorBoundaryConditionsValue;
    si->mCeilBoundaryConditionsValue = mCeilBoundaryConditionsValue;
    si->mCoefficients = mCoefficients;
    si->mIsDirty = mIsDirty;
    return si;
}

double CubicSplineInterpolator::f(double x)
{
    if (mKnots.size() < 1) return 0.0;

    if (mIsDirty) calculateCoefficients();

    if (x < mKnots.first().x())
    {
        if (mFloorExtrapolationMode == ExtrapolationMode_Constant)
            return mFloorExtrapolationValue;
        else if (mFloorExtrapolationMode == ExtrapolationMode_FollowTangent && mKnots.size() > 1)
            return mCoefficients.first().c * (x - mKnots[0].x()) + mKnots[0].y();
        else if (mFloorExtrapolationMode == ExtrapolationMode_Repeat && mKnots.size() > 1)
            x = fmod(x - mKnots.first().x(), mKnots.last().x() - mKnots.first().x()) + mKnots.last().x();
        else if (mFloorExtrapolationMode == ExtrapolationMode_Mirror && mKnots.size() > 1)
        {
            const double a = (mKnots.last().x() - mKnots.first().x()) * 2.;
            const double b = (x - mKnots.first().x()) / a;
            x = fabs(b - floor(b + .5)) * a + mKnots.first().x();
        }
        else
            return mKnots.first().y();
    }
    else if (x > mKnots.last().x())
    {
        if (mCeilExtrapolationMode == ExtrapolationMode_Constant)
            return mCeilExtrapolationValue;
        else if (mCeilExtrapolationMode == ExtrapolationMode_FollowTangent && mKnots.size() > 1)
        {
            const double lkp = mKnots.last().x() - mKnots[mKnots.size() - 2].x();
            return ((3. * mCoefficients.last().a * lkp + 2. * mCoefficients.last().b) * lkp + mCoefficients.last().c) *
                   (x - mKnots.last().x()) + mKnots.last().y();
        }
        else if (mCeilExtrapolationMode == ExtrapolationMode_Repeat && mKnots.size() > 1)
            x = fmod(x - mKnots.first().x(), mKnots.last().x() - mKnots.first().x()) + mKnots.first().x();
        else if (mCeilExtrapolationMode == ExtrapolationMode_Mirror && mKnots.size() > 1)
        {
            const double a = (mKnots.last().x() - mKnots.first().x()) * 2.;
            const double b = (x - mKnots.first().x()) / a;
            x = fabs(b - floor(b + .5)) * a + mKnots.first().x();
        }
        else
            return mKnots.last().y();
    }

    int piece = getPiece(x);

    double w = x - mKnots[piece].x();
    return ((mCoefficients[piece].a * w + mCoefficients[piece].b) * w +
            mCoefficients[piece].c) * w + mCoefficients[piece].d;
}

int CubicSplineInterpolator::getPiece(double x) const
{
    for (int i = 1; i < mKnots.size(); i++)
        if (x < mKnots[i].x())
            return i - 1;
    return mKnots.size() - 2;
}

void CubicSplineInterpolator::calculateCoefficients()
{
    if (mKnots.size() < 1) return;

    register int kSize = mKnots.size();
    bool isPeriodic = mFloorBoundaryConditions == BoundaryConditions_Periodic &&
                      mCeilBoundaryConditions == BoundaryConditions_Periodic;

    mCoefficients.clear();
    mCoefficients.resize(kSize - 1);

    if (kSize == 1)
    {
        // ------------------------------------------
        // constant, the final value is extrapolated
        // from the unique knot
        // ------------------------------------------
        mCoefficients[0].a = mCoefficients[0].b = mCoefficients[0].c = 0.;
        mCoefficients[0].d = mKnots[0].y();
    }
    else if (kSize == 2)
    {
        // ------------------------------------------
        // linear interpolation
        // ------------------------------------------
        mCoefficients[0].a = mCoefficients[0].b = 0.;
        mCoefficients[0].c = (mKnots[1].y() - mKnots[0].y()) / (mKnots[1].x() - mKnots[0].x());
        mCoefficients[0].d = mKnots[0].y();
    }
    else
    {
        // ------------------------------------------
        // cubic interpolation
        // ------------------------------------------
        std::vector<Eigen::Triplet<double> > tripletList;
        Eigen::SparseMatrix<double> AMatrix;
        Eigen::VectorXd bVector;
        Eigen::VectorXd xVector;
        register float dx0, dx1, dx2, dy0, dy1;

        // ------------------------------------------
        // if is periodic, we need to solve a
        // (ksize - 1) x (ksize - 1) system; otherwise,
        // the system is (ksize - 2) x (ksize - 2)
        // ------------------------------------------
        if (isPeriodic)
        {
            tripletList.reserve((kSize - 1) * 3);
            bVector = Eigen::VectorXd(kSize - 1);

            // ------------------------------------------
            // fill the A matrix and the b vector
            // ------------------------------------------
            for (int i = 1; i < kSize - 2; i++)
            {
                // distances between knots
                dx0 = mKnots[i].x() - mKnots[i - 1].x();
                dx1 = mKnots[i + 1].x() - mKnots[i].x();
                dx2 = dx0 + dx1;
                dy0 = mKnots[i].y() - mKnots[i - 1].y();
                dy1 = mKnots[i + 1].y() - mKnots[i].y();
                // diagonal
                tripletList.push_back(Eigen::Triplet<double>(i, i, 2. * dx2));
                // upper diagonal
                tripletList.push_back(Eigen::Triplet<double>(i, i + 1, dx1));
                // lower diagonal
                tripletList.push_back(Eigen::Triplet<double>(i + 1, i, dx1));
                // b vector
                bVector(i) = 6. * (dy1 / dx1 - dy0 / dx0);
            }

            // ------------------------------------------
            // fill the (0, 0), (0, 1), (0, n - 1),
            // (1, 0), (n - 1, 0), (n - 1, n - 1) entries
            // of A and the 0 and n - 1 entries of b
            // ------------------------------------------
            dx0 = mKnots[kSize - 1].x() - mKnots[kSize - 2].x();
            dx1 = mKnots[1].x() - mKnots[0].x();
            dx2 = dx0 + dx1;
            dy0 = mKnots[0].y() - mKnots[kSize - 2].y();
            dy1 = mKnots[1].y() - mKnots[0].y();
            tripletList.push_back(Eigen::Triplet<double>(0, 0, 2. * dx2));
            tripletList.push_back(Eigen::Triplet<double>(0, 1, dx1));
            tripletList.push_back(Eigen::Triplet<double>(0, kSize - 2, dx0));
            tripletList.push_back(Eigen::Triplet<double>(1, 0, dx1));
            bVector(0) = 6. * (dy1 / dx1 - dy0 / dx0);
            dx0 = mKnots[kSize - 2].x() - mKnots[kSize - 3].x();
            dx1 = mKnots[kSize - 1].x() - mKnots[kSize - 2].x();
            dx2 = dx0 + dx1;
            dy0 = mKnots[kSize - 2].y() - mKnots[kSize - 3].y();
            dy1 = mKnots[0].y() - mKnots[kSize - 2].y();
            tripletList.push_back(Eigen::Triplet<double>(kSize - 2, 0, dx1));
            tripletList.push_back(Eigen::Triplet<double>(kSize - 2, kSize - 2, 2. * dx2));
            bVector(kSize - 2) = 6. * (dy1 / dx1 - dy0 / dx0);

            // ------------------------------------------
            // solve system and copy solution to vector x
            // ------------------------------------------
            AMatrix = Eigen::SparseMatrix<double>(kSize - 1, kSize - 1);
            AMatrix.setFromTriplets(tripletList.begin(), tripletList.end());
            Eigen::SparseLU<Eigen::SparseMatrix<double> > solver(AMatrix);
            xVector = solver.solve(bVector);
        }
        else
        {
            tripletList.reserve((kSize - 2) * 3 - 2);
            bVector = Eigen::VectorXd(kSize - 2);
            xVector = Eigen::VectorXd(kSize);

            // ------------------------------------------
            // fill the A matrix and the b vector
            // ------------------------------------------
            for (int i = 0; i < kSize - 2; i++)
            {
                // distances between knots
                dx0 = mKnots[i + 1].x() - mKnots[i].x();
                dx1 = mKnots[i + 2].x() - mKnots[i + 1].x();
                dx2 = dx0 + dx1;
                dy0 = mKnots[i + 1].y() - mKnots[i].y();
                dy1 = mKnots[i + 2].y() - mKnots[i + 1].y();
                // diagonal
                tripletList.push_back(Eigen::Triplet<double>(i, i, 2. * dx2));
                if (i < kSize - 3)
                {
                    // upper diagonal
                    tripletList.push_back(Eigen::Triplet<double>(i, i + 1, dx1));
                    // lower diagonal
                    tripletList.push_back(Eigen::Triplet<double>(i + 1, i, dx1));
                }
                // b vector
                bVector(i) = 6. * (dy1 / dx1 - dy0 / dx0);
            }

            // ------------------------------------------
            // modify A matrix and b vector subject to
            // the end point constraints
            // ------------------------------------------
            dx0 = mKnots[1].x() - mKnots[0].x();
            dx1 = mKnots[kSize - 1].x() - mKnots[kSize - 2].x();
            dy0 = mKnots[1].y() - mKnots[0].y();
            dy1 = mKnots[kSize - 1].y() - mKnots[kSize - 2].y();
            if (mFloorBoundaryConditions == BoundaryConditions_Fixed1stDerivatives)
            {
                bVector(0) -= 3. * (dy0 / dx0 - mFloorBoundaryConditionsValue);
                tripletList.at(0) = Eigen::Triplet<double>(0, 0, tripletList.at(0).value() - dx0 / 2.);
            }
            else if (mFloorBoundaryConditions == BoundaryConditions_Fixed2ndDerivatives)
                bVector(0) -= dx0 * mFloorBoundaryConditionsValue;
            else if (mFloorBoundaryConditions == BoundaryConditions_Copy)
                tripletList.at(0) = Eigen::Triplet<double>(0, 0, tripletList.at(0).value() + dx0);
            else if (mFloorBoundaryConditions == BoundaryConditions_Extrapolate && kSize > 3)
            {
                dx2 = dx0 * dx0 / (mKnots[2].x() - mKnots[1].x());
                tripletList.at(0) = Eigen::Triplet<double>(0, 0, tripletList.at(0).value() + dx0 + dx2);
                tripletList.at(1) = Eigen::Triplet<double>(0, 1, tripletList.at(1).value() - dx2);
            }
            // EndPointConstraints_Natural, EndPointConstraints_Extrapolate && kSize == 3
            else
                xVector(0) = 0.;

            if (mCeilBoundaryConditions == BoundaryConditions_Fixed1stDerivatives)
            {
                bVector(kSize - 3) -= 3. * (mCeilBoundaryConditionsValue - dy1 / dx1);
                tripletList.at(tripletList.size() - 1) = Eigen::Triplet<double>(kSize - 3, kSize - 3,
                                                         tripletList.at(tripletList.size() - 1).value() - dx1 / 2.);
            }
            else if (mCeilBoundaryConditions == BoundaryConditions_Fixed2ndDerivatives)
                bVector(kSize - 3) -= dx1 * mCeilBoundaryConditionsValue;
            else if (mCeilBoundaryConditions == BoundaryConditions_Copy)
                tripletList.at(tripletList.size() - 1) = Eigen::Triplet<double>(kSize - 3, kSize - 3,
                                                         tripletList.at(tripletList.size() - 1).value() + dx1);
            else if (mCeilBoundaryConditions == BoundaryConditions_Extrapolate && kSize > 3)
            {
                dx2 = dx1 * dx1 / (mKnots[kSize - 2].x() - mKnots[kSize - 3].x());
                tripletList.at(tripletList.size() - 1) = Eigen::Triplet<double>(kSize - 3, kSize - 3,
                                                         tripletList.at(tripletList.size() - 1).value() + dx1 + dx2);
                tripletList.at(tripletList.size() - 2) = Eigen::Triplet<double>(kSize - 3, kSize - 4,
                                                         tripletList.at(tripletList.size() - 2).value() - dx2);
            }
            // EndPointConstraints_Natural, EndPointConstraints_Extrapolate && kSize == 3
            else
                xVector(kSize - 1) = 0.;

            // ------------------------------------------
            // solve system and copy solution to vector x
            // ------------------------------------------
            AMatrix = Eigen::SparseMatrix<double>(kSize - 2, kSize - 2);
            AMatrix.setFromTriplets(tripletList.begin(), tripletList.end());
            Eigen::SparseLU<Eigen::SparseMatrix<double> > solver(AMatrix);
            Eigen::VectorXd xVector2 = solver.solve(bVector);
            for (int i = 0; i < kSize - 2; i++)
                xVector(i + 1) = xVector2(i);

            // ------------------------------------------
            // fix the values of the second derivatives
            // at the first and last knots based on
            // the computed second derivatives at the
            // interiot knots
            // ------------------------------------------
            if (mFloorBoundaryConditions == BoundaryConditions_Fixed1stDerivatives)
                xVector(0) = (3. / dx0) * (dy0 / dx0 - mFloorBoundaryConditionsValue) - (xVector(1) / 2.);
            else if (mFloorBoundaryConditions == BoundaryConditions_Fixed2ndDerivatives)
                xVector(0) = mFloorBoundaryConditionsValue;
            else if (mFloorBoundaryConditions == BoundaryConditions_Copy)
                xVector(0) = xVector(1);
            else if (mFloorBoundaryConditions == BoundaryConditions_Extrapolate && kSize > 3)
                xVector(0) = xVector(1) - dx0 * (xVector(2) - xVector(1)) / (mKnots[2].x() - mKnots[1].x());

            if (mCeilBoundaryConditions == BoundaryConditions_Fixed1stDerivatives)
                xVector(kSize - 1) = (3. / dx1) * (mCeilBoundaryConditionsValue - dy1 / dx1) -
                        (xVector(kSize - 2) / 2.);
            else if (mCeilBoundaryConditions == BoundaryConditions_Fixed2ndDerivatives)
                xVector(kSize - 1) = mCeilBoundaryConditionsValue;
            else if (mCeilBoundaryConditions == BoundaryConditions_Copy)
                xVector(kSize - 1) = xVector(kSize - 2);
            else if (mCeilBoundaryConditions == BoundaryConditions_Extrapolate && kSize > 3)
                xVector(kSize - 1) = xVector(kSize - 2) + dx1 * (xVector(kSize - 2) - xVector(kSize - 3)) /
                                     (mKnots[kSize - 2].x() - mKnots[kSize - 3].x());
        }

        // ------------------------------------------
        // compute polynomial coefficients
        // ------------------------------------------
        for (int i = 0; i < kSize - 1; i++)
        {
            // distances between knots
            dx1 = mKnots[i + 1].x() - mKnots[i].x();
            dy1 = mKnots[i + 1].y() - mKnots[i].y();
            // compute coefficients
            dx2 = isPeriodic && i == kSize - 2 ? xVector(0) : xVector(i + 1);
            mCoefficients[i].a = (dx2 - xVector(i)) / (6. * dx1);
            mCoefficients[i].b = xVector(i) / 2.;
            mCoefficients[i].c = dy1 / dx1 - dx1 * (2. * xVector(i) + dx2) / 6.;
            mCoefficients[i].d = mKnots[i].y();
        }
    }

    mIsDirty = false;
}

bool CubicSplineInterpolator::setKnots(const SplineInterpolatorKnots &k)
{
    bool b = BaseSplineInterpolator::setKnots(k);
    if (b) mIsDirty = true;
    return b;
}

bool CubicSplineInterpolator::setKnot(int i, const SplineInterpolatorKnot &k)
{
    bool b = BaseSplineInterpolator::setKnot(i, k);
    if (b) mIsDirty = true;
    return b;
}

bool CubicSplineInterpolator::setKnot(int i, double nx, double ny)
{
    bool b = BaseSplineInterpolator::setKnot(i, nx, ny);
    if (b) mIsDirty = true;
    return b;
}

bool CubicSplineInterpolator::setKnot(double x, const SplineInterpolatorKnot &k)
{
    bool b = BaseSplineInterpolator::setKnot(x, k);
    if (b) mIsDirty = true;
    return b;
}

bool CubicSplineInterpolator::setKnot(double x, double nx, double ny)
{
    bool b = BaseSplineInterpolator::setKnot(x, nx, ny);
    if (b) mIsDirty = true;
    return b;
}

bool CubicSplineInterpolator::addKnot(const SplineInterpolatorKnot &k, bool replace, int * index)
{
    bool b = BaseSplineInterpolator::addKnot(k, replace, index);
    if (b) mIsDirty = true;
    return b;
}

bool CubicSplineInterpolator::addKnot(double nx, double ny, bool replace, int * index)
{
    bool b = BaseSplineInterpolator::addKnot(nx, ny, replace, index);
    if (b) mIsDirty = true;
    return b;
}

bool CubicSplineInterpolator::removeKnot(double x)
{
    bool b = BaseSplineInterpolator::removeKnot(x);
    if (b) mIsDirty = true;
    return b;
}

bool CubicSplineInterpolator::removeKnot(int i)
{
    bool b = BaseSplineInterpolator::removeKnot(i);
    if (b) mIsDirty = true;
    return b;
}

CubicSplineInterpolator::BoundaryConditions CubicSplineInterpolator::floorBoundaryConditions() const
{
    return mFloorBoundaryConditions;
}

CubicSplineInterpolator::BoundaryConditions CubicSplineInterpolator::ceilBoundaryConditions() const
{
    return mCeilBoundaryConditions;
}

double CubicSplineInterpolator::floorBoundaryConditionsValue() const
{
    return mFloorBoundaryConditionsValue;
}

double CubicSplineInterpolator::ceilBoundaryConditionsValue() const
{
    return mCeilBoundaryConditionsValue;
}

void CubicSplineInterpolator::setBoundaryConditions(BoundaryConditions f, BoundaryConditions c,
                                                     double fv, double cv)
{
    mFloorBoundaryConditions = f;
    mCeilBoundaryConditions = c;
    mFloorBoundaryConditionsValue = fv;
    mCeilBoundaryConditionsValue = cv;
    mIsDirty = true;
}

}}
