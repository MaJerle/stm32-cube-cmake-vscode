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
 * @file touchgfx/hal/OSWrappers.hpp
 *
 * Declares the touchgfx::OSWrappers class.
 */
#ifndef TOUCHGFX_OSWRAPPERS_HPP
#define TOUCHGFX_OSWRAPPERS_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * This class specifies OS wrappers for dealing with the framebuffer semaphore and the VSYNC
 * signal.
 */
class OSWrappers
{
public:
    /** Initialize framebuffer semaphore and queue/mutex for VSYNC signal. */
    static void initialize();

    /**
     * Signal that a VSYNC has occurred. Should make the vsync queue/mutex available.
     *
     * @note This function is called from an ISR, and should (depending on OS) trigger a
     *       scheduling.
     */
    static void signalVSync();

    /**
     * Signal that the rendering of the frame has completed. Used by
     * some systems to avoid using any previous vsync.
     */
    static void signalRenderingDone();

    /**
     * This function blocks until a VSYNC occurs.
     *
     * @note This function must first clear the mutex/queue and then wait for the next one to
     *       occur.
     */
    static void waitForVSync();

    /**
     * This function checks if a VSync occurred after last
     * rendering. The function is used in systems that cannot wait in
     * waitForVSync (because they are also checking other event
     * sources.
     *
     * @note signalRenderingDone is typically used together with this function.
     *
     * @return True if VSync occurred.
     */
    static bool isVSyncAvailable();

    /** Take the framebuffer semaphore. Blocks until semaphore is available. */
    static void takeFrameBufferSemaphore();

    /**
     * Attempt to obtain the framebuffer semaphore. If semaphore is not available, do
     * nothing.
     *
     * @note must return immediately! This function does not care who has the taken the semaphore,
     *       it only serves to make sure that the semaphore is taken by someone.
     */
    static void tryTakeFrameBufferSemaphore();

    /** Release the framebuffer semaphore. */
    static void giveFrameBufferSemaphore();

    /**
     * Release the framebuffer semaphore in a way that is safe in interrupt context. Called
     * from ISR.
     */
    static void giveFrameBufferSemaphoreFromISR();

    /**
     * A function that causes executing task to sleep for a number of milliseconds. This
     * function is OPTIONAL. It is only used by the TouchGFX in the case of a specific frame
     * refresh strategy (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL). Due to backwards
     * compatibility, in order for this function to be usable by the HAL the function must
     * be explicitly registered:
     *   hal.registerTaskDelayFunction(&OSWrappers::taskDelay)
     *
     * @param  ms The number of milliseconds to sleep.
     *
     * @see HAL::setFrameRefreshStrategy, HAL::registerTaskDelayFunction
     */
    static void taskDelay(uint16_t ms);

    /**
     * A function that causes the executing task to yield control to
     * another thread. This function is used by the framework when it
     * is necessary to wait a little before continuing (e.g. drawing).
     *
     * The implementation should typically request the operating
     * system to change to another task of similar priority. When
     * running without an operating system, the implementation can run
     * a very short task and return.
     */
    static void taskYield();
};

} // namespace touchgfx

#endif // TOUCHGFX_OSWRAPPERS_HPP
