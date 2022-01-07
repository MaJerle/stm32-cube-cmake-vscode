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
 * @file touchgfx/widgets/AbstractButton.hpp
 *
 * Declares the touchgfx::AbstractButton class
 */
#ifndef TOUCHGFX_ABSTRACTBUTTON_HPP
#define TOUCHGFX_ABSTRACTBUTTON_HPP

#include <touchgfx/Callback.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
/**
 * This class defines an abstract interface for button-like elements. A button is a clickable
 * element that has two states: pressed and released. A button also has an action that
 * is executed when the button goes from state pressed to state released.
 */
class AbstractButton : public Widget
{
public:
    /** Sets this Widget touchable so the user can interact with buttons. */
    AbstractButton()
        : Widget(), action(), pressed(false)
    {
        setTouchable(true);
    }

    /**
     * Updates the current state of the button. The state can be either pressed or released,
     * and if the new state is different from the current state, the button is also
     * invalidated to force a redraw.
     *
     * If the button state is changed from ClickEvent::PRESSED to ClickEvent::RELEASED, the
     * associated action (if any) is also executed.
     *
     * @param  event Information about the click.
     */
    virtual void handleClickEvent(const ClickEvent& event);

    /**
     * Associates an action with the button. The action is performed when the AbstractButton
     * is in the pressed state, goes to the released.
     *
     * @param  callback The callback to be executed. The callback will be executed with a
     *                  reference to the AbstractButton.
     *
     * @see GenericCallback, handleClickEvent, ClickEvent
     */
    void setAction(GenericCallback<const AbstractButton&>& callback)
    {
        action = &callback;
    }

    /**
     * Executes the previously set action.
     *
     * @see setAction
     */
    virtual void executeAction()
    {
        if (action && action->isValid())
        {
            action->execute(*this);
        }
    }

    /**
     * Function to determine if the AbstractButton is currently pressed.
     *
     * @return true if button is pressed, false otherwise.
     */
    virtual bool getPressedState() const
    {
        return pressed;
    }

protected:
    GenericCallback<const AbstractButton&>* action; ///< The callback to be executed when this AbstractButton is clicked

    bool pressed; ///< Is the button pressed or released? True if pressed.
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTBUTTON_HPP
