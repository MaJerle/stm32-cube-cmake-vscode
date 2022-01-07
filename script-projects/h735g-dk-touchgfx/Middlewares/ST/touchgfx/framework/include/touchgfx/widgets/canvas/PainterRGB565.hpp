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
 * @file touchgfx/widgets/canvas/PainterRGB565.hpp
 *
 * Declares the touchgfx::PainterRGB565 class.
 */
#ifndef TOUCHGFX_PAINTERRGB565_HPP
#define TOUCHGFX_PAINTERRGB565_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterRGB565.hpp>

namespace touchgfx
{
/**
 * The PainterRGB565 class allows a shape to be filled with a given color and alpha
 * value. This allows transparent, anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class PainterRGB565 : public AbstractPainterRGB565
{
public:
    /**
     * Initializes a new instance of the PainterRGB565 class.
     *
     * @param  color (Optional) the color, default is black.
     */
    PainterRGB565(colortype color = 0)
        : AbstractPainterRGB565(), painterColor(0)
    {
        setColor(color);
    }

    /**
     * Sets color and alpha to use when drawing the CanvasWidget.
     *
     * @param  color The color.
     */
    void setColor(colortype color)
    {
        painterColor = color;
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

    colortype painterColor; ///< The color
};

} // namespace touchgfx

#endif // TOUCHGFX_PAINTERRGB565_HPP
