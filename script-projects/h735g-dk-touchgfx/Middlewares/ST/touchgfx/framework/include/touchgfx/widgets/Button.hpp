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
 * @file touchgfx/widgets/Button.hpp
 *
 * Declares the touchgfx::Button class.
 */
#ifndef TOUCHGFX_BUTTON_HPP
#define TOUCHGFX_BUTTON_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/widgets/AbstractButton.hpp>

namespace touchgfx
{
/**
 * A button with two images. One image showing the unpressed button and one image showing the
 * pressed state.
 */
class Button : public AbstractButton
{
public:
    Button()
        : AbstractButton(), up(), down(), alpha(255)
    {
    }

    virtual void draw(const Rect& invalidatedArea) const;

    /**
     * Sets the two bitmaps used by this button. One bitmap for the released (normal) state
     * and one bitmap for the pressed state. The images are expected to be of the same
     * dimensions, and the Button is resized to the dimensions of the pressed Bitmap.
     *
     * @param  bitmapReleased Bitmap to use when button is released.
     * @param  bitmapPressed  Bitmap to use when button is pressed.
     *
     * @note It is assumed that the dimensions of the bitmaps are the same. Unexpected (visual)
     *       behavior may be observed if the bitmaps are of different sizes.
     * @note The user code must call invalidate() in order to update the button on the display.
     */
    virtual void setBitmaps(const Bitmap& bitmapReleased, const Bitmap& bitmapPressed);

    virtual Rect getSolidRect() const;

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

    /**
     * Gets currently displayed bitmap. This depends on the current state of the button,
     * released (normal) or pressed.
     *
     * @return The bitmap currently displayed.
     */
    Bitmap getCurrentlyDisplayedBitmap() const
    {
        return (pressed ? down : up);
    }

protected:
    Bitmap up;     ///< The image to display when button is released (normal state).
    Bitmap down;   ///< The image to display when button is pressed.
    uint8_t alpha; ///< The current alpha value. 255=solid, 0=invisible.
};

} // namespace touchgfx

#endif // TOUCHGFX_BUTTON_HPP
