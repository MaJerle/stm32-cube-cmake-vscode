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
 * @file touchgfx/containers/clock/AbstractClock.hpp
 *
 * Declares the touchgfx::AbstractClock class.
 */
#ifndef TOUCHGFX_ABSTRACTCLOCK_HPP
#define TOUCHGFX_ABSTRACTCLOCK_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/containers/Container.hpp>

namespace touchgfx
{
/**
 * Superclass of clock widgets. Allows the hour, minute and second of the clock to be set and
 * read.
 *
 * @see AnalogClock, DigitalClock
 */
class AbstractClock : public Container
{
public:
    AbstractClock();

    /**
     * Sets the time with input format as 24H. Note that this does not affect any selected
     * presentation formats.
     *
     * @param  hour   The hours, value should be between 0 and 23.
     * @param  minute The minutes, value should be between 0 and 59.
     * @param  second The seconds, value should be between 0 and 59.
     *
     * @note all values passed are saved modulo the values limit. For example minutes=62 is
     *       treated as minutes=2.
     */
    virtual void setTime24Hour(uint8_t hour, uint8_t minute, uint8_t second);

    /**
     * Sets the time with input format as 12H. Note that this does not affect any selected
     * presentation formats.
     *
     * @param  hour   The hours, value should be between 1 and 12.
     * @param  minute The minutes, value should be between 0 and 59.
     * @param  second The seconds, value should be between 0 and 59.
     * @param  am     AM/PM setting. True = AM, false = PM.
     *
     * @note all values passed are saved modulo the values limit. For example minutes=62 is
     *       treated as minutes=2.
     */
    virtual void setTime12Hour(uint8_t hour, uint8_t minute, uint8_t second, bool am);

    /**
     * Gets the current hour.
     *
     * @return The current hour in range 0-23.
     *
     * @see getCurrentHour24, getCurrentHour12
     */
    uint8_t getCurrentHour() const;

    /**
     * Gets current hour 24, i.e. between 0 and 23.
     *
     * @return The current hour in range 0-23.
     */
    uint8_t getCurrentHour24() const;

    /**
     * Gets current hour 12, i.e. between 1 and 12.
     *
     * @return The current hour in range 1-12.
     *
     * @see getCurrentHour24, getCurrentAM
     */
    uint8_t getCurrentHour12() const;

    /**
     * Is the current time a.m. or p.m.? True for a.m. and false for p.m.
     *
     * @return True if a.m., false if p.m.
     */
    bool getCurrentAM() const;

    /**
     * Gets the current minute.
     *
     * @return The current minute in range 0-59.
     */
    uint8_t getCurrentMinute() const;

    /**
     * Gets the current second.
     *
     * @return The current second in range 0-59.
     */
    uint8_t getCurrentSecond() const;

protected:
    uint8_t currentHour;   ///< Local copy of the current hour
    uint8_t currentMinute; ///< Local copy of the current minute
    uint8_t currentSecond; ///< Local copy of the current second

    /** Update the visual representation of the clock on the display. */
    virtual void updateClock() = 0;
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTCLOCK_HPP
