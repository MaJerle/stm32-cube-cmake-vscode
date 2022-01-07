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

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/Button.hpp>

namespace touchgfx
{
void Button::draw(const Rect& invalidatedArea) const
{
    Bitmap bmp(pressed ? down : up);
    Rect dirty(0, 0, bmp.getWidth(), bmp.getHeight());
    dirty &= invalidatedArea;
    if ((bmp.getId() != BITMAP_INVALID) && !dirty.isEmpty())
    {
        Rect r;
        translateRectToAbsolute(r);
        HAL::lcd().drawPartialBitmap(bmp, r.x, r.y, dirty, alpha);
    }
}

void Button::setBitmaps(const Bitmap& bitmapReleased, const Bitmap& bitmapPressed)
{
    up = bitmapReleased;
    down = bitmapPressed;
    // Adjust width and height of this widget to match bitmap. It is assumed
    // that the two bitmaps have same dimensions.
    Button::setWidthHeight(down);
}

Rect Button::getSolidRect() const
{
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }

    return (pressed ? down.getSolidRect() : up.getSolidRect());
}
} // namespace touchgfx
