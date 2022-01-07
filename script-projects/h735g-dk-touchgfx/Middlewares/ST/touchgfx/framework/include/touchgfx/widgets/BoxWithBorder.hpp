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
 * @file touchgfx/widgets/BoxWithBorder.hpp
 *
 * Declares the touchgfx::BoxWithBorder class.
 */
#ifndef TOUCHGFX_BOXWITHBORDER_HPP
#define TOUCHGFX_BOXWITHBORDER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Box.hpp>

namespace touchgfx
{
/**
 * The BoxWithBorder class is used to create objects that can draw a box with a border on the
 * display. The width of the border can be specified. If the border width is 0 the
 * BoxWithBorder will function just like a Box.
 */
class BoxWithBorder : public Box
{
public:
    BoxWithBorder()
        : Box(), borderColor(0), borderSize(0)
    {
    }

    /**
     * Constructor that allows specification of dimensions and colors of the BoxWithBorder.
     *
     * @param  width       The width.
     * @param  height      The height.
     * @param  color       The color.
     * @param  borderColor The border color.
     * @param  borderSize  Size of the border.
     * @param  alpha       (Optional) The alpha.
     */
    BoxWithBorder(uint16_t width, uint16_t height, colortype color, colortype borderColor, uint16_t borderSize, uint8_t alpha = 255)
        : Box(width, height, color, alpha), borderColor(borderColor), borderSize(borderSize)
    {
        rect.width = width;
        rect.height = height;
    }

    /**
     * Sets the color of the border drawn along the edge of the BoxWithBorder.
     *
     * @param  color The color of the border.
     * @see setColor, getBorderColor, Color::getColorFrom24BitRGB
     */
    void setBorderColor(colortype color)
    {
        borderColor = color;
    }

    /**
     * Gets the color of the border drawn along the edge of the BoxWithBorder.
     *
     * @return The color of the border.
     * @see setBorderColor, getColor, Color::getRed, Color::getGreen, Color::getRed
     */
    FORCE_INLINE_FUNCTION colortype getBorderColor() const
    {
        return borderColor;
    }

    /**
     * Sets the width of the border. If the width is set to 0, the BoxWithBorder will look
     * exactly like a Box.
     *
     * @param  size The width of the border.
     *
     * @see getBorderSize
     */
    void setBorderSize(uint16_t size)
    {
        borderSize = size;
    }

    /**
     * Gets the width of the border.
     *
     * @return The width of the border.
     *
     * @see setBorderSize
     */
    FORCE_INLINE_FUNCTION uint16_t getBorderSize() const
    {
        return borderSize;
    }

    virtual void draw(const Rect& area) const;

protected:
    colortype borderColor; ///< The color of the border along the edge
    uint16_t borderSize;   ///< Width of the border along the edge
};

} // namespace touchgfx

#endif // TOUCHGFX_BOXWITHBORDER_HPP
