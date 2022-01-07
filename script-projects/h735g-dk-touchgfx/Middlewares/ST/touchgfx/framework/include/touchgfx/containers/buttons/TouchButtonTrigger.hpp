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
 * @file touchgfx/containers/buttons/TouchButtonTrigger.hpp
 *
 * Declares the touchgfx::TouchButtonTrigger class.
 */
#ifndef TOUCHGFX_TOUCHBUTTONTRIGGER_HPP
#define TOUCHGFX_TOUCHBUTTONTRIGGER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>
#include <touchgfx/events/ClickEvent.hpp>

namespace touchgfx
{
/**
 * A touch button trigger. This trigger will create a button that reacts on touches. This means
 * it will call the set action when it gets a touch pressed event. The
 * TouchButtonTrigger can be combined with one or more of the ButtonStyle classes to
 * create a fully functional button.
 *
 * @see ClickButtonTrigger
 */
class TouchButtonTrigger : public AbstractButtonContainer
{
public:
    /**
     * Handles a ClickAvent. The action callback is called when the ClickButtonTrigger
     * receives a ClickEvent::PRESSED event. Function setPressed() will be called with the
     * new button state.
     *
     * @param  event The click event.
     *
     * @see setAction, setPressed, getPressed
     */
    virtual void handleClickEvent(const ClickEvent& event)
    {
        bool wasPressed = getPressed();
        bool newPressedValue = (event.getType() == ClickEvent::PRESSED);
        if ((newPressedValue && !wasPressed) || (!newPressedValue && wasPressed))
        {
            setPressed(newPressedValue);
            invalidate();
        }
        if (newPressedValue)
        {
            executeAction();
        }
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_TOUCHBUTTONTRIGGER_HPP
