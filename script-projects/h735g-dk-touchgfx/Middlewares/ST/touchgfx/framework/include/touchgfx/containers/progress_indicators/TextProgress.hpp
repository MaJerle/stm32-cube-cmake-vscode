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
 * @file touchgfx/containers/progress_indicators/TextProgress.hpp
 *
 * Declares the touchgfx::TextProgress class.
 */
#ifndef TOUCHGFX_TEXTPROGRESS_HPP
#define TOUCHGFX_TEXTPROGRESS_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/containers/progress_indicators/AbstractProgressIndicator.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

namespace touchgfx
{
/**
 * A text progress will display progress as a number with a given number of decimals.
 *
 * @note The implementation does not use floating point variables to calculate the progress.
 */
class TextProgress : public AbstractProgressIndicator
{
public:
    TextProgress();

    /**
     * Sets the position and dimensions of the text progress indicator.
     *
     * Sets the position and dimensions of the text progress indicator relative to the
     * background image.
     *
     * @param  x      The x coordinate.
     * @param  y      The y coordinate.
     * @param  width  The width of the text progress indicator.
     * @param  height The height of the text progress indicator.
     */
    virtual void setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    /**
     * Sets the typed text. The text should have exactly one wildcard and could for example
     * look like this: "&lt;progress&gt;\%".
     *
     * @param  t The TypedText to process.
     *
     * @see getTypedText
     */
    virtual void setTypedText(const TypedText& t);

    /**
     * Gets the typed text.
     *
     * @return The typed text.
     *
     * @see setTypedText
     */
    virtual TypedText getTypedText() const;

    /**
     * Sets the color of the text in the used text area.
     *
     * @param  color The color.
     *
     * @see getColor, TextArea::setColor
     */
    virtual void setColor(colortype color);

    /**
     * Gets the color of the text in the used text area.
     *
     * @return The color.
     */
    virtual colortype getColor() const;

    /** @copydoc Image::setAlpha */
    virtual void setAlpha(uint8_t newAlpha);

    /**
     * Sets the new value for the progress indicator.
     *
     * @param  value The value.
     */
    virtual void setValue(int value);

    /**
     * Sets number of decimals when displaying progress.
     *
     * @param  numberOfDecimals Number of decimals. Only up to two decimals is supported.
     *
     * @see getNumberOfDecimals
     */
    virtual void setNumberOfDecimals(uint16_t numberOfDecimals);

    /**
     * Gets number of decimals.
     *
     * @return The number of decimals.
     *
     * @see setNumberOfDecimals
     */
    virtual uint16_t getNumberOfDecimals() const;

protected:
    TextAreaWithOneWildcard textArea;   ///< The text area
    Unicode::UnicodeChar textBuffer[9]; ///< Room for 100.0000
    uint16_t decimals;                  ///< The number of decimals
};

} // namespace touchgfx

#endif // TOUCHGFX_TEXTPROGRESS_HPP
