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
#include <touchgfx/widgets/canvas/AbstractPainterGRAY2.hpp>
#include <platform/driver/lcd/LCD2bpp.hpp>

namespace touchgfx
{
void AbstractPainterGRAY2::render(uint8_t* ptr,
                                  int x,
                                  int xAdjust,
                                  int y,
                                  unsigned count,
                                  const uint8_t* covers)
{
    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;
    x += xAdjust;
    if (renderInit())
    {
        do
        {
            uint8_t gray, alpha;
            if (renderNext(gray, alpha))
            {
                const uint8_t combinedAlpha = LCD::div255((*covers) * LCD::div255(alpha * widgetAlpha));

                if (combinedAlpha == 0xFF) // max alpha=0xFF on "*covers" and max alpha=0xFF on "widgetAlpha"
                {
                    // Render a solid pixel
                    renderPixel(ptr, x, gray);
                }
                else
                {
                    const uint8_t p_gray = LCD2bpp::getPixel(ptr, x) * 0x55;
                    const uint8_t ialpha = 0xFF - combinedAlpha;
                    renderPixel(ptr, x, LCD::div255((gray * combinedAlpha + p_gray * ialpha) * 0x55) >> 6);
                }
            }
            covers++;
            x++;
            currentX++;
        } while (--count != 0);
    }
}

void AbstractPainterGRAY2::renderPixel(uint8_t* p, uint16_t offset, uint8_t gray)
{
    LCD2bpp::setPixel(p, offset, gray);
}
} // namespace touchgfx
