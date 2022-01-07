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
 * @file touchgfx/widgets/canvas/AbstractPainterARGB8888.hpp
 *
 * Declares the touchgfx::AbstractPainterARGB8888 class.
 */
#ifndef TOUCHGFX_ABSTRACTPAINTERARGB8888_HPP
#define TOUCHGFX_ABSTRACTPAINTERARGB8888_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>

namespace touchgfx
{
/**
 * The AbstractPainterARGB8888 class is an abstract class for creating a painter to draw on a
 * ARGB8888 display using CanvasWidgetRenderer.
 *
 * @see AbstractPainter
 */
class AbstractPainterARGB8888 : public AbstractPainter
{
public:
    AbstractPainterARGB8888()
        : AbstractPainter(), currentX(0), currentY(0)
    {
        assert(compatibleFramebuffer(Bitmap::ARGB8888) && "The chosen painter only works with ARGB8888 displays");
    }

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    /**
     * @copydoc AbstractPainterABGR2222::renderInit()
     */
    virtual bool renderInit()
    {
        return true;
    }

    /**
     * @copydoc AbstractPainterABGR2222::renderNext(uint8_t&,uint8_t&,uint8_t&,uint8_t&)
     */
    virtual bool renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
    {
        return false;
    }

    /**
     * @copydoc AbstractPainterABGR2222::renderPixel(uint8_t*,uint8_t,uint8_t,uint8_t)
     *
     * @note Will set the alpha value to 255 (solid)
     */
    virtual void renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @copydoc AbstractPainterABGR2222::renderPixel(uint8_t*,uint8_t,uint8_t,uint8_t)
     *
     * @param          alpha The alpha.
     *
     * @note The \a alpha value is written to the 32bit framebuffer, just like the color is.
     */
    virtual void renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    int currentX; ///< Current x coordinate relative to the widget
    int currentY; ///< Current y coordinate relative to the widget
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTPAINTERARGB8888_HPP
