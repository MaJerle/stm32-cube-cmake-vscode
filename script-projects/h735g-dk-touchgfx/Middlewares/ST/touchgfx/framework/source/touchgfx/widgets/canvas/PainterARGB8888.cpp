/******************************************************************************
* Copyright (c) 2018(-2021) STMicroelectronics.
* All rights reserved.
*
* This file is part of the TouchGFX 4.18.1 distribution.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
*******************************************************************************/

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/PainterARGB8888.hpp>

namespace touchgfx
{
void PainterARGB8888::render(uint8_t* ptr, int x, int xAdjust, int /*y*/, unsigned count, const uint8_t* covers)
{
    uint8_t* p = ptr + (x + xAdjust) * 4;
    const uint8_t* const p_lineend = p + 4 * count;
    do
    {
        const uint8_t alphaFg = LCD::div255(*covers++ * widgetAlpha);
        const uint8_t alphaBg = p[3];
        if (alphaFg == 255 || alphaBg == 0)
        {
            *p++ = painterBlue;
            *p++ = painterGreen;
            *p++ = painterRed;
            *p++ = alphaFg;
        }
        else if (alphaFg > 0)
        {
            const uint8_t alphaMult = LCD::div255(alphaFg * alphaBg);
            const uint8_t alphaOut = alphaFg + alphaBg - alphaMult;

            const uint8_t blueBg = *p;
            *p++ = (painterBlue * alphaFg + blueBg * alphaBg - blueBg * alphaMult) / alphaOut;
            const uint8_t greenBg = *p;
            *p++ = (painterGreen * alphaFg + greenBg * alphaBg - greenBg * alphaMult) / alphaOut;
            const uint8_t redBg = *p;
            *p++ = (painterRed * alphaFg + redBg * alphaBg - redBg * alphaMult) / alphaOut;
            *p++ = alphaOut;
        }
        else
        {
            p += 4;
        }
    } while (p < p_lineend);
}

bool PainterARGB8888::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red = painterRed;
    green = painterGreen;
    blue = painterBlue;
    alpha = 0xFF;
    return true;
}
} // namespace touchgfx
