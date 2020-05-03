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

#ifndef IBP_IMGPROC_COLORCONVERSION_H
#define IBP_IMGPROC_COLORCONVERSION_H

#include "types.h"

namespace ibp {
namespace imgproc {

void initColorProfiles();

void convertBGRToHSV(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertBGRToHSL(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertBGRToLab(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertBGRToCMYK(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);

void convertHSVToBGR(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertHSVToHSL(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertHSVToLab(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertHSVToCMYK(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);

void convertHSLToBGR(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertHSLToHSV(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertHSLToLab(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertHSLToCMYK(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);

void convertLabToBGR(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertLabToHSV(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertLabToHSL(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertLabToCMYK(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);

void convertCMYKToBGR(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertCMYKToHSV(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertCMYKToHSL(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);
void convertCMYKToLab(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);

extern void (* convertColors[5][5])(const unsigned char * inputBuffer, unsigned char * outputBuffer, int nPixels);

inline void swapChannels01(unsigned char * inputBuffer);
inline void swapChannels02(unsigned char * inputBuffer);
inline void swapChannels03(unsigned char * inputBuffer);
inline void swapChannels12(unsigned char * inputBuffer);
inline void swapChannels13(unsigned char * inputBuffer);
inline void swapChannels23(unsigned char * inputBuffer);

extern void (* swapChannels[4][4])(unsigned char * inputBuffer);

}}

#endif // IBP_IMGPROC_COLORCONVERSION_H
