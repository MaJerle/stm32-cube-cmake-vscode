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
 * @file touchgfx/widgets/Box.hpp
 *
 * Declares the touchgfx::Box class.
 */
#ifndef TOUCHGFX_BOX_HPP
#define TOUCHGFX_BOX_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
/**
 * Simple widget capable of showing a rectangle of a specific color and an optional alpha.
 */
class Box : public Widget
{
public:
    /** Construct a new Box with a default alpha value of 255 (solid) */
    Box()
        : Widget(), alpha(255), color(0)
    {
    }

    /**
     * Construct a Box with the given size and color (and optionally alpha).
     *
     * @param  width  The width of the box.
     * @param  height The height of the box.
     * @param  color  The color of the box.
     * @param  alpha  (Optional) The alpha of the box. Default is 255 (solid).
     */
    Box(uint16_t width, uint16_t height, colortype color, uint8_t alpha = 255)
        : Widget(),
          alpha(alpha), color(color)
    {
        rect.width = width;
        rect.height = height;
    }

    virtual Rect getSolidRect() const;

    /**
     * Sets the color of the Box.
     *
     * @param  color The color of the box.
     *
     * @see getColor, Color::getColorFrom24BitRGB
     */
    void setColor(colortype color)
    {
        this->color = color;
    }

    /**
     * Gets the current color of the Box.
     *
     * @return The current color of the box.
     *
     * @see setColor, Color::getRed, Color::getGreen, Color::getRed
     */
    FORCE_INLINE_FUNCTION colortype getColor() const
    {
        return color;
    }

    /**
     * @copydoc Image::setAlpha
     */
    void setAlpha(uint8_t newAlpha)
    {
        alpha = newAlpha;
    }

    /**
     * @copydoc Image::getAlpha
     */
    FORCE_INLINE_FUNCTION uint8_t getAlpha() const
    {
        return alpha;
    }

    virtual void draw(const Rect& area) const;

protected:
    uint8_t alpha;   ///< The alpha value used for this Box.
    colortype color; ///< The fill color for this Box
};

} // namespace touchgfx

#endif // TOUCHGFX_BOX_HPP
