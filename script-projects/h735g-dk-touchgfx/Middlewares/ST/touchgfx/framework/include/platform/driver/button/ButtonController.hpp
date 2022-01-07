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
 * @file platform/driver/button/ButtonController.hpp
 *
 * Declares the touchgfx::ButtonController interface class.
 */
#ifndef TOUCHGFX_BUTTONCONTROLLER_HPP
#define TOUCHGFX_BUTTONCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/** Interface for sampling external key events. */
class ButtonController
{
public:
    /** Finalizes an instance of the ButtonController class. */
    virtual ~ButtonController()
    {
    }

    /** Initializes button controller. */
    virtual void init() = 0;

    /**
     * Sample external key events.
     *
     * @param [out] key Output parameter that will be set to the key value if a keypress was
     *                     detected.
     *
     * @return True if a keypress was detected and the "key" parameter is set to a value.
     */
    virtual bool sample(uint8_t& key) = 0;

    /** Resets button controller. Does nothing in the default implementation. */
    virtual void reset()
    {
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_BUTTONCONTROLLER_HPP
