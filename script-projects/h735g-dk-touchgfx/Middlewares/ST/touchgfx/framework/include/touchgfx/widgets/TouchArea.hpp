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
 * @file touchgfx/widgets/TouchArea.hpp
 *
 * Declares the touchgfx::TouchArea class.
 */
#ifndef TOUCHGFX_TOUCHAREA_HPP
#define TOUCHGFX_TOUCHAREA_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/widgets/AbstractButton.hpp>

namespace touchgfx
{
/**
 * Invisible widget used to capture touch events. The TouchArea consumes drag events without the
 * widget it self moving.
 */
class TouchArea : public AbstractButton
{
public:
    TouchArea()
        : AbstractButton(), pressedAction(0)
    {
    }

    virtual void draw(const Rect& invalidatedArea) const
    {
    }

    virtual void handleDragEvent(const DragEvent& event)
    {
    }

    virtual void handleClickEvent(const ClickEvent& event);

    virtual Rect getSolidRect() const
    {
        return Rect(0, 0, 0, 0);
    }

    /**
     * Associates an action to be performed when the TouchArea is pressed.
     *
     * @param  callback The callback is given a reference to this touch area.
     */
    void setPressedAction(GenericCallback<const AbstractButton&>& callback)
    {
        pressedAction = &callback;
    }

protected:
    GenericCallback<const AbstractButton&>* pressedAction; ///< The action to perform when the TouchArea is clicked
};

} // namespace touchgfx

#endif // TOUCHGFX_TOUCHAREA_HPP
