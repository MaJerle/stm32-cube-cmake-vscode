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
 * @file touchgfx/mixins/Draggable.hpp
 *
 * Declares the touchgfx::Draggable class.
 */
#ifndef TOUCHGFX_DRAGGABLE_HPP
#define TOUCHGFX_DRAGGABLE_HPP

#include <touchgfx/events/DragEvent.hpp>

namespace touchgfx
{
/**
 * Mix-in class that extends a class to become Draggable, which means that the object on screen
 * can be freely moved around using the touch screen.
 *
 * @tparam T specifies the type to extend with the Draggable behavior.
 */
template <class T>
class Draggable : public T
{
public:
    /** Initializes a new instance of the Draggable class. Make the object touchable. */
    Draggable()
        : T()
    {
        T::setTouchable(true);
    }

    /**
     * Called when dragging the Draggable object. The object is moved according to the drag
     * event.
     *
     * @param  event The drag event.
     */
    virtual void handleDragEvent(const DragEvent& event)
    {
        T::handleDragEvent(event);
        T::moveRelative(event.getDeltaX(), event.getDeltaY());
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_DRAGGABLE_HPP
