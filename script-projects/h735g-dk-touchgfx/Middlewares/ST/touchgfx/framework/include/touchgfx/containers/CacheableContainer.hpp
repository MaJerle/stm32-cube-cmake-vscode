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
 * @file touchgfx/containers/CacheableContainer.hpp
 *
 * Declares the touchgfx::CacheableContainer class.
 */
#ifndef TOUCHGFX_CACHEABLECONTAINER_HPP
#define TOUCHGFX_CACHEABLECONTAINER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
/**
 * A CacheableContainer can be seen as a regular Container, i.e. a Drawable that can have child
 * nodes. The z-order of children is determined by the order in which Drawables are
 * added to the container - the Drawable added last will be front-most on the screen.
 *
 * The important difference is that a CacheableContainer can also render its content to
 * a dynamic bitmap which can then be used as a texture in subsequent drawing operations,
 * either as a simple Image or in a TextureMapper. If the bitmap format of the dynamic
 * bitmap differs from the format of the current LCD, the LCD from drawing the bitmap
 * must be setup using HAL::setAuxiliaryLCD().
 *
 * @see Container, Bitmap, Image, TextureMapper
 */
class CacheableContainer : public Container
{
public:
    CacheableContainer();

    /**
     * Set the dynamic bitmap into which the container content will be rendered. The format
     * of the bitmap must be the same as the current LCD or the same as the auxiliary LCD
     * setup using HAL::setAuxiliaryLCD.
     *
     * @param  bitmapId Id of the dynamic bitmap to serve as a render target.
     *
     * @see updateCache, getCacheBitmap, HAL::setAuxiliaryLCD
     */
    void setCacheBitmap(BitmapId bitmapId);

    /**
     * Get the dynamic bitmap used by the CacheableContainer.
     *
     * @return the id of the assigned bitmap or BITMAP_INVALID if no bitmap has been assigned.
     *
     * @see setCacheBitmap
     */
    BitmapId getCacheBitmap() const;

    /**
     * Render the container into the attached dynamic bitmap.
     *
     * @see setCacheBitmap
     */
    void updateCache();

    /**
     * Render the container into the attached dynamic bitmap. Only the specified Rect region
     * is updated.
     *
     * @param  rect Region to update.
     *
     * @see setCacheBitmap
     */
    void updateCache(const Rect& rect);

    /**
     * Toggle cached mode on and off. The CacheableContainer behaves just like a regular
     * Container when cached mode is turned off.
     *
     * @param  enable Enable or disable cached mode.
     */
    void enableCachedMode(bool enable);

    /**
     * Request that a subregion of this drawable is redrawn. Will recursively traverse the
     * children. When this function returns, the specified invalidated area has been redrawn
     * for all appropriate Drawables covering the region.
     *
     * @param [in] invalidatedArea The area of this drawable to redraw expressed in coordinates
     *                             relative to its parent (e.g. to request a complete
     *                             redraw, invalidatedArea will be (0, 0, width, height).
     */
    virtual void invalidateRect(Rect& invalidatedArea) const;

    /**
     * Set the solid area on the dynamic bitmap assigned to the CacheableContainer.
     *
     * @param [in] rect The rectangle of th CacheableContainer that is solid.
     *
     * @return true if the operation succeeds, false otherwise.
     */
    bool setSolidRect(const Rect& rect);

    /**
     * Queries the CacheableContainer whether any child widget has been invalidated.
     *
     * @return True if a child widget has been invalidated and false otherwise.
     */
    bool isChildInvalidated() const;

    /**
     * @copydoc Image::setAlpha()
     *
     * @note The alpha is only applied when cached mode is enabled.
     *
     * @see enableCachedMode
     */
    void setAlpha(uint8_t newAlpha)
    {
        cachedImage.setAlpha(newAlpha);
    }

    /** @copydoc Image::getAlpha() */
    uint8_t getAlpha() const
    {
        return cachedImage.getAlpha();
    }

protected:
    /// @cond
    virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement);

    /**
     * A CachedImage is a specialized Image object that exposes the setupDrawChain() method.
     *
     * @see CacheableContainer, Image
     */
    class CachedImage : public Image
    {
    public:
        virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
        {
            Drawable::setupDrawChain(invalidatedArea, nextPreviousElement);
        }
    };
    /// @endcond

private:
    BitmapId cachedBitmapId;  ///< The BitmapId of the dynamic bitmap attached to the CacheableContainer
    CachedImage cachedImage;  ///< The CachedImage object used as a wrapper widget around the attached dynamic bitmap
    bool isCachedMode;        ///< Cached mode whether enabled or disabled
    bool childWasInvalidated; ///< A child widget has been invalidated. The flag is meaningful when isCachedMode is true.
};

} // namespace touchgfx

#endif // TOUCHGFX_CACHEABLECONTAINER_HPP
