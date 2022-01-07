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
 * @file touchgfx/widgets/canvas/PainterGRAY4.hpp
 *
 * Declares the touchgfx::PainterGRAY4 class.
 */
#ifndef TOUCHGFX_PAINTERGRAY4_HPP
#define TOUCHGFX_PAINTERGRAY4_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/canvas/AbstractPainterGRAY4.hpp>
#include <platform/driver/lcd/LCD4bpp.hpp>

namespace touchgfx
{
/**
 * The PainterGRAY4 class allows a shape to be filled with a given color and alpha
 * value. This allows transparent, anti-aliased elements to be drawn.
 *
 * @see AbstractPainter
 */
class PainterGRAY4 : public AbstractPainterGRAY4
{
public:
    /**
     * Initializes a new instance of the PainterGRAY4 class.
     *
     * @param  color (Optional) the color, default is black.
     */
    PainterGRAY4(colortype color = 0)
        : AbstractPainterGRAY4(), painterColor(0), painterGray(0)
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
        painterGray = LCD4bpp::getNativeColor(color);
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

#endif // TOUCHGFX_PAINTERGRAY4_HPP
