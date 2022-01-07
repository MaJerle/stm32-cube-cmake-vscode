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
 * @file touchgfx/widgets/canvas/PainterRGB888.hpp
 *
 * Declares the touchgfx::PainterRGB888 class.
 */
#ifndef TOUCHGFX_PAINTERRGB888_HPP
#define TOUCHGFX_PAINTERRGB888_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterRGB888.hpp>

namespace touchgfx
{
/**
 * The PainterRGB888 class allows a shape to be filled with a given color
 * value. This allows anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class PainterRGB888 : public AbstractPainterRGB888
{
public:
    /**
     * Initializes a new instance of the PainterRGB888 class.
     *
     * @param  color (Optional) the color, default is black.
     */
    PainterRGB888(colortype color = 0)
        : AbstractPainterRGB888(), painterRed(0), painterGreen(0), painterBlue(0)
    {
        setColor(color);
    }

    /**
     * Sets color to use when drawing the CanvasWidget.
     *
     * @param  color The color.
     */
    void setColor(colortype color)
    {
        painterRed = Color::getRed(color);
        painterGreen = Color::getGreen(color);
        painterBlue = Color::getBlue(color);
    }

    /**
     * Gets the current color.
     *
     * @return The color.
     */
    colortype getColor() const
    {
        return Color::getColorFrom24BitRGB(painterRed, painterGreen, painterBlue);
    }

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    virtual bool renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha);

    uint8_t painterRed;   ///< The red part of the color
    uint8_t painterGreen; ///< The green part of the color
    uint8_t painterBlue;  ///< The blue part of the color
};

} // namespace touchgfx

#endif // TOUCHGFX_PAINTERRGB888_HPP
