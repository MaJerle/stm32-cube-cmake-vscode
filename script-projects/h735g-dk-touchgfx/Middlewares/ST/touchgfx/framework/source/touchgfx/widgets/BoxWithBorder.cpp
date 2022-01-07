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
#include <touchgfx/Drawable.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>

namespace touchgfx
{
void BoxWithBorder::draw(const Rect& area) const
{
    const Rect center = Rect(borderSize, borderSize, getWidth() - (2 * borderSize), getHeight() - (2 * borderSize));
    if (center.isEmpty())
    {
        Rect dirty = area;
        translateRectToAbsolute(dirty);
        HAL::lcd().fillRect(dirty, borderColor, alpha);
        return;
    }

    Rect dirty = area & center;
    Box::draw(dirty);

    if (borderSize == 0)
    {
        return;
    }

    Rect borders[4] = {
        Rect(0, 0, getWidth(), borderSize),                                                   // Upper
        Rect(0, getHeight() - borderSize, getWidth(), borderSize),                            // lower
        Rect(0, borderSize, borderSize, getHeight() - (2 * borderSize)),                      // left
        Rect(getWidth() - borderSize, borderSize, borderSize, getHeight() - (2 * borderSize)) // right
    };
    for (int i = 0; i < 4; i++)
    {
        Rect borderDirty = borders[i] & area;
        if (!borderDirty.isEmpty())
        {
            translateRectToAbsolute(borderDirty);
            HAL::lcd().fillRect(borderDirty, borderColor, alpha);
        }
    }
}
} // namespace touchgfx
