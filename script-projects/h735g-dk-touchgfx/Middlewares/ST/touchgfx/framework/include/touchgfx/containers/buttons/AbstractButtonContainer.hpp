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
 * @file touchgfx/containers/buttons/AbstractButtonContainer.hpp
 *
 * Declares the touchgfx::AbstractButtonContainer class.
 */
#ifndef TOUCHGFX_ABSTRACTBUTTONCONTAINER_HPP
#define TOUCHGFX_ABSTRACTBUTTONCONTAINER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/containers/Container.hpp>

namespace touchgfx
{
/**
 * An abstract button container. The AbstractButtonContainer defines pressed/not pressed state,
 * the alpha value, and the action Callback of a button. AbstractButtonContainer is used
 * as superclass for classes defining a specific button behavior.
 *
 * @see ClickButtonTrigger, RepeatButtonTrigger, ToggleButtonTrigger, TouchButtonTrigger
 */
class AbstractButtonContainer : public Container
{
public:
    AbstractButtonContainer()
        : pressed(false), alpha(255), action(0)
    {
        setTouchable(true);
    }

    /**
     * Sets the pressed state to the given state. A subclass of AbstractButtonContainer
     * should implement handlePressedUpdate() to handle the new pressed state.
     *
     * @param  isPressed True if is pressed, false if not.
     *
     * @see getPressed, handlePressedUpdated
     */
    void setPressed(bool isPressed)
    {
        pressed = isPressed;
        handlePressedUpdated();
    }

    /**
     * Gets the pressed state.
     *
     * @return True if it succeeds, false if it fails.
     *
     * @see setPressed
     */
    bool getPressed()
    {
        return pressed;
    }

    /** @copydoc Image::setAlpha() */
    void setAlpha(uint8_t newAlpha)
    {
        alpha = newAlpha;
        handleAlphaUpdated();
    }

    /** @copydoc Image::getAlpha() */
    uint8_t getAlpha() const
    {
        return alpha;
    }

    /**
     * Sets an action callback to be executed by the subclass of AbstractContainerButton.
     *
     * @param  callback The callback.
     *
     * @see executeAction
     */
    void setAction(GenericCallback<const AbstractButtonContainer&>& callback)
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

protected:
    bool pressed;  ///< True if pressed
    uint8_t alpha; ///< The current alpha value. 255 denotes solid, 0 denotes completely invisible.

    GenericCallback<const AbstractButtonContainer&>* action; ///< The action to be executed

    /** Handles what should happen when the pressed state is updated. */
    virtual void handlePressedUpdated()
    {
    }

    /** Handles what should happen when the alpha is updated. */
    virtual void handleAlphaUpdated()
    {
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTBUTTONCONTAINER_HPP
