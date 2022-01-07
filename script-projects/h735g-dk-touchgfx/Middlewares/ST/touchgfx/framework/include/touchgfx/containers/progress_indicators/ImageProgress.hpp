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
 * @file touchgfx/containers/progress_indicators/ImageProgress.hpp
 *
 * Declares the touchgfx::ImageProgress class.
 */
#ifndef TOUCHGFX_IMAGEPROGRESS_HPP
#define TOUCHGFX_IMAGEPROGRESS_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>
#include <touchgfx/widgets/TiledImage.hpp>

namespace touchgfx
{
/**
 * An image progress will show parts of an image as a progress indicator. The image can progress
 * from the left, the right, the bottom or the top of the given area, and can visually
 * be fixed with a larger and larger portion of the image showing, or it can be moved
 * into view.
 */
class ImageProgress : public AbstractDirectionProgress
{
public:
    ImageProgress();

    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * Sets anchor at zero.
     *
     * Setting anchor at zero will force the image will be placed so that it is not moved
     * during progress, only more and more of the image will become visible. If the image is
     * not anchored at zero, it will be anchored at the current progress and will appear to
     * slide into view.
     *
     * @param  anchorAtZero true to anchor at zero, false to anchor at current progress.
     *
     * @see getAnchorAtZero
     */
    virtual void setAnchorAtZero(bool anchorAtZero);

    /**
     * Gets the current anchor at zero setting.
     *
     * @return true if the image is anchored at zero, false if it is anchored at current
     *         progress.
     *
     * @see setAnchorAtZero
     */
    virtual bool getAnchorAtZero() const;

    /**
     * Sets the bitmap id to use for progress. Please note that the bitmap is tiled which
     * will allow smaller bitmaps to repeat on the display and save memory.
     *
     * @param  bitmapId The bitmap id.
     *
     * @see getBitmap, TiledImage
     */
    virtual void setBitmap(BitmapId bitmapId);

    /**
     * Gets the bitmap id of the current image.
     *
     * @return The image.
     *
     * @see setBitmap
     */
    virtual BitmapId getBitmap() const;

    /**
     * @copydoc Image::setAlpha
     */
    virtual void setAlpha(uint8_t newAlpha);

    virtual void setValue(int value);

protected:
    Container container; ///< The container for the image to allow for anchored images
    TiledImage image;    ///< The image
    bool fixedPosition;  ///< true if the image should not move during progress
};

} // namespace touchgfx

#endif // TOUCHGFX_IMAGEPROGRESS_HPP
