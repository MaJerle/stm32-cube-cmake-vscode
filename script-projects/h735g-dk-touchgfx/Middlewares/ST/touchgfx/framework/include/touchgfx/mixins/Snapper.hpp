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
 * @file touchgfx/mixins/Snapper.hpp
 *
 * Declares the touchgfx::Snapper class.
 */
#ifndef TOUCHGFX_SNAPPER_HPP
#define TOUCHGFX_SNAPPER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/mixins/Draggable.hpp>

namespace touchgfx
{
/**
 * A mix-in that will make class T draggable and able to snap to a position when a drag
 * operation has ended. The mix-in is able to perform callbacks when the snapper gets
 * dragged and when the Snapper snaps to its snap position.
 *
 * @see Draggable<T>
 *
 * @tparam T specifies the type to enable the Snap behavior to.
 */
template <class T>
class Snapper : public Draggable<T>
{
public:
    Snapper()
        : Draggable<T>(), snapPositionX(0), snapPositionY(0), dragAction(0), snappedAction(0)
    {
    }

    virtual void handleDragEvent(const DragEvent& event)
    {
        Draggable<T>::handleDragEvent(event);

        if (dragAction && dragAction->isValid())
        {
            dragAction->execute(event);
        }
    }

    /**
     * Handles the click events when the Snapper is clicked. It saves its current position
     * as the snap position if the Snapper is pressed. This happens when the drag operation
     * starts.
     *
     * The snapper will then move to the snap position when the click is released. This
     * happens when the drag operation ends.
     *
     * @param  event The click event.
     */
    virtual void handleClickEvent(const ClickEvent& event)
    {
        T::handleClickEvent(event);

        if (event.getType() == ClickEvent::RELEASED)
        {
            if (snappedAction && snappedAction->isValid())
            {
                snappedAction->execute();
            }

            T::moveTo(snapPositionX, snapPositionY);
        }
        else if (event.getType() == ClickEvent::PRESSED)
        {
            snapPositionX = T::getX();
            snapPositionY = T::getY();
        }
    }

    /**
     * Sets the position the Snapper should snap to. This position will be overridden with
     * the Snappers current position when the Snapper is pressed.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     */
    void setSnapPosition(int16_t x, int16_t y)
    {
        snapPositionX = x;
        snapPositionY = y;
    }

    /**
     * Associates an action to be performed when the Snapper is dragged.
     *
     * @param  callback The callback will be executed with the DragEvent.
     *
     * @see GenericCallback
     */
    void setDragAction(GenericCallback<const DragEvent&>& callback)
    {
        dragAction = &callback;
    }

    /**
     * Associates an action to be performed when the Snapper is snapped.
     *
     * @param [in] callback The callback to be executed on snap.
     *
     * @see GenericCallback
     */
    void setSnappedAction(GenericCallback<>& callback)
    {
        snappedAction = &callback;
    }

private:
    int16_t snapPositionX;
    int16_t snapPositionY;
    GenericCallback<const DragEvent&>* dragAction;
    GenericCallback<>* snappedAction;
};

} // namespace touchgfx

#endif // TOUCHGFX_SNAPPER_HPP
