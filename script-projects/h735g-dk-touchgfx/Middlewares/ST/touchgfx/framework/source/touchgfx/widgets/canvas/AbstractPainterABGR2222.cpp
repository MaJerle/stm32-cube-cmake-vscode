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
#include <touchgfx/widgets/canvas/AbstractPainterABGR2222.hpp>
#include <platform/driver/lcd/LCD8bpp_ABGR2222.hpp>

namespace touchgfx
{
void AbstractPainterABGR2222::render(uint8_t* ptr,
                                     int x,
                                     int xAdjust,
                                     int y,
                                     unsigned count,
                                     const uint8_t* covers)
{
    uint8_t* p = ptr + (x + xAdjust);
    const uint8_t* const p_lineend = p + count;

    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;
    if (renderInit())
    {
        do
        {
            uint8_t red, green, blue, alpha;
            if (renderNext(red, green, blue, alpha))
            {
                const uint8_t combinedAlpha = LCD::div255((*covers) * LCD::div255(alpha * widgetAlpha));

                if (combinedAlpha == 0xFF) // max alpha=255 on "*covers" and max alpha=255 on "widgetAlpha"
                {
                    // Render a solid pixel
                    renderPixel(p, red, green, blue);
                }
                else
                {
                    const uint8_t ialpha = 0xFF - combinedAlpha;
                    const uint8_t p_red = LCD8bpp_ABGR2222::getRedFromNativeColor(*p);
                    const uint8_t p_green = LCD8bpp_ABGR2222::getGreenFromNativeColor(*p);
                    const uint8_t p_blue = LCD8bpp_ABGR2222::getBlueFromNativeColor(*p);
                    renderPixel(p,
                                LCD::div255(red * combinedAlpha + p_red * ialpha),
                                LCD::div255(green * combinedAlpha + p_green * ialpha),
                                LCD::div255(blue * combinedAlpha + p_blue * ialpha));
                }
            }
            covers++;
            p++;
            currentX++;
        } while (p < p_lineend);
    }
}

void AbstractPainterABGR2222::renderPixel(uint8_t* p, uint8_t red, uint8_t green, uint8_t blue)
{
    *p = LCD8bpp_ABGR2222::getNativeColorFromRGB(red, green, blue);
}
} // namespace touchgfx
