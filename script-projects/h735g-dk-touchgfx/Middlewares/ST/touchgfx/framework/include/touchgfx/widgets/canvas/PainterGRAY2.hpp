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
 * @file touchgfx/widgets/canvas/PainterGRAY2.hpp
 *
 * Declares the touchgfx::PainterGRAY2 class.
 */
#ifndef TOUCHGFX_PAINTERGRAY2_HPP
#define TOUCHGFX_PAINTERGRAY2_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterGRAY2.hpp>
#include <platform/driver/lcd/LCD2bpp.hpp>

namespace touchgfx
{
/**
 * The PainterGRAY2 class allows a shape to be filled with a given color
 * value. This allows anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class PainterGRAY2 : public AbstractPainterGRAY2
{
public:
    /**
     * Initializes a new instance of the PainterGRAY2 class.
     *
     * @param  color (Optional) the color, default is black.
     */

    PainterGRAY2(colortype color = 0)
        : AbstractPainterGRAY2(), painterGray(0)
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
        painterGray = LCD2bpp::getNativeColor(color);
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
    virtual bool renderNext(uint8_t& gray, uint8_t& alpha);

    colortype painterColor; ///< The painter color
    uint8_t painterGray;    ///< The gray color
};

} // namespace touchgfx

#endif // TOUCHGFX_PAINTERGRAY2_HPP
