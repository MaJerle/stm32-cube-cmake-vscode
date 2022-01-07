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

/**
 * @file touchgfx/widgets/canvas/PainterBW.hpp
 *
 * Declares the touchgfx::PainterBW class.
 */
#ifndef TOUCHGFX_PAINTERBW_HPP
#define TOUCHGFX_PAINTERBW_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterBW.hpp>
#include <platform/driver/lcd/LCD1bpp.hpp>

namespace touchgfx
{
/**
 * PainterBW is used for drawing one 1bpp displays. The color is either on or off. No
 * transparency is supported.
 *
 * @see AbstractPainter
 */
class PainterBW : public AbstractPainterBW
{
public:
    /**
     * Converts the selected color to either white (1) or black (0) depending on the gray
     * representation of the RGB color.
     *
     * @param  red   The red color.
     * @param  green The green color.
     * @param  blue  The blue color.
     *
     * @return 1 (white) if the brightness of the RGB color is above 50% and 0 (black)
     *         otherwise.
     */
    static unsigned bw(unsigned red, unsigned green, unsigned blue)
    {
        return (red * 77 + green * 150 + blue * 29) >> 15;
    }

    /**
     * Sets color to use when drawing the CanvasWidget.
     *
     * @param  color The color
     */
    void setColor(colortype color)
    {
        painterColor = color;
        painterBW = LCD1bpp::getNativeColor(color);
    }

    /**
     * Gets the current color.
     *
     * @return The color.
     */
    colortype getColor() const
    {
        return painterColor;
    }

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    virtual bool renderNext(uint8_t& color);

    colortype painterColor; ///< The painter color
    uint8_t painterBW;      ///< The color to use when painting
};

} // namespace touchgfx

#endif // TOUCHGFX_PAINTERBW_HPP
