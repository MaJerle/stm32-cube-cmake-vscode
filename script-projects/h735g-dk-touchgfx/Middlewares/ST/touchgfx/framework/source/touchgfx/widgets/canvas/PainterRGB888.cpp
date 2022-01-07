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
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>

namespace touchgfx
{
void PainterRGB888::render(uint8_t* ptr, int x, int xAdjust, int /*y*/, unsigned count, const uint8_t* covers)
{
    uint8_t* p = ptr + (x + xAdjust) * 3;
    const uint8_t* const p_lineend = p + 3 * count;
    uint8_t pByte;
    if (widgetAlpha == 0xFF)
    {
        do
        {
            const uint8_t alpha = *covers++;
            if (alpha == 0xFF)
            {
                *p++ = painterBlue;
                *p++ = painterGreen;
                *p++ = painterRed;
            }
            else
            {
                const uint8_t ialpha = 0xFF - alpha;
                pByte = *p;
                *p++ = LCD::div255(painterBlue * alpha + pByte * ialpha);
                pByte = *p;
                *p++ = LCD::div255(painterGreen * alpha + pByte * ialpha);
                pByte = *p;
                *p++ = LCD::div255(painterRed * alpha + pByte * ialpha);
            }
        } while (p < p_lineend);
    }
    else
    {
        do
        {
            const uint8_t alpha = LCD::div255(*covers++ * widgetAlpha);
            const uint8_t ialpha = 0xFF - alpha;
            pByte = *p;
            *p++ = LCD::div255(painterBlue * alpha + pByte * ialpha);
            pByte = *p;
            *p++ = LCD::div255(painterGreen * alpha + pByte * ialpha);
            pByte = *p;
            *p++ = LCD::div255(painterRed * alpha + pByte * ialpha);
        } while (p < p_lineend);
    }
}

bool PainterRGB888::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red = painterRed;
    green = painterGreen;
    blue = painterBlue;
    alpha = 0xFF;
    return true;
}
} // namespace touchgfx
