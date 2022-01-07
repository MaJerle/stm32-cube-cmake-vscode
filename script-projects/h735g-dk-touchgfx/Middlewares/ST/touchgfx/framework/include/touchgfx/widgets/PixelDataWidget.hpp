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
 * @file touchgfx/widgets/PixelDataWidget.hpp
 *
 * Declares the touchgfx::PixelDataWidget class.
 */
#ifndef TOUCHGFX_PIXELDATAWIDGET_HPP
#define TOUCHGFX_PIXELDATAWIDGET_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
/**
 * A widget for displaying a buffer of pixel data. This can also be though of as a dynamic
 * bitmap where the dimensions of the bitmap is the same as the dimensions of the widget
 * and the actual bitmap data can be set and updated dynamically. The size of the buffer
 * must match the number of bytes required for the widget calculated as WIDTH x HEIGHT x
 * BYTES_PER_PIXEL. If the LCD is 16 bit per pixel the buffer must hold 2 bytes for each
 * pixel. If the LCD is 24 bit the buffer must hold 3 bytes for each pixel.
 */
class PixelDataWidget : public Widget
{
public:
    PixelDataWidget()
        : Widget(),
          buffer(0),
          format(Bitmap::RGB888),
          alpha(255)
    {
    }

    virtual void draw(const Rect& invalidatedArea) const;

    virtual Rect getSolidRect() const;

    /**
     * Set the pixel data to display. The given pointer must contain WIDTH x HEIGHT x
     * BYTES_PER_PIXEL bytes of addressable image data.
     *
     * @param [in] data Image data.
     *
     * @see getPixelData, setBitmapFormat
     */
    void setPixelData(uint8_t* const data)
    {
        buffer = data;
    }

    /**
     * Get the pixel data memory pointer, previously set with setPixelData().
     *
     * @return  The pixel data.
     *
     * @see setPixelData, setBitmapFormat
     */
    uint8_t* getPixelData() const
    {
        return buffer;
    }

    /**
     * Set the format of the pixel data. The supported formats depend on the display type. For
     * example grayscale displays do not support color images.
     *
     * @param   bitmapFormat    Describes the format to use when reading the pixel data.
     *
     * @see getBitmapFormat
     */
    void setBitmapFormat(Bitmap::BitmapFormat bitmapFormat)
    {
        format = bitmapFormat;
    }

    /**
     * Get the format of the pixel data previously set using setBitmapFormat().
     *
     * @return  The bitmap format.
     *
     * @see setBitmapFormat
     */
    Bitmap::BitmapFormat getBitmapFormat() const
    {
        return format;
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
    uint8_t getAlpha() const
    {
        return alpha;
    }

protected:
    uint8_t* buffer;             ///< The buffer where the pixels are copied from
    Bitmap::BitmapFormat format; ///< The pixel format for the data.
    uint8_t alpha;               ///< The Alpha for this widget.
};

} // namespace touchgfx

#endif // TOUCHGFX_PIXELDATAWIDGET_HPP
