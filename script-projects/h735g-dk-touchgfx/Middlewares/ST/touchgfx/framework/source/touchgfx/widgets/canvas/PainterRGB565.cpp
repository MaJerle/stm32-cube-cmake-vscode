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
#include <touchgfx/Color.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>

namespace touchgfx
{
void PainterRGB565::render(uint8_t* ptr, int x, int xAdjust, int /*y*/, unsigned count, const uint8_t* covers)
{
    uint16_t* p = reinterpret_cast<uint16_t*>(ptr) + (x + xAdjust);
    const uint16_t* const p_lineend = p + count;
    const uint16_t color565 = LCD16bpp::getNativeColor(painterColor);
    if (widgetAlpha == 0xFF)
    {
        do
        {
            const uint8_t alpha = *covers++;
            if (alpha == 0xFF)
            {
                *p = color565;
            }
            else
            {
                *p = mixColors(color565, *p, alpha);
            }
            p++;
        } while (p < p_lineend);
    }
    else
    {
        do
        {
            const uint8_t alpha = LCD::div255((*covers++) * widgetAlpha);
            if (alpha == 0xFF)
            {
                *p = color565;
            }
            else
            {
                *p = mixColors(color565, *p, alpha);
            }
            p++;
        } while (p < p_lineend);
    }
}

bool PainterRGB565::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    red = Color::getRed(painterColor);
    green = Color::getGreen(painterColor);
    blue = Color::getBlue(painterColor);
    alpha = 0xFF;
    return true;
}
} // namespace touchgfx
