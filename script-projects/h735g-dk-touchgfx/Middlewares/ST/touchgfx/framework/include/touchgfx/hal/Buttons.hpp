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
 * @file touchgfx/hal/Buttons.hpp
 *
 * Declares the touchgfx::Buttons class.
 */
#ifndef TOUCHGFX_BUTTONS_HPP
#define TOUCHGFX_BUTTONS_HPP

namespace touchgfx
{
/** A class for accessing a physical button. */
class Buttons
{
public:
    /** Perform configuration of IO pins. */
    static void init();

    /**
     * Sample button states.
     *
     * @return the sampled state of the buttons.
     */
    static unsigned int sample();
};

} // namespace touchgfx

#endif // TOUCHGFX_BUTTONS_HPP
