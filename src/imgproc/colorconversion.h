#ifndef ANITOOLS_IMGPROC_COLORCONVERSION_H
#define ANITOOLS_IMGPROC_COLORCONVERSION_H

#include "types.h"

namespace anitools {
namespace imgproc {

void initColorProfiles();

extern void (* convertColor[5][5])(const unsigned char * inputColor, unsigned char * outputColor);

void convertBGRToHSV(const unsigned char * inputColor, unsigned char * outputColor);
void convertBGRToHSL(const unsigned char * inputColor, unsigned char * outputColor);
void convertBGRToLab(const unsigned char * inputColor, unsigned char * outputColor);
void convertBGRToCMYK(const unsigned char * inputColor, unsigned char * outputColor);

void convertHSVToBGR(const unsigned char * inputColor, unsigned char * outputColor);
void convertHSVToHSL(const unsigned char * inputColor, unsigned char * outputColor);
void convertHSVToLab(const unsigned char * inputColor, unsigned char * outputColor);
void convertHSVToCMYK(const unsigned char * inputColor, unsigned char * outputColor);

void convertHSLToBGR(const unsigned char * inputColor, unsigned char * outputColor);
void convertHSLToHSV(const unsigned char * inputColor, unsigned char * outputColor);
void convertHSLToLab(const unsigned char * inputColor, unsigned char * outputColor);
void convertHSLToCMYK(const unsigned char * inputColor, unsigned char * outputColor);

void convertLabToBGR(const unsigned char * inputColor, unsigned char * outputColor);
void convertLabToHSV(const unsigned char * inputColor, unsigned char * outputColor);
void convertLabToHSL(const unsigned char * inputColor, unsigned char * outputColor);
void convertLabToCMYK(const unsigned char * inputColor, unsigned char * outputColor);

void convertCMYKToBGR(const unsigned char * inputColor, unsigned char * outputColor);
void convertCMYKToHSV(const unsigned char * inputColor, unsigned char * outputColor);
void convertCMYKToHSL(const unsigned char * inputColor, unsigned char * outputColor);
void convertCMYKToLab(const unsigned char * inputColor, unsigned char * outputColor);

void convertIdentity(const unsigned char * inputColor, unsigned char * outputColor);

extern void (* swapChannels[4][4])(unsigned char * inputColor);

inline void swapChannels01(unsigned char * inputColor);
inline void swapChannels02(unsigned char * inputColor);
inline void swapChannels03(unsigned char * inputColor);
inline void swapChannels12(unsigned char * inputColor);
inline void swapChannels13(unsigned char * inputColor);
inline void swapChannels23(unsigned char * inputColor);
inline void swapChannelsIdentity(unsigned char * inputColor);

}}

#endif // ANITOOLS_IMGPROC_COLORCONVERSION_H
