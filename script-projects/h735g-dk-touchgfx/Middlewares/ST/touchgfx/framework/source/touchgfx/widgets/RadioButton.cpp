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
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/RadioButton.hpp>

namespace touchgfx
{
void RadioButton::draw(const Rect& invalidatedArea) const
{
    Bitmap bitmap = getCurrentlyDisplayedBitmap();
    if (bitmap.getId() != BITMAP_INVALID)
    {
        Rect meAbs;
        translateRectToAbsolute(meAbs); // To find our x and y coords in absolute.

        // Calculate intersection between bitmap rect and invalidated area.
        Rect dirtyBitmapArea = bitmap.getRect() & invalidatedArea;

        if (!dirtyBitmapArea.isEmpty())
        {
            HAL::lcd().drawPartialBitmap(bitmap, meAbs.x, meAbs.y, dirtyBitmapArea, alpha);
        }
    }
}

void RadioButton::handleClickEvent(const ClickEvent& event)
{
    bool wasPressed = pressed;
    pressed = (event.getType() == ClickEvent::PRESSED);
    if ((pressed && !wasPressed) || (!pressed && wasPressed))
    {
        invalidate();
    }
    if (wasPressed && (event.getType() == ClickEvent::RELEASED))
    {
        if (deselectionEnabled)
        {
            setSelected(!getSelected());
        }
        else if (!getSelected())
        {
            setSelected(true);
        }
    }
}

void RadioButton::setBitmaps(const Bitmap& bmpUnselected, const Bitmap& bmpUnselectedPressed, const Bitmap& bmpSelected, const Bitmap& bmpSelectedPressed)
{
    bitmapUnselected = bmpUnselected;
    bitmapUnselectedPressed = bmpUnselectedPressed;
    bitmapSelected = bmpSelected;
    bitmapSelectedPressed = bmpSelectedPressed;

    RadioButton::setWidthHeight(bitmapUnselected);
}

Rect RadioButton::getSolidRect() const
{
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }

    return getCurrentlyDisplayedBitmap().getSolidRect();
}

void RadioButton::setSelected(bool newSelected)
{
    bool wasSelected = selected;
    selected = newSelected;

    if (wasSelected && !newSelected)
    {
        executeDeselectedAction();
    }

    if (!wasSelected && newSelected)
    {
        executeAction();
    }

    invalidate();
}
} // namespace touchgfx
