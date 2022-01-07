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
 * @file touchgfx/lcd/LCD8DebugPrinterBase.hpp
 *
 * Declares the touchgfx::LCD8DebugPrinterBase class.
 */
#ifndef TOUCHGFX_LCD8DEBUGPRINTERBASE_HPP
#define TOUCHGFX_LCD8DEBUGPRINTERBASE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/DebugPrinter.hpp>

namespace touchgfx
{
/**
 * The class LCD8DebugPrinterBase defines the abstract base class used for printing debug messages
 * on top of a specific 8bit framebuffer, with a specifc color type.
 *
 * @note The DebugPrinter interface is implemented in each specific LCD8 debug printer.
 * @see LCD8ABGR2222DebugPrinter, LCD8ARGB2222DebugPrinter, LCD8BGRA2222DebugPrinter, LCD8RGBA2222DebugPrinter
 *
 */
class LCD8DebugPrinterBase : public DebugPrinter
{
protected:
    /**
     * Draws the DebugPrinter::debugString on top of the framebuffer content.
     *
     * @param [in] rect The rect to draw inside.
     * @param [in] debugColorType The specific 8bit color type to use.
     */
    void drawColorType(const Rect& rect, const uint8_t debugColorType) const;
};

} // namespace touchgfx

#endif // TOUCHGFX_LCD8DEBUGPRINTERBASE_HPP
