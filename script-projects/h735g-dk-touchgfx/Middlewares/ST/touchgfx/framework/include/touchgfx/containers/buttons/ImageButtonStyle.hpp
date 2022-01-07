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
 * @file touchgfx/containers/buttons/ImageButtonStyle.hpp
 *
 * Declares the touchgfx::ImageButtonStyle class.
 */
#ifndef TOUCHGFX_IMAGEBUTTONSTYLE_HPP
#define TOUCHGFX_IMAGEBUTTONSTYLE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
/**
 * An image button style. This class is supposed to be used with one of the ButtonTrigger
 * classes to create a functional button. This class will show one of two images
 * depending on the state of the button (pressed or released).
 *
 * The ImageButtonStyle will set the size of the enclosing container (normally
 * AbstractButtonContainer) to the size of the pressed Bitmap. This can be overridden by
 * calling setWidth/setHeight after setting the bitmaps.
 *
 * The position of the bitmap can be adjusted with setBitmapXY (default is upper left
 * corner).
 *
 * @tparam T Generic type parameter. Typically a AbstractButtonContainer subclass.
 *
 * @see AbstractButtonContainer
 */
template <class T>
class ImageButtonStyle : public T
{
public:
    ImageButtonStyle()
        : T(), up(), down()
    {
        buttonImage.setXY(0, 0);
        T::add(buttonImage);
    }

    /**
     * Sets the bitmaps.
     *
     * @param  bmpReleased The bitmap released.
     * @param  bmpPressed  The bitmap pressed.
     */
    virtual void setBitmaps(const Bitmap& bmpReleased, const Bitmap& bmpPressed)
    {
        up = bmpReleased;
        down = bmpPressed;
        ImageButtonStyle::setWidthHeight(down);

        handlePressedUpdated();
    }

    /**
     * Sets bitmap x and y.
     *
     * @param  x An uint16_t to process.
     * @param  y An uint16_t to process.
     */
    void setBitmapXY(uint16_t x, uint16_t y)
    {
        buttonImage.setXY(x, y);
    }

    /**
     * Gets currently displayed bitmap.
     *
     * @return The currently displayed bitmap.
     */
    Bitmap getCurrentlyDisplayedBitmap() const
    {
        return (AbstractButtonContainer::pressed ? down : up);
    }

protected:
    Image buttonImage; ///< The button image
    Bitmap up;         ///< The image to display when button is released.
    Bitmap down;       ///< The image to display when button is pressed.

    /** @copydoc AbstractButtonContainer::handlePressedUpdated() */
    virtual void handlePressedUpdated()
    {
        buttonImage.setBitmap(T::getPressed() ? down : up);
        T::handlePressedUpdated();
    }

    /** @copydoc AbstractButtonContainer::handleAlphaUpdated() */
    virtual void handleAlphaUpdated()
    {
        buttonImage.setAlpha(T::getAlpha());
        T::handleAlphaUpdated();
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_IMAGEBUTTONSTYLE_HPP
