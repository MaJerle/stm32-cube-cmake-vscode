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
 * @file touchgfx/transitions/BlockTransition.hpp
 *
 * Declares the touchgfx::BlockTransition class.
 */
#ifndef TOUCHGFX_BLOCKTRANSITION_HPP
#define TOUCHGFX_BLOCKTRANSITION_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/transitions/Transition.hpp>

namespace touchgfx
{
/**
 * A Transition that draws two small blocks in every frame. It is
 * therefore very useful on MCUs with limited performance.
 */
class BlockTransition : public Transition
{
public:
    /**
     * Initializes a new instance of the BlockTransition class.
     *
     * @param  transitionSteps (Optional) Number of steps in the transition animation.
     */
    BlockTransition()
        : Transition(),
          animationCounter(0)
    {
        // 8x6 blocks, with 8 blocks on the longest edge
        if (HAL::DISPLAY_WIDTH > HAL::DISPLAY_HEIGHT)
        {
            blockWidth = (HAL::DISPLAY_WIDTH + 7) / 8;
            blockHeight = (HAL::DISPLAY_HEIGHT + 5) / 6;
            blocksHorizontal = 8;
        }
        else
        {
            blockWidth = (HAL::DISPLAY_WIDTH + 5) / 6;
            blockHeight = (HAL::DISPLAY_HEIGHT + 7) / 8;
            blocksHorizontal = 6;
        }
    }

    /**
     * Handles the tick event when transitioning. It uncovers and
     * invalidates two blocks in every frame, for a total of 24
     * frames.
     */
    virtual void handleTickEvent()
    {
        const int blocks = 48;
        // "Random" sequence of blocks to invalidate
        const int indeces[blocks] = { 20, 11, 47, 14, 10, 0, 18, 28, 13, 6, 2, 41,
                                      44, 5, 3, 17, 36, 46, 26, 15, 29, 39, 25, 12,
                                      19, 24, 7, 38, 37, 30, 9, 43, 4, 31, 22, 23,
                                      35, 16, 32, 42, 8, 1, 40, 33, 21, 27, 34, 45 };

        Transition::handleTickEvent();

        if (animationCounter == 0 && HAL::USE_DOUBLE_BUFFERING)
        {
            Application::getInstance()->copyInvalidatedAreasFromTFTToClientBuffer();
        }

        if (animationCounter < blocks)
        {
            int blocks_per_tick = 2;
            while (blocks_per_tick-- > 0 && animationCounter < blocks)
            {
                // Invalidate next block in sequence
                const int index = indeces[animationCounter];

                const int16_t x = (index % blocksHorizontal) * blockWidth;
                const int16_t y = (index / blocksHorizontal) * blockHeight;

                Rect invRect(x, y, blockWidth, blockHeight);
                screenContainer->invalidateRect(invRect);
                animationCounter++;
            }
        }
        else
        {
            done = true;
        }
    }

    virtual void tearDown()
    {
    }

    virtual void init()
    {
        Transition::init();
    }

    /**
     * Block transition does not require an invalidation. Invalidation
     * is handled by the class. Do no invalidation initially.
     */
    virtual void invalidate()
    {
    }

private:
    uint16_t blockWidth;
    uint16_t blockHeight;
    uint16_t blocksHorizontal;
    uint8_t animationCounter; ///< Current step in the transition animation.
};

} // namespace touchgfx

#endif // TOUCHGFX_BLOCKTRANSITION_HPP
