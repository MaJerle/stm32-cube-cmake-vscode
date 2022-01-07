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
 * @file touchgfx/containers/buttons/TiledImageButtonStyle.hpp
 *
 * Declares the touchgfx::TiledImageButtonStyle class.
 */
#ifndef TOUCHGFX_TILEDIMAGEBUTTONSTYLE_HPP
#define TOUCHGFX_TILEDIMAGEBUTTONSTYLE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>
#include <touchgfx/widgets/TiledImage.hpp>

namespace touchgfx
{
/**
 * A tiled image button style.
 *
 * An tiled image button style. This class is supposed to be used with one of the
 * ButtonTrigger classes to create a functional button. This class will show one of two
 * tiled images depending on the state of the button (pressed or released).
 *
 * The TiledImageButtonStyle does not set the size of the enclosing container (normally
 * AbstractButtonContainer) to the size of the pressed Bitmap. This can be overridden by
 * calling setWidth/setHeight after setting the bitmaps.
 *
 * @tparam T Generic type parameter. Typically a AbstractButtonContainer subclass.
 *
 * @see AbstractButtonContainer
 */
template <class T>
class TiledImageButtonStyle : public T
{
public:
    TiledImageButtonStyle()
        : T()
    {
        tiledImage.setXY(0, 0);
        T::add(tiledImage);
    }

    /**
     * Sets width.
     *
     * @param  width The width.
     */
    virtual void setWidth(int16_t width)
    {
        tiledImage.setWidth(width);
        T::setWidth(width);
    }

    /**
     * Sets height.
     *
     * @param  height The height.
     */
    virtual void setHeight(int16_t height)
    {
        tiledImage.setHeight(height);
        T::setHeight(height);
    }

    /**
     * Sets tile bitmaps.
     *
     * @param  bmpReleased The bitmap released.
     * @param  bmpPressed  The bitmap pressed.
     */
    virtual void setTileBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed)
    {
        upTile = bmpReleased;
        downTile = bmpPressed;
        AbstractButtonContainer::setWidthHeight(downTile);

        handlePressedUpdated();
    }

    /**
     * Sets an offset into the bitmap where the tile drawing should start.
     *
     * @param  x The x coordinate offset.
     * @param  y The y coordinate offset.
     * @see TiledImage::setOffset
     */
    virtual void setTileOffset(int16_t x, int16_t y)
    {
        tiledImage.setOffset(x, y);
    }

protected:
    TiledImage tiledImage; ///< The tiled image
    Bitmap upTile;         ///< The image to display when button is released.
    Bitmap downTile;       ///< The image to display when button is pressed.

    /** @copydoc AbstractButtonContainer::handlePressedUpdated() */
    virtual void handlePressedUpdated()
    {
        int16_t buttonWidth = AbstractButtonContainer::getWidth();
        int16_t buttonHeight = AbstractButtonContainer::getHeight();

        tiledImage.setBitmap(T::getPressed() ? downTile : upTile);
        tiledImage.setWidthHeight(buttonWidth, buttonHeight);
        T::handlePressedUpdated();
    }

    /** @copydoc AbstractButtonContainer::handleAlphaUpdated() */
    virtual void handleAlphaUpdated()
    {
        tiledImage.setAlpha(T::getAlpha());
        T::handleAlphaUpdated();
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_TILEDIMAGEBUTTONSTYLE_HPP
