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
 * @file touchgfx/widgets/canvas/AbstractPainterRGB888.hpp
 *
 * Declares the touchgfx::AbstractPainterRGB888 class.
 */
#ifndef TOUCHGFX_ABSTRACTPAINTERRGB888_HPP
#define TOUCHGFX_ABSTRACTPAINTERRGB888_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>

namespace touchgfx
{
/**
 * The AbstractPainterRGB888 class is an abstract class for creating a painter to draw on a
 * RGB888 display using CanvasWidgetRenderer.
 *
 * @see AbstractPainter
 */
class AbstractPainterRGB888 : public AbstractPainter
{
public:
    AbstractPainterRGB888()
        : AbstractPainter(), currentX(0), currentY(0)
    {
        assert(compatibleFramebuffer(Bitmap::RGB888) && "The chosen painter only works with RGB888 displays");
    }

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    /**
     * @copydoc AbstractPainterRGB565::renderInit()
     */
    virtual bool renderInit()
    {
        return true;
    }

    /**
     * @copydoc AbstractPainterRGB565::renderNext(uint8_t&,uint8_t&,uint8_t&,uint8_t&)
     */
    virtual bool renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
    {
        return false;
    }

    /**
     * @copydoc AbstractPainterRGB565::renderPixel(uint16_t*,uint8_t,uint8_t,uint8_t)
     */
    virtual void renderPixel(uint16_t* p, uint8_t red, uint8_t green, uint8_t blue);

    int currentX; ///< Current x coordinate relative to the widget
    int currentY; ///< Current y coordinate relative to the widget
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTPAINTERRGB888_HPP
