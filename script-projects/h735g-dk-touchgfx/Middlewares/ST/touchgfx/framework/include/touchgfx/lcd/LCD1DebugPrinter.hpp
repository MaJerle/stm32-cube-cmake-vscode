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
 * @file touchgfx/lcd/LCD1DebugPrinter.hpp
 *
 * Declares the touchgfx::LCD1DebugPrinter class.
 */
#ifndef TOUCHGFX_LCD1DEBUGPRINTER_HPP
#define TOUCHGFX_LCD1DEBUGPRINTER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/DebugPrinter.hpp>

namespace touchgfx
{
/**
 * The class LCD1DebugPrinter implements the DebugPrinter interface for printing debug messages
 * on top of 1bit framebuffer.
 *
 * @see DebugPrinter
 */
class LCD1DebugPrinter : public DebugPrinter
{
public:
    virtual void draw(const Rect& rect) const;
};

} // namespace touchgfx

#endif // TOUCHGFX_LCD1DEBUGPRINTER_HPP
