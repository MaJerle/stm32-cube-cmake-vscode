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
 * @file platform/driver/touch/NoTouchController.hpp
 *
 * Declares the touchgfx::NoTouchController class.
 */
#ifndef TOUCHGFX_NOTOUCHCONTROLLER_HPP
#define TOUCHGFX_NOTOUCHCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>
#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
/**
 * Empty TouchController implementation which does nothing. Use this if your display does not
 * have touch input capabilities.
 */
class NoTouchController : public TouchController
{
public:
    virtual void init()
    {
    }

    virtual bool sampleTouch(int32_t& x, int32_t& y)
    {
        return false;
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_NOTOUCHCONTROLLER_HPP
