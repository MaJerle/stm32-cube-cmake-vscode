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
 * @file touchgfx/widgets/canvas/AbstractPainterGRAY2.hpp
 *
 * Declares the touchgfx::AbstractPainterGRAY2 class.
 */
#ifndef TOUCHGFX_ABSTRACTPAINTERGRAY2_HPP
#define TOUCHGFX_ABSTRACTPAINTERGRAY2_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/canvas/AbstractPainter.hpp>

namespace touchgfx
{
/**
 * The AbstractPainterGRAY2 class is an abstract class for creating a painter to draw on a
 * GRAY2 display using CanvasWidgetRenderer.
 *
 * @see AbstractPainter
 */
class AbstractPainterGRAY2 : public AbstractPainter
{
public:
    AbstractPainterGRAY2()
        : AbstractPainter(), currentX(0), currentY(0)
    {
        assert(compatibleFramebuffer(Bitmap::GRAY2) && "The chosen painter only works with GRAY2 displays");
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
     * Get the color of the next pixel in the scan line to blend into the framebuffer.
     *
     * @param [out] gray  The gray color (0-3).
     * @param [out] alpha The alpha.
     *
     * @return true if the pixel should be painted, false otherwise.
     */
    virtual bool renderNext(uint8_t& gray, uint8_t& alpha)
    {
        return false;
    }

    /**
     * Renders (writes) the specified color into the framebuffer.
     *
     * @param [in] p      pointer into the framebuffer where the given color should be written.
     * @param      offset The offset to the pixel from the given pointer.
     * @param      gray   The gray color.
     */
    virtual void renderPixel(uint8_t* p, uint16_t offset, uint8_t gray);

    int currentX; ///< Current x coordinate relative to the widget
    int currentY; ///< Current y coordinate relative to the widget
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTPAINTERGRAY2_HPP
