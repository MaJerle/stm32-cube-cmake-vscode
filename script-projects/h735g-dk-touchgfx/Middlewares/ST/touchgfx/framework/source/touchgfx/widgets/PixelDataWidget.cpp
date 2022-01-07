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
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/PixelDataWidget.hpp>
#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
void PixelDataWidget::draw(const Rect& invalidatedArea) const
{
    if (!buffer)
    {
        return;
    }

    // Convert to lcd coordinates.
    const Rect absolute = getAbsoluteRect();
    // Copy to LCD
    HAL::lcd().blitCopy(buffer, format, absolute, invalidatedArea, alpha, false);
}

Rect PixelDataWidget::getSolidRect() const
{
    if (buffer && alpha == 255)
    {
        // There are at least some solid pixels
        switch (format)
        {
        case Bitmap::BW:     // 1-bit, black / white, no alpha channel
        case Bitmap::BW_RLE: // 1-bit, black / white, no alpha channel compressed with horizontal RLE
        case Bitmap::GRAY2:  // 2-bit, gray scale, no alpha channel
        case Bitmap::GRAY4:  // 4-bit, gray scale, no alpha channel
        case Bitmap::RGB565: // 16-bit, 5 bits for red, 6 bits for green, 5 bits for blue, no alpha channel
        case Bitmap::RGB888: // 16-bit, 5 bits for red, 6 bits for green, 5 bits for blue, no alpha channel
            // All solid pixels
            return Rect(0, 0, getWidth(), getHeight());
        case Bitmap::ARGB8888: // 32-bit, 8 bits for each of red, green, blue and alpha channel
        case Bitmap::ARGB2222: // 8-bit color
        case Bitmap::ABGR2222: // 8-bit color
        case Bitmap::RGBA2222: // 8-bit color
        case Bitmap::BGRA2222: // 8-bit color
        case Bitmap::L8:       // 8-bit indexed color
        case Bitmap::A4:       // 4-bit alpha level
        case Bitmap::CUSTOM:   // Custom pixel format
            // No knowledge about solid pixels
            break;
        }
    }
    // Empty rectangle
    return Rect();
}
} // namespace touchgfx
