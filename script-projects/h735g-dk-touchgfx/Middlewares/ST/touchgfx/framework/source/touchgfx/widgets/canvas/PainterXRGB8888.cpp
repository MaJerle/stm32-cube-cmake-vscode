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
#include <touchgfx/widgets/canvas/PainterXRGB8888.hpp>

namespace touchgfx
{
void PainterXRGB8888::render(uint8_t* ptr, int x, int xAdjust, int /*y*/, unsigned count, const uint8_t* covers)
{
    uint32_t* p32 = reinterpret_cast<uint32_t*>(ptr) + (x + xAdjust);
    const uint32_t* const p32_lineend = p32 + count;
    if (widgetAlpha == 0xFF)
    {
        do
        {
            const uint8_t alpha = *covers++;
            if (alpha == 0xFF)
            {
                *p32 = painterColor;
            }
            else
            {
                const uint8_t ialpha = 0xFF - alpha;
                const uint32_t rgbBg = *p32;
                *p32 = (LCD::div255(painterBlue * alpha + (rgbBg & 0xFF) * ialpha)) |
                       (LCD::div255(painterGreen * alpha + ((rgbBg >> 8) & 0xFF) * ialpha) << 8) |
                       (LCD::div255(painterRed * alpha + ((rgbBg >> 16) & 0xFF) * ialpha) << 16);
            }
            p32++;
        } while (p32 < p32_lineend);
    }
    else
    {
        do
        {
            const uint8_t alpha = LCD::div255(*covers++ * widgetAlpha);
            const uint8_t ialpha = 0xFF - alpha;
            const uint32_t rgbBg = *p32;
            *p32++ = (LCD::div255(painterBlue * alpha + (rgbBg & 0xFF) * ialpha)) |
                     (LCD::div255(painterGreen * alpha + ((rgbBg >> 8) & 0xFF) * ialpha) << 8) |
                     (LCD::div255(painterRed * alpha + ((rgbBg >> 16) & 0xFF) * ialpha) << 16);
        } while (p32 < p32_lineend);
    }
}

bool PainterXRGB8888::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red = painterRed;
    green = painterGreen;
    blue = painterBlue;
    alpha = 0xFF;
    return true;
}
} // namespace touchgfx
