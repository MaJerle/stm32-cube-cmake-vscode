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

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <platform/driver/touch/SDL2TouchController.hpp>
#include <platform/hal/simulator/sdl2/HALSDL2.hpp>

namespace touchgfx
{
void SDL2TouchController::init()
{
}

bool SDL2TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    return static_cast<HALSDL2*>(HAL::getInstance())->doSampleTouch(x, y);
}
} // namespace touchgfx
