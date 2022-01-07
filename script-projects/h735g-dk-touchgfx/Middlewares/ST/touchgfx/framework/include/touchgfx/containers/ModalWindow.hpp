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
 * @file touchgfx/containers/ModalWindow.hpp
 *
 * Declares the touchgfx::ModalWindow class.
 */
#ifndef TOUCHGFX_MODALWINDOW_HPP
#define TOUCHGFX_MODALWINDOW_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
/**
 * Container for displaying a modal window and hijacking touch event and prevent them from
 * reaching the underlying view and widgets.
 *
 * The container has a background image and a surrounding box that acts as a shade on
 * top of the rest of the screen. The background image must be set (using
 * setBackground()) and the shade can be adjusted (using setShadeAlpha() and
 * setShadeColor()).
 *
 * The ModalWindow can either be used directly by adding widgets/containers to the
 * ModalWindow from your view or by sub-classing it if you need a specific ModalWindow
 * with predefined behavior across your application.
 *
 * The ModalWindow should be instantiated in the view class and added as the last
 * element (to always be on top, i.e. be modal). The ModalWindow will fill up the entire
 * screen so it should always be placed at x=0, y=0 on the display.
 *
 * To control the visibility of the ModalWindow use the show and hide methods.
 */
class ModalWindow : public Container
{
public:
    ModalWindow();

    /**
     * Sets the background of the actual window. The remaining area of the screen will be
     * covered by the shade. The background image is centered on the screen.
     *
     * @param  bmpId Identifier for the background bitmap.
     */
    virtual void setBackground(const BitmapId& bmpId);

    /**
     * Sets the background of the actual window. The remaining area of the screen will be
     * covered by the shade. The background image will be placed at the backgroundX and
     * backgroundY coordinate.
     *
     * @param  bmpId       Identifier for the bitmap.
     * @param  backgroundX The background x coordinate.
     * @param  backgroundY The background y coordinate.
     */
    virtual void setBackground(const BitmapId& bmpId, int16_t backgroundX, int16_t backgroundY);

    /**
     * Gets the width of the actual window (the background images). Whereas the getWidth()
     * method will return the width including the shade.
     *
     * @return The width of the actual window.
     */
    virtual uint16_t getBackgroundWidth() const;

    /**
     * Gets the height of the actual window (the background images). Whereas the getHeight()
     * method will return the height including the shade.
     *
     * @return The height of the actual window.
     */
    virtual uint16_t getBackgroundHeight() const;

    virtual void add(Drawable& d);

    virtual void remove(Drawable& d);

    /**
     * Sets the alpha value of the background shade. Default, if not set, is 96.
     *
     * @param  alpha The new alpha.
     */
    virtual void setShadeAlpha(uint8_t alpha);

    /**
     * Gets the alpha value of the background shade.
     *
     * @return The background shades alpha.
     */
    virtual uint8_t getShadeAlpha() const;

    /**
     * Sets the color of the background shade. Default is black.
     *
     * @param  color The new color.
     */
    virtual void setShadeColor(colortype color);

    /**
     * Gets the color of the background shade.
     *
     * @return The color of the background shade.
     */
    virtual colortype getShadeColor() const;

    /** Make the ModalWindow visible. */
    virtual void show();

    /** Make the ModalWindow invisible. */
    virtual void hide();

protected:
    Box backgroundShade;       ///< The background shade
    Container windowContainer; ///< The window container that defines the active container area where both the windowBackground and added drawables are placed.
    Image windowBackground;    ///< The window background
};

} // namespace touchgfx

#endif // TOUCHGFX_MODALWINDOW_HPP
