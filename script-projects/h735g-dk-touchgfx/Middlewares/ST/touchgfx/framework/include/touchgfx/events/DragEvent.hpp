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
 * @file touchgfx/events/DragEvent.hpp
 *
 * Declares the touchgfx::DragEvent class.
 */
#ifndef TOUCHGFX_DRAGEVENT_HPP
#define TOUCHGFX_DRAGEVENT_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Event.hpp>

namespace touchgfx
{
/**
 * A drag event. The only drag event currently supported is DRAGGED, which will be issued every
 * time the input system detects a drag.
 *
 * @see Event
 */
class DragEvent : public Event
{
public:
    /** Values that represent drag event types. */
    enum DragEventType
    {
        DRAGGED ///< An enum constant representing the dragged option
    };

    /**
     * Initializes a new instance of the DragEvent class.
     *
     * @param  type  The type of the drag event.
     * @param  fromX The x coordinate of the drag start position (dragged from)
     * @param  fromY The y coordinate of the drag start position (dragged from)
     * @param  toX   The x coordinate of the new position (dragged to)
     * @param  toY   The y coordinate of the new position (dragged to)
     */
    DragEvent(DragEventType type, int16_t fromX, int16_t fromY, int16_t toX, int16_t toY)
        : dragEventType(type), dragFromX(fromX), dragFromY(fromY), dragToX(toX), dragToY(toY)
    {
    }

    /**
     * Gets the x coordinate where the drag operation was started (dragged from).
     *
     * @return The x coordinate where the drag operation was started (dragged from).
     */
    int16_t getOldX() const
    {
        return dragFromX;
    }

    /**
     * Gets the y coordinate where the drag operation was started (dragged from).
     *
     * @return The y coordinate where the drag operation was started (dragged from).
     */
    int16_t getOldY() const
    {
        return dragFromY;
    }

    /**
     * Gets the new x coordinate (dragged to).
     *
     * @return The new x coordinate (dragged to).
     */
    int16_t getNewX() const
    {
        return dragToX;
    }

    /**
     * Gets the new x coordinate (dragged to).
     *
     * @return The new y coordinate (dragged to).
     */
    int16_t getNewY() const
    {
        return dragToY;
    }

    /**
     * Gets the type of this drag event.
     *
     * @return The type of this drag event.
     */
    DragEventType getType() const
    {
        return dragEventType;
    }

    /**
     * Gets the distance in x coordinates (how long was the drag).
     *
     * @return The distance of this drag event.
     */
    int16_t getDeltaX() const
    {
        return dragToX - dragFromX;
    }

    /**
     * Gets the distance in y coordinates (how long was the drag).
     *
     * @return The distance of this drag event.
     */
    int16_t getDeltaY() const
    {
        return dragToY - dragFromY;
    }

    /**
     * Gets event type.
     *
     * @return The type of this event.
     */
    virtual Event::EventType getEventType()
    {
        return Event::EVENT_DRAG;
    }

private:
    DragEventType dragEventType;
    int16_t dragFromX;
    int16_t dragFromY;
    int16_t dragToX;
    int16_t dragToY;
};

} // namespace touchgfx

#endif // TOUCHGFX_DRAGEVENT_HPP
