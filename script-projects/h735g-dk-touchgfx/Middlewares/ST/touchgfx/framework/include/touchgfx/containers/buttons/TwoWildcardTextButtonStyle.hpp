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
 * @file touchgfx/containers/buttons/TwoWildcardTextButtonStyle.hpp
 *
 * Declares the touchgfx::TwoWildcardTextButtonStyle class.
 */
#ifndef TOUCHGFX_TWOWILDCARDTEXTBUTTONSTYLE_HPP
#define TOUCHGFX_TWOWILDCARDTEXTBUTTONSTYLE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

namespace touchgfx
{
/**
 * A wildcard text button style.
 *
 * An wildcard text button style. This class is supposed to be used with one of the
 * ButtonTrigger classes to create a functional button. This class will show a text with
 * a wildcard in one of two colors depending on the state of the button (pressed or
 * released).
 *
 * The TwoWildcardTextButtonStyle does not set the size of the enclosing container
 * (normally AbstractButtonContainer). The size must be set manually.
 *
 * To get a background behind the text, use TwoWildcardTextButtonStyle together with
 * e.g. ImageButtonStyle:
 * @code
 *      TwoWildcardTextButtonStyle<ImageButtonStyle<ClickButtonTrigger> > myButton;
 * @endcode
 *
 * The position of the text can be adjusted with setTwoWildcardTextXY (default is
 * centered).
 *
 * @tparam T Generic type parameter. Typically a AbstractButtonContainer subclass.
 *
 * @see AbstractButtonContainer
 */
template <class T>
class TwoWildcardTextButtonStyle : public T
{
public:
    TwoWildcardTextButtonStyle()
        : T()
    {
        T::add(twoWildcardText);
    }

    /**
     * Sets wildcard text.
     *
     * @param  t A TypedText to process.
     */
    void setTwoWildcardText(TypedText t)
    {
        twoWildcardText.setTypedText(t);
        twoWildcardText.setWidthHeight(T::getWidth(), T::getHeight());
    }

    /**
     * Sets wildcard text x coordinate.
     *
     * @param  x The x coordinate.
     */
    void setTwoWildcardTextX(int16_t x)
    {
        twoWildcardText.setX(x);
    }

    /**
     * Sets wildcard text y coordinate.
     *
     * @param  y The y coordinate.
     */
    void setTwoWildcardTextY(int16_t y)
    {
        twoWildcardText.setY(y);
    }

    /**
     * Sets wildcard text position.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     */
    void setTwoWildcardTextXY(int16_t x, int16_t y)
    {
        setTwoWildcardTextX(x);
        setTwoWildcardTextY(y);
    }

    /**
     * Sets text position and dimensions.
     *
     * @param  x      The x coordinate.
     * @param  y      The y coordinate.
     * @param  width  The width of the text.
     * @param  height The height of the text.
     */
    void setTwoWildcardTextPosition(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        twoWildcardText.setPosition(x, y, width, height);
    }

    /**
     * Sets wildcard text rotation.
     *
     * @param  rotation The rotation.
     */
    void setTwoWildcardTextRotation(TextRotation rotation)
    {
        twoWildcardText.setRotation(rotation);
    }

    /**
     * Sets the first wildcard in the text. Must be a null-terminated UnicodeChar array.
     *
     * @param  value A pointer to the UnicodeChar to set the wildcard to.
     */
    void setWildcardTextBuffer1(const Unicode::UnicodeChar* value)
    {
        twoWildcardText.setWildcard1(value);
    }

    /**
     * Sets the second wildcard in the text. Must be a null-terminated UnicodeChar array.
     *
     * @param  value A pointer to the UnicodeChar to set the wildcard to.
     */
    void setWildcardTextBuffer2(const Unicode::UnicodeChar* value)
    {
        twoWildcardText.setWildcard2(value);
    }

    /**
     * Sets wild card text colors.
     *
     * @param  newColorReleased The new color released.
     * @param  newColorPressed  The new color pressed.
     */
    void setTwoWildcardTextColors(colortype newColorReleased, colortype newColorPressed)
    {
        colorReleased = newColorReleased;
        colorPressed = newColorPressed;

        handlePressedUpdated();
    }

protected:
    TextAreaWithTwoWildcards twoWildcardText; ///< The wildcard text
    colortype colorReleased;                  ///< The color released
    colortype colorPressed;                   ///< The color pressed

    /** @copydoc AbstractButtonContainer::handlePressedUpdated() */
    virtual void handlePressedUpdated()
    {
        twoWildcardText.setColor(T::getPressed() ? colorPressed : colorReleased);
        T::handlePressedUpdated();
    }

    /** @copydoc AbstractButtonContainer::handleAlphaUpdated() */
    virtual void handleAlphaUpdated()
    {
        twoWildcardText.setAlpha(T::getAlpha());
        T::handleAlphaUpdated();
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_TWOWILDCARDTEXTBUTTONSTYLE_HPP
