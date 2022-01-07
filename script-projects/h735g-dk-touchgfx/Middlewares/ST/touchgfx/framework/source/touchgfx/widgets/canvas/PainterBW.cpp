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
#include <touchgfx/widgets/canvas/PainterBW.hpp>

namespace touchgfx
{
void PainterBW::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    currentX = x + areaOffsetX;
    currentY = y + areaOffsetY;
    x += xAdjust;
    unsigned char* p = ptr + (x / 8);

    if (widgetAlpha == 0xFF)
    {
        do
        {
            if (*covers++ >= 0x80)
            {
                unsigned pixel = 1 << (7 - (x % 8));
                if (painterBW)
                {
                    *p |= pixel;
                }
                else
                {
                    *p &= ~pixel;
                }
            }
            if (((++x) % 8) == 0)
            {
                p++;
            }
            currentX++;
        } while (--count);
    }
    else
    {
        do
        {
            if (widgetAlpha * *covers++ >= 0xFF * 0x80)
            {
                unsigned pixel = 1 << (7 - (x % 8));
                if (painterBW)
                {
                    *p |= pixel;
                }
                else
                {
                    *p &= ~pixel;
                }
            }
            if (((++x) % 8) == 0)
            {
                p++;
            }
            currentX++;
        } while (--count);
    }
}

bool PainterBW::renderNext(uint8_t& color)
{
    color = painterBW;
    return true;
}
} // namespace touchgfx
