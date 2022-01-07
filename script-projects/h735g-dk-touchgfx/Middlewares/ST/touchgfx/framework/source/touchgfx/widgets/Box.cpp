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
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/Box.hpp>

namespace touchgfx
{
Rect Box::getSolidRect() const
{
    Rect solidRect(0, 0, 0, 0);
    if (alpha == 255)
    {
        solidRect.width = rect.width;
        solidRect.height = rect.height;
    }
    return solidRect;
}

void Box::draw(const Rect& area) const
{
    Rect dirty = area;
    translateRectToAbsolute(dirty);
    HAL::lcd().fillRect(dirty, color, alpha);
}
} // namespace touchgfx
