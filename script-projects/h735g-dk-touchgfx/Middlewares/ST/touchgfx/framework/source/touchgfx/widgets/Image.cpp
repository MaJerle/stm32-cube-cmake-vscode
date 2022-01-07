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
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
void Image::setBitmap(const Bitmap& bitmap)
{
    this->bitmap = bitmap;
    // When setting bitmap, adjust size of this widget to match.
    setWidthHeight(bitmap);
}

void Image::draw(const Rect& invalidatedArea) const
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

Rect Image::getSolidRect() const
{
    // If alpha is less than solid, we have an empty solid rect.
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }
    // Return solid rect from bitmap (precalculated).
    return bitmap.getSolidRect();
}
} // namespace touchgfx
