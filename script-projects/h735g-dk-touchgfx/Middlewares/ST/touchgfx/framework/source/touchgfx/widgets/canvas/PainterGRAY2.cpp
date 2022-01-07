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

#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/PainterGRAY2.hpp>
#include <platform/driver/lcd/LCD2bpp.hpp>

namespace touchgfx
{
void PainterGRAY2::render(uint8_t* ptr,
                          int x,
                          int xAdjust,
                          int y,
                          unsigned count,
                          const uint8_t* covers)
{
    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;
    x += xAdjust;
    if (widgetAlpha == 0xFF)
    {
        do
        {
            const uint8_t alpha = *covers++;

            if (alpha == 0xFF)
            {
                // Render a solid pixel
                LCD2bpp::setPixel(ptr, x, painterGray);
            }
            else
            {
                const uint8_t ialpha = 0xFF - alpha;
                const uint8_t p_gray = LCD2bpp::getPixel(ptr, x);
                LCD2bpp::setPixel(ptr, x, LCD::div255((painterGray * alpha + p_gray * ialpha) * 0x55) >> 6);
            }
            currentX++;
            x++;
        } while (--count != 0);
    }
    else
    {
        do
        {
            const uint8_t alpha = LCD::div255((*covers++) * widgetAlpha);
            const uint8_t ialpha = 0xFF - alpha;

            const uint8_t p_gray = LCD2bpp::getPixel(ptr, x);
            LCD2bpp::setPixel(ptr, x, LCD::div255((painterGray * alpha + p_gray * ialpha) * 0x55) >> 6);
            currentX++;
            x++;
        } while (--count != 0);
    }
}

bool PainterGRAY2::renderNext(uint8_t& gray, uint8_t& alpha)
{
    gray = painterGray;
    alpha = 0xFF;
    return true;
}
} // namespace touchgfx
