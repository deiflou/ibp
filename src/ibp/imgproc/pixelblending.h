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

#ifndef PIXELBLENDING_H
#define PIXELBLENDING_H

#include "types.h"

namespace ibp {
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
