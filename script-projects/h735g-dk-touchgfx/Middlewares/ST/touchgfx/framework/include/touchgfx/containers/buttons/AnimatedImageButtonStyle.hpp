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
 * @file touchgfx/containers/buttons/AnimatedImageButtonStyle.hpp
 *
 * Declares the touchgfx::AnimatedImageButtonStyle class.
 */
#ifndef TOUCHGFX_ANIMATEDIMAGEBUTTONSTYLE_HPP
#define TOUCHGFX_ANIMATEDIMAGEBUTTONSTYLE_HPP

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>

namespace touchgfx
{
/**
 * An animated image button style.  An animated image button style. This class is supposed to be
 * used with one of the ButtonTrigger classes to create a functional button. This class
 * will show the first or last image of an animated image depending on the state of the
 * button (pressed or released). When the state changes the button will show the
 * sequence of images in forward or reversed order.
 *
 * The AnimatedImageButtonStyle will set the size of the enclosing container (normally
 * AbstractButtonContainer) to the size of the first Bitmap. This can be overridden by
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
class AnimatedImageButtonStyle : public T
{
public:
    AnimatedImageButtonStyle()
        : T(), buttonAnimatedImage()
    {
        buttonAnimatedImage.setXY(0, 0);
        T::add(buttonAnimatedImage);
    }

    /**
     * Sets the bitmaps.
     *
     * @param  bitmapStart The bitmap start.
     * @param  bitmapEnd   The bitmap end.
     */
    void setBitmaps(const Bitmap& bitmapStart, const Bitmap& bitmapEnd)
    {
        buttonAnimatedImage.setBitmaps(bitmapStart.getId(), bitmapEnd.getId());

        AbstractButtonContainer::setWidthHeight(bitmapStart);

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
        buttonAnimatedImage.setXY(x, y);
    }

    /**
     * Sets update ticks interval.
     *
     * @param  updateInterval The update interval.
     */
    void setUpdateTicksInterval(uint8_t updateInterval)
    {
        buttonAnimatedImage.setUpdateTicksInterval(updateInterval);
    }

protected:
    AnimatedImage buttonAnimatedImage; ///< The button animated image

    /** @copydoc AbstractButtonContainer::handlePressedUpdated() */
    virtual void handlePressedUpdated()
    {
        buttonAnimatedImage.startAnimation(AbstractButtonContainer::pressed, true, false);
        T::handlePressedUpdated();
    }

    /** @copydoc AbstractButtonContainer::handleAlphaUpdated() */
    virtual void handleAlphaUpdated()
    {
        buttonAnimatedImage.setAlpha(T::getAlpha());
        T::handleAlphaUpdated();
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_ANIMATEDIMAGEBUTTONSTYLE_HPP
