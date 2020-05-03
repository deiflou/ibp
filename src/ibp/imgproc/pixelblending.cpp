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

#include "pixelblending.h"
#include "lut.h"
#include "util.h"
#include "colorconversion.h"
#include "../misc/util.h"

namespace ibp {
namespace imgproc {

void (*alphaBlendColors[12])(BGRA src, BGRA dst, BGRA & blend) = {
blendSource,
blendDestination,
blendSourceOverDestination,
blendDestinationOverSource,
blendSourceInDestination,
blendDestinationInSource,
blendSourceOutDestination,
blendDestinationOutSource,
blendSourceAtopDestination,
blendDestinationAtopSource,
blendSourceClearDestination,
blendSourceXorDestination
};

void (*blendColors[24])(BGRA src, BGRA dst, BGRA & blend) = {
blendSourceOverDestination,
blendDarken,
blendMultiply,
blendColorBurn,
blendLinearBurn,
blendDarkerColor,
blendLighten,
blendScreen,
blendColorDodge,
blendLinearDodge,
blendLighterColor,
blendOverlay,
blendSoftLight,
blendHardLight,
blendVividLight,
blendLinearLight,
blendPinLight,
blendHardMix,
blendDifference,
blendExclusion,
blendHue,
blendSaturation,
blendColor,
blendLuminosity
};

#define IBP_EARLY_BLEND_ALPHA_DISCARD \
if (blend.a == 0) \
{ \
    blend.r = blend.g = blend.b = 0; \
    return; \
}

#define IBP_EARLY_SRC_DST_ALPHA_DISCARD \
if (src.a == 0) \
{ \
    blend = dst; \
    return; \
} \
if (dst.a == 0) \
{ \
    blend = src; \
    return; \
}

#define IBP_EARLY_ALPHA_DISCARD \
IBP_EARLY_SRC_DST_ALPHA_DISCARD; \
blend.a = lut03[dst.a][src.a]; \
IBP_EARLY_BLEND_ALPHA_DISCARD;

#define IBP_PRE_BLEND \
IBP_EARLY_ALPHA_DISCARD; \
IBP_premultiplyBGRA(src); \
IBP_premultiplyBGRA(dst);

#define IBP_POST_BLEND \
blend.r += lut01[src.r][255 - dst.a] + lut01[dst.r][255 - src.a]; \
blend.g += lut01[src.g][255 - dst.a] + lut01[dst.g][255 - src.a]; \
blend.b += lut01[src.b][255 - dst.a] + lut01[dst.b][255 - src.a]; \
IBP_postmultiplyBGRA(blend);

void blendSource(BGRA src, BGRA /*dst*/, BGRA & blend)
{
    blend = src;
}

void blendDestination(BGRA /*src*/, BGRA dst, BGRA & blend)
{
    blend = dst;
}

void blendSourceOverDestination(BGRA src, BGRA dst, BGRA & blend)
{
    blend.a = lut03[dst.a][src.a];
    IBP_EARLY_BLEND_ALPHA_DISCARD;

    IBP_premultiplyBGRA(src);
    IBP_premultiplyBGRA(dst);

    blend.r = src.r + lut01[dst.r][255 - src.a];
    blend.g = src.g + lut01[dst.g][255 - src.a];
    blend.b = src.b + lut01[dst.b][255 - src.a];

    IBP_postmultiplyBGRA(blend);
}

void blendDestinationOverSource(BGRA src, BGRA dst, BGRA & blend)
{
    blend.a = lut03[src.a][dst.a];
    IBP_EARLY_BLEND_ALPHA_DISCARD;

    IBP_premultiplyBGRA(src);
    IBP_premultiplyBGRA(dst);

    blend.r = dst.r + lut01[src.r][255 - dst.a];
    blend.g = dst.g + lut01[src.g][255 - dst.a];
    blend.b = dst.b + lut01[src.b][255 - dst.a];

    IBP_postmultiplyBGRA(blend);
}

void blendSourceInDestination(BGRA src, BGRA dst, BGRA & blend)
{
    blend.r = src.r;
    blend.g = src.g;
    blend.b = src.b;
    blend.a = lut01[src.a][dst.a];
}

void blendDestinationInSource(BGRA src, BGRA dst, BGRA & blend)
{
    blend.r = dst.r;
    blend.g = dst.g;
    blend.b = dst.b;
    blend.a = lut01[dst.a][src.a];
}

void blendSourceOutDestination(BGRA src, BGRA dst, BGRA & blend)
{
    blend.r = src.r;
    blend.g = src.g;
    blend.b = src.b;
    blend.a = lut01[src.a][255 - dst.a];
}

void blendDestinationOutSource(BGRA src, BGRA dst, BGRA & blend)
{
    blend.r = dst.r;
    blend.g = dst.g;
    blend.b = dst.b;
    blend.a = lut01[dst.a][255 - src.a];
}

void blendSourceAtopDestination(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_premultiplyBGRA(src);

    blend.r = src.r + lut01[dst.r][255 - src.a];
    blend.g = src.g + lut01[dst.g][255 - src.a];
    blend.b = src.b + lut01[dst.b][255 - src.a];
    blend.a = dst.a;
}

void blendDestinationAtopSource(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_premultiplyBGRA(dst);

    blend.r = dst.r + lut01[src.r][255 - dst.a];
    blend.g = dst.g + lut01[src.g][255 - dst.a];
    blend.b = dst.b + lut01[src.b][255 - dst.a];
    blend.a = src.a;
}

void blendSourceClearDestination(BGRA /*src*/, BGRA /*dst*/, BGRA & blend)
{
    blend.r = blend.g = blend.b = blend.a = 0;
}

void blendSourceXorDestination(BGRA src, BGRA dst, BGRA & blend)
{
    blend.a = src.a + dst.a - (lut01[src.a][dst.a] << 1);
    IBP_EARLY_BLEND_ALPHA_DISCARD;

    IBP_premultiplyBGRA(src);
    IBP_premultiplyBGRA(dst);

    blend.r = lut01[src.r][255 - dst.a] + lut01[dst.r][255 - src.a];
    blend.g = lut01[src.g][255 - dst.a] + lut01[dst.g][255 - src.a];
    blend.b = lut01[src.b][255 - dst.a] + lut01[dst.b][255 - src.a];

    IBP_postmultiplyBGRA(blend);
}

void blendDarken(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    blend.r = qMin<int>(lut01[src.r][dst.a], lut01[dst.r][src.a]);
    blend.g = qMin<int>(lut01[src.g][dst.a], lut01[dst.g][src.a]);
    blend.b = qMin<int>(lut01[src.b][dst.a], lut01[dst.b][src.a]);

    IBP_POST_BLEND
}

void blendMultiply(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    blend.r = lut01[src.r][dst.r];
    blend.g = lut01[src.g][dst.g];
    blend.b = lut01[src.b][dst.b];

    IBP_POST_BLEND
}

void blendColorBurn(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    if (src.r == 0)
    {
        if (dst.r == dst.a)
            blend.r = lut01[src.a][dst.a] + lut01[dst.r][255 - src.a];
        else
            blend.r = lut01[dst.r][255 - src.a];
    }
    else
        blend.r = lut01[lut01[src.a][dst.a]][255 - qMin<int>(255, (255 - lut02[dst.r][dst.a]) * src.a / src.r)] +
                  lut01[src.r][255 - dst.a] + lut01[dst.r][255 - src.a];
    if (src.g == 0)
    {
        if (dst.g == dst.a)
            blend.g = lut01[src.a][dst.a] + lut01[dst.g][255 - src.a];
        else
            blend.g = lut01[dst.g][255 - src.a];
    }
    else
        blend.g = lut01[lut01[src.a][dst.a]][255 - qMin<int>(255, (255 - lut02[dst.g][dst.a]) * src.a / src.g)] +
                lut01[src.g][255 - dst.a] + lut01[dst.g][255 - src.a];
    if (src.b == 0)
    {
        if (dst.b == dst.a)
            blend.b = lut01[src.a][dst.a] + lut01[dst.b][255 - src.a];
        else
            blend.b = lut01[dst.b][255 - src.a];
    }
    else
        blend.b = lut01[lut01[src.a][dst.a]][255 - qMin<int>(255, (255 - lut02[dst.b][dst.a]) * src.a / src.b)] +
                lut01[src.b][255 - dst.a] + lut01[dst.b][255 - src.a];

    IBP_postmultiplyBGRA(blend);
}

void blendLinearBurn(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    blend.r = qMax<int>(src.r + dst.r - lut01[src.a][dst.a], 0);
    blend.g = qMax<int>(src.g + dst.g - lut01[src.a][dst.a], 0);
    blend.b = qMax<int>(src.b + dst.b - lut01[src.a][dst.a], 0);

    IBP_postmultiplyBGRA(blend);
}

void blendDarkerColor(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    if (IBP_pixelIntensity1(src) < IBP_pixelIntensity1(dst))
    {
        blend.r = lut01[src.r][dst.a];
        blend.g = lut01[src.g][dst.a];
        blend.b = lut01[src.b][dst.a];
    }
    else
    {
        blend.r = lut01[dst.r][src.a];
        blend.g = lut01[dst.g][src.a];
        blend.b = lut01[dst.b][src.a];
    }

    IBP_POST_BLEND
}


void blendLighten(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    blend.r = qMax<int>(lut01[src.r][dst.a], lut01[dst.r][src.a]);
    blend.g = qMax<int>(lut01[src.g][dst.a], lut01[dst.g][src.a]);
    blend.b = qMax<int>(lut01[src.b][dst.a], lut01[dst.b][src.a]);

    IBP_POST_BLEND
}

void blendScreen(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    blend.r = lut03[src.r][dst.r];
    blend.g = lut03[src.g][dst.g];
    blend.b = lut03[src.b][dst.b];

    IBP_postmultiplyBGRA(blend);
}

void blendColorDodge(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    if (src.r == src.a)
    {
        if (dst.r == 0)
            blend.r = lut01[src.r][255 - dst.a];
        else
            blend.r = lut01[src.a][dst.a] + lut01[src.r][255- dst.a] + lut01[dst.r][255 - src.a];
    }
    else
        blend.r = lut01[lut01[src.a][dst.a]][qMin<int>(255, lut02[dst.r][dst.a] * src.a / (src.a - src.r))] +
                lut01[src.r][255- dst.a] + lut01[dst.r][255 - src.a];
    if (src.g == src.a)
    {
        if (dst.g == 0)
            blend.g = lut01[src.g][255 - dst.a];
        else
            blend.g = lut01[src.a][dst.a] + lut01[src.g][255- dst.a] + lut01[dst.g][255 - src.a];
    }
    else
        blend.g = lut01[lut01[src.a][dst.a]][qMin<int>(255, lut02[dst.g][dst.a] * src.a / (src.a - src.g))] +
                lut01[src.g][255- dst.a] + lut01[dst.g][255 - src.a];
    if (src.b == src.a)
    {
        if (dst.b == 0)
            blend.b = lut01[src.b][255 - dst.a];
        else
            blend.b = lut01[src.a][dst.a] + lut01[src.b][255- dst.a] + lut01[dst.b][255 - src.a];
    }
    else
        blend.b = lut01[lut01[src.a][dst.a]][qMin<int>(255, lut02[dst.b][dst.a] * src.a / (src.a - src.b))] +
                lut01[src.b][255- dst.a] + lut01[dst.b][255 - src.a];

    IBP_postmultiplyBGRA(blend);
}

void blendLinearDodge(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    //blend.r = qMin<int>((src.r + dst.r) * 255 / blend.a, 255);
    //blend.g = qMin<int>((src.g + dst.g) * 255 / blend.a, 255);
    //blend.b = qMin<int>((src.b + dst.b) * 255 / blend.a, 255);
    blend.r = qMin<int>(lut02[src.r][blend.a] + lut02[dst.r][blend.a], 255);
    blend.g = qMin<int>(lut02[src.g][blend.a] + lut02[dst.g][blend.a], 255);
    blend.b = qMin<int>(lut02[src.b][blend.a] + lut02[dst.b][blend.a], 255);
}

void blendLighterColor(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    if (IBP_pixelIntensity1(src) > IBP_pixelIntensity1(dst))
    {
        blend.r = lut01[src.r][dst.a];
        blend.g = lut01[src.g][dst.a];
        blend.b = lut01[src.b][dst.a];
    }
    else
    {
        blend.r = lut01[dst.r][src.a];
        blend.g = lut01[dst.g][src.a];
        blend.b = lut01[dst.b][src.a];
    }

    IBP_POST_BLEND
}


void blendOverlay(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    if (2 * dst.r <= dst.a)
        blend.r = 2 * lut01[src.r][dst.r] + lut01[src.r][255 - dst.a] + lut01[dst.r][255 - src.a];
    else
        blend.r = src.r * (255 + dst.a) / 255 + dst.r * (255 + src.a) / 255 - 2 * lut01[dst.r][src.r] -
                lut01[dst.a][src.a];

    if (2 * dst.g <= dst.a)
        blend.g = 2 * lut01[src.g][dst.g] + lut01[src.g][255 - dst.a] + lut01[dst.g][255 - src.a];
    else
        blend.g = src.g * (255 + dst.a) / 255 + dst.g * (255 + src.a) / 255 - 2 * lut01[dst.g][src.g] -
                lut01[dst.a][src.a];

    if (2 * dst.b <= dst.a)
        blend.b = 2 * lut01[src.b][dst.b] + lut01[src.b][255 - dst.a] + lut01[dst.b][255 - src.a];
    else
        blend.b = src.b * (255 + dst.a) / 255 + dst.b * (255 + src.a) / 255 - 2 * lut01[dst.b][src.b] -
                lut01[dst.a][src.a];

    IBP_postmultiplyBGRA(blend);
}

void blendSoftLight(BGRA src, BGRA dst, BGRA & blend)
{
    float m;

    IBP_PRE_BLEND

    m = lut02[dst.r][dst.a] / 255.0f;
    if (2 * src.r < src.a)
        blend.r = lut01[dst.r][src.a + (int)((2 * src.r - src.a) * (1.0f - m))];
    else if (2 * src.r > src.a && 4 * dst.r <= dst.a)
        blend.r = lut01[dst.r][src.a] + lut01[dst.a][(int)((2 * src.r - src.a) * (((4.0f * m) * (4.0f * m + 1.0f)) *
                                                                                  (m - 1.0f) + 7.0f * m))];
    else
        blend.r = lut01[dst.r][src.a] + lut01[dst.a][(int)((2 * src.r - src.a) * (pow(m, 0.5f) - m))];

    m = lut02[dst.g][dst.a] / 255.0f;
    if (2 * src.g < src.a)
        blend.g = lut01[dst.g][src.a + (int)((2 * src.g - src.a) * (1.0f - m))];
    else if (2 * src.g > src.a && 4 * dst.g <= dst.a)
        blend.g = lut01[dst.g][src.a] + lut01[dst.a][(int)((2 * src.g - src.a) * (((4.0f * m) * (4.0f * m + 1.0f)) *
                                                                                  (m - 1.0f) + 7.0f * m))];
    else
        blend.g = lut01[dst.g][src.a] + lut01[dst.a][(int)((2 * src.g - src.a) * (pow(m, 0.5f) - m))];

    m = lut02[dst.b][dst.a] / 255.0f;
    if (2 * src.b < src.a)
        blend.b = lut01[dst.b][src.a + (int)((2 * src.b - src.a) * (1.0f - m))];
    else if (2 * src.b > src.a && 4 * dst.b <= dst.a)
        blend.b = lut01[dst.b][src.a] + lut01[dst.a][(int)((2 * src.b - src.a) * (((4.0f * m) * (4.0f * m + 1.0f)) *
                                                                                  (m - 1.0f) + 7.0f * m))];
    else
        blend.b = lut01[dst.b][src.a] + lut01[dst.a][(int)((2 * src.b - src.a) * (pow(m, 0.5f) - m))];

    IBP_POST_BLEND
}

void blendHardLight(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    if (2 * src.r <= src.a)
        blend.r = 2 * lut01[src.r][dst.r] + lut01[src.r][255 - dst.a] + lut01[dst.r][255 - src.a];
    else
        blend.r = src.r * (255 + dst.a) / 255 + dst.r * (255 + src.a) / 255 - 2 * lut01[src.r][dst.r] -
                lut01[src.a][dst.a];

    if (2 * src.g <= src.a)
        blend.g = 2 * lut01[src.g][dst.g] + lut01[src.g][255 - dst.a] + lut01[dst.g][255 - src.a];
    else
        blend.g = src.g * (255 + dst.a) / 255 + dst.g * (255 + src.a) / 255 - 2 * lut01[src.g][dst.g] -
                lut01[src.a][dst.a];

    if (2 * src.b <= src.a)
        blend.b = 2 * lut01[src.b][dst.b] + lut01[src.b][255 - dst.a] + lut01[dst.b][255 - src.a];
    else
        blend.b = src.b * (255 + dst.a) / 255 + dst.b * (255 + src.a) / 255 - 2 * lut01[src.b][dst.b] -
                lut01[src.a][dst.a];

    IBP_postmultiplyBGRA(blend);
}

void blendVividLight(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    if (2 * src.r <= src.a)
    {
        if (src.r == 0)
        {
            blend.r = lut01[dst.r][255 - src.a];
        }
        else
            blend.r = lut01[lut01[src.a][dst.a]][255 - qMin<int>(255, (255 - lut02[dst.r][dst.a]) * src.a /
                    (2 * src.r))] + lut01[src.r][255 - dst.a] + lut01[dst.r][255 - src.a];
    }
    else
    {
        if (src.r == src.a)
        {
            if (dst.r == 0)
                blend.r = lut01[src.r][255 - dst.a];
            else
                blend.r = lut01[src.a][dst.a] + lut01[src.r][255 - dst.a] + lut01[dst.r][255 - src.a];
        }
        else
            blend.r = lut01[lut01[src.a][dst.a]][qMin<int>(255, lut02[dst.r][dst.a] * src.a /
                    (2 * (src.a - src.r)))] + lut01[src.r][255 - dst.a] + lut01[dst.r][255 - src.a];
    }
    if (2 * src.g <= src.a)
    {
        if (src.g == 0)
        {
            blend.g = lut01[dst.g][255 - src.a];
        }
        else
            blend.g = lut01[lut01[src.a][dst.a]][255 - qMin<int>(255, (255 - lut02[dst.g][dst.a]) * src.a /
                    (2 * src.g))] + lut01[src.g][255 - dst.a] + lut01[dst.g][255 - src.a];
    }
    else
    {
        if (src.g == src.a)
        {
            if (dst.g == 0)
                blend.g = lut01[src.g][255 - dst.a];
            else
                blend.g = lut01[src.a][dst.a] + lut01[src.g][255 - dst.a] + lut01[dst.g][255 - src.a];
        }
        else
            blend.g = lut01[lut01[src.a][dst.a]][qMin<int>(255, lut02[dst.g][dst.a] * src.a /
                    (2 * (src.a - src.g)))] + lut01[src.g][255 - dst.a] + lut01[dst.g][255 - src.a];
    }
    if (2 * src.b <= src.a)
    {
        if (src.b == 0)
        {
            blend.b = lut01[dst.b][255 - src.a];
        }
        else
            blend.b = lut01[lut01[src.a][dst.a]][255 - qMin<int>(255, (255 - lut02[dst.b][dst.a]) * src.a /
                    (2 * src.b))] + lut01[src.b][255 - dst.a] + lut01[dst.b][255 - src.a];
    }
    else
    {
        if (src.b == src.a)
        {
            if (dst.b == 0)
                blend.b = lut01[src.b][255 - dst.a];
            else
                blend.b = lut01[src.a][dst.a] + lut01[src.b][255 - dst.a] + lut01[dst.b][255 - src.a];
        }
        else
            blend.b = lut01[lut01[src.a][dst.a]][qMin<int>(255, lut02[dst.b][dst.a] * src.a /
                    (2 * (src.a - src.b)))] + lut01[src.b][255 - dst.a] + lut01[dst.b][255 - src.a];
    }

    IBP_postmultiplyBGRA(blend);
}

void blendLinearLight(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    if (2 * src.r <= src.a)
        blend.r = qMax<int>((lut01[src.r][dst.a] - lut01[src.a][dst.a] + src.r + dst.r) * 255 / blend.a, 0);
    else
        blend.r = qMin<int>((lut01[src.r][dst.a] - lut01[src.a][dst.a] + src.r + dst.r) * 255 / blend.a, 255);
    if (2 * src.g <= src.a)
        blend.g = qMax<int>((lut01[src.g][dst.a] - lut01[src.a][dst.a] + src.g + dst.g) * 255 / blend.a, 0);
    else
        blend.g = qMin<int>((lut01[src.g][dst.a] - lut01[src.a][dst.a] + src.g + dst.g) * 255 / blend.a, 255);
    if (2 * src.b <= src.a)
        blend.b = qMax<int>((lut01[src.b][dst.a] - lut01[src.a][dst.a] + src.b + dst.b) * 255 / blend.a, 0);
    else
        blend.b = qMin<int>((lut01[src.b][dst.a] - lut01[src.a][dst.a] + src.b + dst.b) * 255 / blend.a, 255);
    /*
    if (2 * src.r <= src.a)
        blend.r = qMax<int>(((src.r - src.a) * dst.a / 255 + src.r + dst.r) * 255 / blend.a, 0);
    else
        blend.r = qMin<int>(((src.r - src.a) * dst.a / 255 + src.r + dst.r) * 255 / blend.a, 255);
    if (2 * src.g <= src.a)
        blend.g = qMax<int>(((src.g - src.a) * dst.a / 255 + src.g + dst.g) * 255 / blend.a, 0);
    else
        blend.g = qMin<int>(((src.g - src.a) * dst.a / 255 + src.g + dst.g) * 255 / blend.a, 255);
    if (2 * src.b <= src.a)
        blend.b = qMax<int>(((src.b - src.a) * dst.a / 255 + src.b + dst.b) * 255 / blend.a, 0);
    else
        blend.b = qMin<int>(((src.b - src.a) * dst.a / 255 + src.b + dst.b) * 255 / blend.a, 255);
        */
}

void blendPinLight(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    if (lut01[dst.r][src.a] < dst.a * (2 * src.r - src.a) / 255)
        blend.r = src.r * (dst.a + 255) / 255 - lut01[src.a][dst.a] + lut01[dst.r][255 - src.a];
    else if (lut01[dst.r][src.a] > 2 * lut01[src.r][dst.a])
        blend.r = lut01[src.r][dst.a] + src.r + lut01[dst.r][255 - src.a];
    else
        blend.r = lut01[src.r][255 - dst.a] + dst.r;
    if (lut01[dst.g][src.a] < dst.a * (2 * src.g - src.a) / 255)
        blend.g = src.g * (dst.a + 255) / 255 - lut01[src.a][dst.a] + lut01[dst.g][255 - src.a];
    else if (lut01[dst.g][src.a] > 2 * lut01[src.g][dst.a])
        blend.g = lut01[src.g][dst.a] + src.g + lut01[dst.g][255 - src.a];
    else
        blend.g = lut01[src.g][255 - dst.a] + dst.g;
    if (lut01[dst.b][src.a] < dst.a * (2 * src.b - src.a) / 255)
        blend.b = src.b * (dst.a + 255) / 255 - lut01[src.a][dst.a] + lut01[dst.b][255 - src.a];
    else if (lut01[dst.b][src.a] > 2 * lut01[src.b][dst.a])
        blend.b = lut01[src.b][dst.a] + src.b + lut01[dst.b][255 - src.a];
    else
        blend.b = lut01[src.b][255 - dst.a] + dst.b;

    IBP_postmultiplyBGRA(blend);
}

void blendHardMix(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_EARLY_ALPHA_DISCARD

    blend.r = src.r < 255 - dst.r ? 0 : lut01[src.a][dst.a];
    blend.g = src.g < 255 - dst.g ? 0 : lut01[src.a][dst.a];
    blend.b = src.b < 255 - dst.b ? 0 : lut01[src.a][dst.a];

    IBP_premultiplyBGRA(src);
    IBP_premultiplyBGRA(dst);

    IBP_POST_BLEND
}


void blendDifference(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    blend.r = src.r + dst.r - 2 * qMin<int>(lut01[src.r][dst.a], lut01[dst.r][src.a]);
    blend.g = src.g + dst.g - 2 * qMin<int>(lut01[src.g][dst.a], lut01[dst.g][src.a]);
    blend.b = src.b + dst.b - 2 * qMin<int>(lut01[src.b][dst.a], lut01[dst.b][src.a]);

    IBP_postmultiplyBGRA(blend);
}

void blendExclusion(BGRA src, BGRA dst, BGRA & blend)
{
    IBP_PRE_BLEND

    blend.r = lut01[src.r][dst.a] + lut01[dst.r][src.a] - 2 * lut01[src.r][dst.r];
    blend.g = lut01[src.g][dst.a] + lut01[dst.g][src.a] - 2 * lut01[src.g][dst.g];
    blend.b = lut01[src.b][dst.a] + lut01[dst.b][src.a] - 2 * lut01[src.b][dst.b];

    IBP_POST_BLEND
}


void blendHue(BGRA src, BGRA dst, BGRA & blend)
{
    HSL srcHSL, dstHSL;

    IBP_EARLY_ALPHA_DISCARD

    convertBGRToHSL((unsigned char *)&src, (unsigned char *)&srcHSL, 1);
    convertBGRToHSL((unsigned char *)&dst, (unsigned char *)&dstHSL, 1);
    dstHSL.h = srcHSL.h;
    convertHSLToBGR((unsigned char *)&dstHSL, (unsigned char *)&blend, 1);

    IBP_premultiplyBGRA(src);
    IBP_premultiplyBGRA(dst);

    IBP_premultiplyBGRAWithAlpha(blend, src.a);
    IBP_premultiplyBGRAWithAlpha(blend, dst.a);

    IBP_POST_BLEND
}

void blendSaturation(BGRA src, BGRA dst, BGRA & blend)
{
    HSL srcHSL, dstHSL;

    IBP_EARLY_ALPHA_DISCARD

    convertBGRToHSL((unsigned char *)&src, (unsigned char *)&srcHSL, 1);
    convertBGRToHSL((unsigned char *)&dst, (unsigned char *)&dstHSL, 1);
    dstHSL.s = srcHSL.s;
    convertHSLToBGR((unsigned char *)&dstHSL, (unsigned char *)&blend, 1);

    IBP_premultiplyBGRA(src);
    IBP_premultiplyBGRA(dst);

    IBP_premultiplyBGRAWithAlpha(blend, src.a);
    IBP_premultiplyBGRAWithAlpha(blend, dst.a);

    IBP_POST_BLEND
}

void blendColor(BGRA src, BGRA dst, BGRA & blend)
{
    HSL srcHSL, dstHSL;

    IBP_EARLY_ALPHA_DISCARD

    convertBGRToHSL((unsigned char *)&src, (unsigned char *)&srcHSL, 1);
    convertBGRToHSL((unsigned char *)&dst, (unsigned char *)&dstHSL, 1);
    srcHSL.l = dstHSL.l;
    convertHSLToBGR((unsigned char *)&srcHSL, (unsigned char *)&blend, 1);

    IBP_premultiplyBGRA(src);
    IBP_premultiplyBGRA(dst);

    IBP_premultiplyBGRAWithAlpha(blend, src.a);
    IBP_premultiplyBGRAWithAlpha(blend, dst.a);

    IBP_POST_BLEND
}

void blendLuminosity(BGRA src, BGRA dst, BGRA & blend)
{
    HSL srcHSL, dstHSL;

    IBP_EARLY_ALPHA_DISCARD

    convertBGRToHSL((unsigned char *)&src, (unsigned char *)&srcHSL, 1);
    convertBGRToHSL((unsigned char *)&dst, (unsigned char *)&dstHSL, 1);
    dstHSL.l = srcHSL.l;
    convertHSLToBGR((unsigned char *)&dstHSL, (unsigned char *)&blend, 1);

    IBP_premultiplyBGRA(src);
    IBP_premultiplyBGRA(dst);

    IBP_premultiplyBGRAWithAlpha(blend, src.a);
    IBP_premultiplyBGRAWithAlpha(blend, dst.a);

    IBP_POST_BLEND
}

}}
