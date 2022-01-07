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

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/containers/CacheableContainer.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
CacheableContainer::CacheableContainer()
    : Container(),
      cachedBitmapId(BITMAP_INVALID),
      cachedImage(),
      isCachedMode(false),
      childWasInvalidated(false)
{
}

void CacheableContainer::setCacheBitmap(BitmapId bitmapId)
{
    Bitmap bitmap(bitmapId);

    cachedBitmapId = BITMAP_INVALID;

    /* Accept only dynamic bitmaps */
    if (!Bitmap::dynamicBitmapGetAddress(bitmapId))
    {
        return;
    }

    /* Retrieve the auxiliary LCD implementation attached to HA */
    LCD* auxLCD = HAL::getInstance()->getAuxiliaryLCD();

    /* Dynamic bitmap and framebuffer should be of the same format */
    if (bitmap.getFormat() == HAL::lcd().framebufferFormat() ||
        (auxLCD && bitmap.getFormat() == auxLCD->framebufferFormat()))
    {
        cachedBitmapId = bitmapId;
        cachedImage.setBitmap(Bitmap(cachedBitmapId));
    }
}

BitmapId CacheableContainer::getCacheBitmap() const
{
    return cachedBitmapId;
}

void CacheableContainer::updateCache()
{
    updateCache(Rect());
}

void CacheableContainer::updateCache(const Rect& invalidatedArea)
{
    if (isCachedMode && (cachedBitmapId != BITMAP_INVALID))
    {
        /* will call Container::draw(invalidatedArea) to render all widgets into the dynamic bitmap */
        HAL::getInstance()->drawDrawableInDynamicBitmap(*this, cachedBitmapId, invalidatedArea);
        childWasInvalidated = false;
    }
}

void CacheableContainer::enableCachedMode(bool enable)
{
    isCachedMode = enable;
}

void CacheableContainer::invalidateRect(Rect& invalidatedArea) const
{
    Container::invalidateRect(invalidatedArea);

    if (isCachedMode && (cachedBitmapId != BITMAP_INVALID))
    {
        const_cast<CacheableContainer*>(this)->childWasInvalidated = true;
    }
}

bool CacheableContainer::setSolidRect(const Rect& rect)
{
    return Bitmap::dynamicBitmapSetSolidRect(cachedBitmapId, rect);
}

bool CacheableContainer::isChildInvalidated() const
{
    return childWasInvalidated;
}

void CacheableContainer::setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
{
    if (isCachedMode && (cachedBitmapId != BITMAP_INVALID) && isVisible())
    {
        Rect r = getAbsoluteRect();
        cachedImage.setPosition(r.x, r.y, r.width, r.height);
        cachedImage.setupDrawChain(invalidatedArea, nextPreviousElement);
    }
    else
    {
        Container::setupDrawChain(invalidatedArea, nextPreviousElement);
    }
}
} // namespace touchgfx
