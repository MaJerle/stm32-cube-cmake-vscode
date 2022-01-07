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
 * @file touchgfx/widgets/canvas/PainterARGB8888.hpp
 *
 * Declares the touchgfx::PainterARGB8888 class.
 */
#ifndef TOUCHGFX_PAINTERARGB8888_HPP
#define TOUCHGFX_PAINTERARGB8888_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterARGB8888.hpp>

namespace touchgfx
{
/**
 * The PainterARGB8888 class allows a shape to be filled with a given color
 * value. This allows anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class PainterARGB8888 : public AbstractPainterARGB8888
{
public:
    /**
     * Initializes a new instance of the PainterARGB8888 class.
     *
     * @param  color (Optional) the color, default is black.
     */
    PainterARGB8888(colortype color = 0)
        : AbstractPainterARGB8888(), painterRed(0), painterGreen(0), painterBlue(0)
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
        painterColor = color;
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
        return painterColor;
    }

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    virtual bool renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha);

    colortype painterColor; ///< The painter color
    uint8_t painterRed;     ///< The red part of the color
    uint8_t painterGreen;   ///< The green part of the color
    uint8_t painterBlue;    ///< The blue part of the color
};

} // namespace touchgfx

#endif // TOUCHGFX_PAINTERARGB8888_HPP
