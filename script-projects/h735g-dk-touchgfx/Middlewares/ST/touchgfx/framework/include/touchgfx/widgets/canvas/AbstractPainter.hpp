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
 * @file touchgfx/widgets/canvas/AbstractPainter.hpp
 *
 * Declares the touchgfx::AbstractPainter class.
 */
#ifndef TOUCHGFX_ABSTRACTPAINTER_HPP
#define TOUCHGFX_ABSTRACTPAINTER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/**
 * An abstract class for creating painter classes for drawing canvas widgets. All canvas widgets
 * need a painter to fill the shape drawn with a CanvasWidgetRenderer. The painter must
 * provide the color of a pixel on a given coordinate, which will the be blended into
 * the framebuffer depending on the position of the canvas widget and the transparency
 * of the given pixel.
 *
 * The AbstractPainter also implements a function which will blend each pixel in a
 * scanline snippet into the framebuffer, but for better performance, the function
 * should be reimplemented in each painter.
 */
class AbstractPainter
{
public:
    /** Initializes a new instance of the AbstractPainter class. */
    AbstractPainter()
        : areaOffsetX(0),
          areaOffsetY(0),
          widgetAlpha(255)
    {
    }

    /** Finalizes an instance of the AbstractPainter class. */
    virtual ~AbstractPainter()
    {
    }

    /**
     * Paint a designated part of the RenderingBuffer with respect to the amount of coverage
     * of each pixel given by the parameter covers. The cover is the alpha for each pixel,
     * which is what makes it possible to have smooth anti-aliased edges on the shapes drawn
     * with CanvasWidgetRenderer.
     *
     * @param [in] ptr     Pointer to the row in the RenderingBuffer.
     * @param      x       The x coordinate.
     * @param      xAdjust The minor adjustment of x (used when a pixel is smaller than a byte
     *                     to specify that the \a ptr should have been advanced
     *                     "xAdjust" pixels futher into the byte).
     * @param      y       The y coordinate.
     * @param      count   Number of pixels to fill.
     * @param      covers  The coverage in of each pixel.
     *
     * @note The implementation of render() in the AbstractPainter classes is a generic (i.e. slow)
     *       implementation that should be completely implemented in subclasses of
     *       AbstractPainter for better performance.
     */
    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers) = 0;

protected:
    int16_t areaOffsetX; ///< The offset x coordinate of the area being drawn.
    int16_t areaOffsetY; ///< The offset y coordinate of the area being drawn.
    uint8_t widgetAlpha; ///< The alpha of the widget using the painter.

    /**
     * Sets the offset of the area being drawn. This allows render() to calculate the x, y
     * relative to the widget, and not just relative to the invalidated area.
     *
     * @param  offsetX The offset x coordinate of the invalidated area relative to the
     *                 widget.
     * @param  offsetY The offset y coordinate of the invalidated area relative to the
     *                 widget.
     *
     * @note Used by CanvasWidgetRenderer - should not be overwritten.
     */
    void setAreaOffset(uint16_t offsetX, uint16_t offsetY)
    {
        areaOffsetX = offsetX;
        areaOffsetY = offsetY;
    }

    /**
     * Sets the widget alpha to allow an entire canvas widget to easily be faded without
     * changing the painter of the widget.
     *
     * @param  alpha The alpha.
     *
     * @note Used internally by CanvasWidgetRenderer.
     */
    void setWidgetAlpha(const uint8_t alpha)
    {
        widgetAlpha = alpha;
    }

    /**
     * Helper function to check if the provided bitmap format matches the current
     * framebuffer format.
     *
     * @param  format A bitmap format.
     *
     * @return True if the format matches the framebuffer format, false otherwise.
     */
    FORCE_INLINE_FUNCTION static bool compatibleFramebuffer(Bitmap::BitmapFormat format)
    {
        bool compat = HAL::lcd().framebufferFormat() == format;
        if (HAL::getInstance()->getAuxiliaryLCD())
        {
            compat |= HAL::getInstance()->getAuxiliaryLCD()->framebufferFormat() == format;
        }
        return compat;
    }

    friend class Canvas;
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTPAINTER_HPP
