/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
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

#ifndef PIXELBLENDING_H
#define PIXELBLENDING_H

#include "types.h"

namespace anitools {
namespace imgproc {

void blendSource(BGRA src, BGRA dst, BGRA & blend);
void blendDestination(BGRA src, BGRA dst, BGRA & blend);
void blendSourceOverDestination(BGRA src, BGRA dst, BGRA & blend);
void blendDestinationOverSource(BGRA src, BGRA dst, BGRA & blend);
void blendSourceInDestination(BGRA src, BGRA dst, BGRA & blend);
void blendDestinationInSource(BGRA src, BGRA dst, BGRA & blend);
void blendSourceOutDestination(BGRA src, BGRA dst, BGRA & blend);
void blendDestinationOutSource(BGRA src, BGRA dst, BGRA & blend);
void blendSourceAtopDestination(BGRA src, BGRA dst, BGRA & blend);
void blendDestinationAtopSource(BGRA src, BGRA dst, BGRA & blend);
void blendSourceClearDestination(BGRA src, BGRA dst, BGRA & blend);
void blendSourceXorDestination(BGRA src, BGRA dst, BGRA & blend);

extern void (*alphaBlendColors[12])(BGRA src, BGRA dst, BGRA & blend);

void blendDarken(BGRA src, BGRA dst, BGRA & blend);
void blendMultiply(BGRA src, BGRA dst, BGRA & blend);
void blendColorBurn(BGRA src, BGRA dst, BGRA & blend);
void blendLinearBurn(BGRA src, BGRA dst, BGRA & blend);
void blendDarkerColor(BGRA src, BGRA dst, BGRA & blend);
void blendLighten(BGRA src, BGRA dst, BGRA & blend);
void blendScreen(BGRA src, BGRA dst, BGRA & blend);
void blendColorDodge(BGRA src, BGRA dst, BGRA & blend);
void blendLinearDodge(BGRA src, BGRA dst, BGRA & blend);
void blendLighterColor(BGRA src, BGRA dst, BGRA & blend);
void blendOverlay(BGRA src, BGRA dst, BGRA & blend);
void blendSoftLight(BGRA src, BGRA dst, BGRA & blend);
void blendHardLight(BGRA src, BGRA dst, BGRA & blend);
void blendVividLight(BGRA src, BGRA dst, BGRA & blend);
void blendLinearLight(BGRA src, BGRA dst, BGRA & blend);
void blendPinLight(BGRA src, BGRA dst, BGRA & blend);
void blendHardMix(BGRA src, BGRA dst, BGRA & blend);
void blendDifference(BGRA src, BGRA dst, BGRA & blend);
void blendExclusion(BGRA src, BGRA dst, BGRA & blend);
void blendHue(BGRA src, BGRA dst, BGRA & blend);
void blendSaturation(BGRA src, BGRA dst, BGRA & blend);
void blendColor(BGRA src, BGRA dst, BGRA & blend);
void blendLuminosity(BGRA src, BGRA dst, BGRA & blend);

extern void (*blendColors[24])(BGRA src, BGRA dst, BGRA & blend);

}}

#endif // PIXELBLENDING_H
