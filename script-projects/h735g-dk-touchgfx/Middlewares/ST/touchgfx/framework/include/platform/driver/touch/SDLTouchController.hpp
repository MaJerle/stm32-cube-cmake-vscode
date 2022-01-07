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
 * @file platform/driver/touch/SDLTouchController.hpp
 *
 * Declares the touchgfx::SDLTouchController class.
 */
#ifndef TOUCHGFX_SDLTOUCHCONTROLLER_HPP
#define TOUCHGFX_SDLTOUCHCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>
#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
/**
 * TouchController for the simulator.
 *
 * @see TouchController
 *
 * @deprecated Use SDL2TouchController
 */
class SDLTouchController : public TouchController
{
public:
    virtual void init();

    virtual bool sampleTouch(int32_t& x, int32_t& y);
};

} // namespace touchgfx

#endif // TOUCHGFX_SDLTOUCHCONTROLLER_HPP
