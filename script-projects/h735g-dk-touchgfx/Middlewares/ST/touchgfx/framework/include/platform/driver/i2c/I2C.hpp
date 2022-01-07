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
 * @file platform/driver/i2c/I2C.hpp
 *
 * Declares the touchfgx::I2C interface class.
 */
#ifndef TOUCHGFX_I2C_HPP
#define TOUCHGFX_I2C_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/** Platform independent interface for I2C drivers. */
class I2C
{
public:
    /**
     * Initializes a new instance of the I2C class. Stores the channel of the I2C bus to be
     * configured.
     *
     * @param  ch I2C channel.
     */
    I2C(uint8_t ch)
        : channel(ch)
    {
    }

    /** Finalizes an instance of the I2C class. */
    virtual ~I2C()
    {
    }

    /** Initializes the I2C driver. */
    virtual void init() = 0;

    /**
     * Reads the specified register on the device with the specified address.
     *
     * @param       addr The I2C device address.
     * @param       reg  The register.
     * @param [out] data Pointer to buffer in which to place the result.
     * @param       cnt  Size of buffer in bytes.
     *
     * @return true on success, false otherwise.
     */
    virtual bool readRegister(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t cnt) = 0;

    /**
     * Writes the specified value in a register.
     *
     * @param  addr The I2C device address.
     * @param  reg  The register.
     * @param  val  The new value.
     *
     * @return true on success, false otherwise.
     */
    virtual bool writeRegister(uint8_t addr, uint8_t reg, uint8_t val) = 0;

protected:
    uint8_t channel; ///< I2c channel is stored in order to initialize and recover a specific I2C channel
};

} // namespace touchgfx

#endif // TOUCHGFX_I2C_HPP
