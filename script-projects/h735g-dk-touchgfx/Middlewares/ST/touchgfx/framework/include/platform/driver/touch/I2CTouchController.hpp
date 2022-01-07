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
 * @file platform/driver/touch/I2CTouchController.hpp
 *
 * Declares the touchgfx::I2CTouchController interface class.
 */
#ifndef TOUCHGFX_I2CTOUCHCONTROLLER_HPP
#define TOUCHGFX_I2CTOUCHCONTROLLER_HPP

#include <touchgfx/hal/Types.hpp>
#include <platform/driver/i2c/I2C.hpp>
#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
/**
 * Specific I2C-enabled type of Touch Controller.
 *
 * @see TouchController
 */
class I2CTouchController : public TouchController
{
public:
    /**
     * Constructor. Initializes I2C driver.
     *
     * @param [in] i2c I2C driver.
     */
    I2CTouchController(I2C& i2c)
        : i2c(i2c)
    {
        i2c.init();
    }

    virtual ~I2CTouchController()
    {
    }

    virtual void init() = 0;

    virtual bool sampleTouch(int32_t& x, int32_t& y) = 0;

protected:
    I2C& i2c; ///< I2C driver
};

} // namespace touchgfx

#endif // TOUCHGFX_I2CTOUCHCONTROLLER_HPP
