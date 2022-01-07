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
 * @file touchgfx/events/GestureEvent.hpp
 *
 * Declares the touchgfx::GestureEvent class.
 */
#ifndef TOUCHGFX_GESTUREEVENT_HPP
#define TOUCHGFX_GESTUREEVENT_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Event.hpp>

namespace touchgfx
{
/**
 * A gesture event. The only gesture events currently supported is #SWIPE_HORIZONTAL and
 * #SWIPE_VERTICAL, which will be issued every time the input system detects a swipe.
 *
 * @see Event
 */
class GestureEvent : public Event
{
public:
    /** Values that represent gesture types. */
    enum GestureEventType
    {
        SWIPE_HORIZONTAL, ///< An enum constant representing a horizontal swipe
        SWIPE_VERTICAL    ///< An enum constant representing a vertical swipe
    };

    /**
     * Constructor. Create a gesture event of the specified type with the specified
     * coordinates.
     *
     * @param  type     The type of the gesture event.
     * @param  velocity The velocity of this gesture (swipe)
     * @param  x        The x coordinate of the gesture.
     * @param  y        The y coordinate of the gesture.
     */
    GestureEvent(GestureEventType type, int16_t velocity, int16_t x, int16_t y)
        : gestureEventType(type),
          gestureVelocity(velocity),
          gestureX(x),
          gestureY(y)
    {
    }

    /**
     * Gets the velocity of this gesture event.
     *
     * @return The velocity of this gesture event.
     */
    int16_t getVelocity() const
    {
        return gestureVelocity;
    }

    /**
     * Gets the type of this gesture event.
     *
     * @return The type of this gesture event.
     */
    GestureEventType getType() const
    {
        return gestureEventType;
    }

    /**
     * Gets the x coordinate of this gesture event.
     *
     * @return The x coordinate of this gesture event.
     */
    int16_t getX() const
    {
        return gestureX;
    }

    /**
     * Gets the y coordinate of this gesture event.
     *
     * @return The y coordinate of this gesture event.
     */
    int16_t getY() const
    {
        return gestureY;
    }

    /**
     * Gets event type.
     *
     * @return The type of this event.
     */
    virtual Event::EventType getEventType()
    {
        return Event::EVENT_GESTURE;
    }

private:
    /** Initializes a new instance of the GestureEvent class. */
    GestureEvent()
        : gestureEventType(SWIPE_HORIZONTAL), gestureVelocity(0), gestureX(0), gestureY(0)
    {
    }

    GestureEventType gestureEventType;
    int16_t gestureVelocity;
    int16_t gestureX;
    int16_t gestureY;
};

} // namespace touchgfx

#endif // TOUCHGFX_GESTUREEVENT_HPP
