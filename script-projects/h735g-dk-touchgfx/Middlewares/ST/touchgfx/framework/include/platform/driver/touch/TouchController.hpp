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
 * @file platform/driver/touch/TouchController.hpp
 *
 * Declares the touchgfx::TouchController interface class.
 */
#ifndef TOUCHGFX_TOUCHCONTROLLER_HPP
#define TOUCHGFX_TOUCHCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/** Basic Touch Controller interface. */
class TouchController
{
public:
    /** Finalizes an instance of the TouchController class. */
    virtual ~TouchController()
    {
    }

    /** Initializes touch controller. */
    virtual void init() = 0;

    /**
     * Checks whether the touch screen is being touched, and if so, what coordinates.
     *
     * @param [out] x The x position of the touch.
     * @param [out] y The y position of the touch.
     *
     * @return True if a touch has been detected, otherwise false.
     */
    virtual bool sampleTouch(int32_t& x, int32_t& y) = 0;
};

} // namespace touchgfx

#endif // TOUCHGFX_TOUCHCONTROLLER_HPP
