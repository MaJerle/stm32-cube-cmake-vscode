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
 * @file touchgfx/hal/GPIO.hpp
 *
 * Declares the touchgfx::GPIO class.
 */
#ifndef TOUCHGFX_GPIO_HPP
#define TOUCHGFX_GPIO_HPP

namespace touchgfx
{
/**
 * Interface class for manipulating GPIOs in order to do performance measurements on target. Not
 * used on the PC simulator.
 */
class GPIO
{
public:
    /** Enum for the GPIOs used. */
    enum GPIO_ID
    {
        VSYNC_FREQ,  /// Pin is toggled at each VSYNC
        RENDER_TIME, /// Pin is high when frame rendering begins, low when finished
        FRAME_RATE,  /// Pin is toggled when the framebuffers are swapped.
        MCU_ACTIVE   /// Pin is high when the MCU is doing work (i.e. not in idle task).
    };

    /** Perform configuration of IO pins. */
    static void init();

    /**
     * Sets a pin high.
     *
     * @param  id the pin to set.
     */
    static void set(GPIO_ID id);

    /**
     * Sets a pin low.
     *
     * @param  id the pin to set.
     */
    static void clear(GPIO_ID id);

    /**
     * Toggles a pin.
     *
     * @param  id the pin to toggle.
     */
    static void toggle(GPIO_ID id);

    /**
     * Gets the state of a pin.
     *
     * @param  id the pin to get.
     *
     * @return true if the pin is high, false otherwise.
     */
    static bool get(GPIO_ID id);
};

} // namespace touchgfx

#endif // TOUCHGFX_GPIO_HPP
