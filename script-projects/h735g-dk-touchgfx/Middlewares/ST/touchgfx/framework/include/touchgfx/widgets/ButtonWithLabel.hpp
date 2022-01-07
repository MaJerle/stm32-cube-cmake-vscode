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
 * @file touchgfx/widgets/ButtonWithLabel.hpp
 *
 * Declares the touchgfx::ButtonWithLabel class.
 */
#ifndef TOUCHGFX_BUTTONWITHLABEL_HPP
#define TOUCHGFX_BUTTONWITHLABEL_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/widgets/Button.hpp>

namespace touchgfx
{
/**
 * A Button that has a bitmap with a text on top of it. It is possible to have two different
 * colors for the text depending on the current state of the button (released or
 * pressed).
 *
 * Typical use case could be a red button with a normal and a pressed image. Those
 * images can be reused across several buttons. The ButtonWithLabel will then allow a
 * text to be superimposed on top of the red button.
 *
 * @see Button
 */
class ButtonWithLabel : public Button
{
public:
    ButtonWithLabel();

    /**
     * Sets the text to display on the button. Texts with wildcards are not supported.
     *
     * @param  t The text to display.
     *
     * @note The user code must call invalidate() in order to update the button on the display.
     */
    void setLabelText(TypedText t)
    {
        typedText = t;
        updateTextPosition();
    }

    /**
     * Gets the text used for the label.
     *
     * @return The text used for the label.
     */
    TypedText getLabelText() const
    {
        return typedText;
    }

    /**
     * Sets label color for the text when the button is in the normal, released state.
     *
     * @param  col               The color with which the text label should be drawn.
     *
     * @see setLabelColorPressed
     *
     * @note If the button is currently in the normal, released state, the button should be forced
     *       to redraw itself. This is done by calling invalidate() on the ButtonWithLabel.
     * @note The user code must call invalidate() in order to update the button on the display.
     */
    void setLabelColor(colortype col)
    {
        color = col;
    }

    /**
     * Sets label color for the text when the button is in the pressed state.
     *
     * @param  col               The color with which the text label should be drawn when the
     *                           button is pressed.
     *
     * @see setLabelColor
     *
     * @note If the button is currently in the pressed state, the button should be forced to
     *       redraw itself. This is done by calling invalidate() on the ButtonWithLabel.
     * @note The user code must call invalidate() in order to update the button on the display.
     */
    void setLabelColorPressed(colortype col)
    {
        colorPressed = col;
    }

    /**
     * Sets the rotation of the text on the label. The text can be rotated in steps of 90
     * degrees.
     *
     * @param  rotation The rotation of the text. Default is TEXT_ROTATE_0.
     *
     * @see TextArea::setRotation
     *
     * @note that this will not rotate the bitmap of the label, only the text.
     * @note The user code must call invalidate() in order to update the button on the display.
     */
    void setLabelRotation(TextRotation rotation)
    {
        this->rotation = rotation;
    }

    /**
     * Gets the current rotation of the text on the label.
     *
     * @return The current rotation of the text.
     */
    TextRotation getLabelRotation()
    {
        return rotation;
    }

    /**
     * Positions the label text horizontally centered. If the text changes due to a language
     * change you may need to reposition the label text by calling this function to keep the
     * text horizontally centered.
     *
     * @note The user code must call invalidate() in order to update the button on the display.
     */
    void updateTextPosition()
    {
        if (typedText.hasValidId())
        {
            const Font* f = typedText.getFont();
            const Unicode::UnicodeChar* s = typedText.getText();
            textHeightIncludingSpacing = f->getMaxTextHeight(s) * f->getNumberOfLines(s) + f->getSpacingAbove(s);
        }
        else
        {
            textHeightIncludingSpacing = 0;
        }
    }

    virtual void draw(const Rect& area) const;

protected:
    TypedText typedText;                ///< The TypedText used for the button label.
    colortype color;                    ///< The color used for the label when the button is in the released, normal state.
    colortype colorPressed;             ///< The color used for the label when the button is in the pressed state.
    TextRotation rotation;              ///< The rotation used for the label.
    uint8_t textHeightIncludingSpacing; ///< Total height of the label (text height + spacing).
};

} // namespace touchgfx

#endif // TOUCHGFX_BUTTONWITHLABEL_HPP
