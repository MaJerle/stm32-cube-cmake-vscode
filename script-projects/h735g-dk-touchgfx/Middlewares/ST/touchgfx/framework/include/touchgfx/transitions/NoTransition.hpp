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
 * @file touchgfx/transitions/NoTransition.hpp
 *
 * Declares the touchgfx::NoTransition class.
 */
#ifndef TOUCHGFX_NOTRANSITION_HPP
#define TOUCHGFX_NOTRANSITION_HPP

#include <touchgfx/transitions/Transition.hpp>

namespace touchgfx
{
/**
 * The most simple Transition without any visual effects. THe screen transition is done by
 * immediately replace the current Screen with a new Screen.
 *
 * @see Transition
 */
class NoTransition : public Transition
{
public:
    /** Indicates that the transition is done after the first tick. */
    virtual void handleTickEvent()
    {
        done = true;
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_NOTRANSITION_HPP
