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
 * @file touchgfx/hal/Atomic.hpp
 *
 * Declares functions for performing atomic operations.
 */
#ifndef TOUCHGFX_ATOMIC_HPP
#define TOUCHGFX_ATOMIC_HPP

/**
 * Defines a atomic write on supported platforms
 */

#if defined(WIN32) || defined(_WIN32)

#include <windows.h>
/** Defines the atomic type. */
typedef LONG atomic_t;

/**
 * Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param       value  The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    InterlockedExchange(&target, value);
}

#elif defined(__GNUC__) && !defined(__ARMCC_VERSION)

#include <csignal>
/** Defines the atomic type. */
typedef sig_atomic_t atomic_t;

/**
 * Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param       value  The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    __sync_synchronize();
    target = value;
}

#elif defined(__IAR_SYSTEMS_ICC__)

/** Defines the atomic type. */
typedef unsigned long atomic_t;

/**
 * Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param       value  The value to write.
 *
 * @note Assume that 32 bit writes are atomic.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    target = value;
}
#elif defined(__ARMCC_VERSION)
/** Defines the atomic type. */
typedef unsigned long atomic_t;

/**
 * Makes a atomic write of value to target.
 *
 * @param [out] target The value to write to.
 * @param       value  The value to write.
 */
inline void atomic_set(atomic_t& target, atomic_t value)
{
    target = value;
}
#else

#error "Compiler/platform not supported"

#endif

#endif // TOUCHGFX_ATOMIC_HPP
