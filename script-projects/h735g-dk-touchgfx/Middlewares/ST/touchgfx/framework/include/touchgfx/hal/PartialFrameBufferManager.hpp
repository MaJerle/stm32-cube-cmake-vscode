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
 * @file touchgfx/hal/PartialFrameBufferManager.hpp
 *
 * Declares the touchgfx::PartialFrameBufferManager class.
 */
#ifndef TOUCHGFX_PARTIALFRAMEBUFFERMANAGER_HPP
#define TOUCHGFX_PARTIALFRAMEBUFFERMANAGER_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * Check if a Frame Buffer Block is being transmitted.
 *
 * @return Non zero if possible.
 */
int transmitActive();

/**
 * Check if a Frame Buffer Block ending at bottom may be sent.
 *
 * @param bottom The bottom coordinate of the block to transfer.
 *
 * @return Non zero if possible.
 */
int shouldTransferBlock(uint16_t bottom);

/**
 * Transmit a Frame Buffer Block.
 *
 * @param pixels Pointer to the pixel data.
 * @param x      X coordinate of the block.
 * @param y      Y coordinate of the block.
 * @param w      Width of the block.
 * @param h      Height of the block.
 */
void transmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/**
 * This class specifies strategies for transmitting block to the display using Partial Frame Buffer.
 */
class PartialFrameBufferManager
{
public:
    /**
     * Transmit all remaining drawn Framebuffer blocks.
     *
     * @note This function does not return before all blocks have been transmitted.
     */
    static void transmitRemainingBlocks();

    /**
     * Tries to transmit a drawn block.
     *
     * @note Will return immediately if already transmitting.
     */
    static void tryTransmitBlock();

    /**
     * Tries to transmit a drawn block in interrupt context.
     *
     *  @note Will transmit next block immediately if drawn.
     */
    static void tryTransmitBlockFromIRQ();
};

} // namespace touchgfx

#endif // TOUCHGFX_PARTIALFRAMEBUFFERMANAGER_HPP
