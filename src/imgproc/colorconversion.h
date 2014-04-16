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

#ifndef ANITOOLS_IMGPROC_COLORCONVERSION_H
#define ANITOOLS_IMGPROC_COLORCONVERSION_H

#include "types.h"

namespace anitools {
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

#endif // ANITOOLS_IMGPROC_COLORCONVERSION_H
