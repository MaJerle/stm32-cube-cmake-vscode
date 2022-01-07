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
 * @file touchgfx/hal/Gestures.hpp
 *
 * Declares the touchgfx::Gestures class.
 */
#ifndef TOUCHGFX_GESTURES_HPP
#define TOUCHGFX_GESTURES_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/UIEventListener.hpp>
#include <touchgfx/events/ClickEvent.hpp>

namespace touchgfx
{
/** This class implements the detection of gestures. */
class Gestures
{
    static const uint8_t MAX_TICKS_BETWEEN_MOVES_FOR_SWIPE = 7;
    static const uint8_t MIN_VELOCITY_AT_RELEASE_BEFORE_SWIPE = 3;

    /** Defines the state of a drag. */
    struct DragState
    {
        DragState()
            : startX(0),
              startY(0),
              downX(0),
              downY(0),
              tickCount(0),
              velocityX(0),
              velocityY(0),
              inProgress(false)
        {
        }

        uint16_t startX;    ///< Starting x coordinate
        uint16_t startY;    ///< Starting y coordinate
        uint16_t downX;     ///< Starting x coordinate of the drag
        uint16_t downY;     ///< Starting x coordinate of the drag
        uint16_t tickCount; ///< Measures the timing of the drag
        int16_t velocityX;  ///< The velocity (X orientation) of the drag
        int16_t velocityY;  ///< The velocity (Y orientation) of the drag
        bool inProgress;    ///< Whether a drag is in progress or not
    };

public:
    /** Default constructor. Does nothing. */
    Gestures()
        : drag(), listener(0), dragThresholdValue(0)
    {
    }

    /**
     * Register the event listener.
     *
     * @param [in] l The EventListener to register.
     */
    void registerEventListener(UIEventListener& l);

    /** Has to be called during the timer tick. */
    void tick();

    /**
     * Register a drag event.
     *
     * @param  oldX The x coordinate of the drag start position (dragged from)
     * @param  oldY The y coordinate of the drag start position (dragged from)
     * @param  newX The x coordinate of the new position (dragged to)
     * @param  newY The y coordinate of the new position (dragged to)
     *
     * @return True if the drag exceeds threshold value (and therefore was reported as a
     *         drag), or false if the drag did not exceed threshold (and therefore was
     *         discarded).
     */
    bool registerDragEvent(uint16_t oldX, uint16_t oldY, uint16_t newX, uint16_t newY);

    /**
     * Register a click event and figure out if this is a drag event, too.
     *
     * @param  event The type of the click event.
     * @param  x     The x coordinate of the click event.
     * @param  y     The y coordinate of the click event.
     */
    void registerClickEvent(ClickEvent::ClickEventType event, uint16_t x, uint16_t y);

    /**
     * Configure the threshold for reporting drag events. A touch input movement must exceed
     * this value in either axis in order to report a drag. Default value is 0.
     *
     * @param  val New threshold value.
     */
    void setDragThreshold(uint16_t val)
    {
        dragThresholdValue = val;
    }

private:
    DragState drag;
    UIEventListener* listener;
    uint16_t dragThresholdValue;
};

} // namespace touchgfx

#endif // TOUCHGFX_GESTURES_HPP
