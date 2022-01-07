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
 * @file touchgfx/mixins/ClickListener.hpp
 *
 * Declares the touchgfx::ClickListener class.
 */
#ifndef TOUCHGFX_CLICKLISTENER_HPP
#define TOUCHGFX_CLICKLISTENER_HPP

#include <touchgfx/Callback.hpp>
#include <touchgfx/events/ClickEvent.hpp>

namespace touchgfx
{
/**
 * Mix-in class that extends a class with a click action event that is called when the class
 * receives a click event.
 *
 * @tparam T specifies the type to extend with the ClickListener behavior.
 */
template <class T>
class ClickListener : public T
{
public:
    /** Initializes a new instance of the ClickListener class. Make the object touchable. */
    ClickListener()
        : T(), clickAction(0)
    {
        T::setTouchable(true);
    }

    /**
     * Ensures that the clickEvent is propagated to the super class T and to the clickAction
     * listener.
     *
     * @param  event Information about the click.
     */
    virtual void handleClickEvent(const ClickEvent& event)
    {
        T::handleClickEvent(event);
        if (clickAction && clickAction->isValid())
        {
            clickAction->execute(*this, event);
        }
    }

    /**
     * Associates an action to be performed when the class T is clicked.
     *
     * @param  callback The callback to be executed. The callback will be given a reference
     *                  to T.
     */
    void setClickAction(GenericCallback<const T&, const ClickEvent&>& callback)
    {
        clickAction = &callback;
    }

protected:
    GenericCallback<const T&, const ClickEvent&>* clickAction; ///< The callback to be executed when T is clicked
};

} // namespace touchgfx

#endif // TOUCHGFX_CLICKLISTENER_HPP
