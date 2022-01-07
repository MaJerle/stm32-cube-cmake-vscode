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
#include <touchgfx/widgets/ButtonWithIcon.hpp>

namespace touchgfx
{
ButtonWithIcon::ButtonWithIcon()
    : Button(),
      iconReleased(),
      iconPressed(),
      iconX(0),
      iconY(0)
{
}

void ButtonWithIcon::setBitmaps(const Bitmap& newBackgroundReleased, const Bitmap& newBackgroundPressed,
                                const Bitmap& newIconReleased, const Bitmap& newIconPressed)
{
    Button::setBitmaps(newBackgroundReleased, newBackgroundPressed);

    iconReleased = newIconReleased;
    iconPressed = newIconPressed;

    iconX = (getWidth() / 2) - (newIconPressed.getWidth() / 2);
    iconY = (getHeight() / 2) - (newIconPressed.getHeight() / 2);
}

void ButtonWithIcon::draw(const Rect& invalidatedArea) const
{
    Button::draw(invalidatedArea);

    Bitmap bmp(pressed ? iconPressed : iconReleased);
    Rect iconRect(iconX, iconY, bmp.getWidth(), bmp.getHeight());
    Rect dirty = invalidatedArea & iconRect;
    if ((bmp.getId() != BITMAP_INVALID) && !dirty.isEmpty())
    {
        Rect r;
        translateRectToAbsolute(r);
        dirty.x -= iconX;
        dirty.y -= iconY;
        HAL::lcd().drawPartialBitmap(bmp, r.x + iconX, r.y + iconY, dirty, alpha);
    }
}
} // namespace touchgfx
