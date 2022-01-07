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
 * @file touchgfx/hal/BoardConfiguration.hpp
 *
 * Declares initialization functions for the hardware as well as for TouchGFX.
 */
#ifndef TOUCHGFX_BOARDCONFIGURATION_HPP
#define TOUCHGFX_BOARDCONFIGURATION_HPP

namespace touchgfx
{
/**
 * Function to perform generic hardware initialization of the board. This function prototype is
 * only provided as a convention.
 */
void hw_init();

/**
 * Function to perform touchgfx initialization. This function prototype is only provided as a
 * convention.
 */
void touchgfx_init();

} // namespace touchgfx

#endif // TOUCHGFX_BOARDCONFIGURATION_HPP
