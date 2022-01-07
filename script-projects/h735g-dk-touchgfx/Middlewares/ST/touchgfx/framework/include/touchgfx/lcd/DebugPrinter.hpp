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
 * @file touchgfx/lcd/DebugPrinter.hpp
 *
 * Declares the touchgfx::DebugPrinter class.
 */
#ifndef TOUCHGFX_DEBUGPRINTER_HPP
#define TOUCHGFX_DEBUGPRINTER_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * The class DebugPrinter defines the interface for printing debug messages on top of the
 * framebuffer.
 */
class DebugPrinter
{
public:
    /** Initializes a new instance of the DebugPrinter class. */
    DebugPrinter()
        : debugString(0), debugRegion(Rect(0, 0, 0, 0)), debugForegroundColor(colortype(0xffffffff)), debugScale(1)
    {
    }

    /** Finalizes an instance of the DebugPrinter class. */
    virtual ~DebugPrinter()
    {
    }

    /**
     * Sets the debug string to be displayed on top of the framebuffer.
     *
     * @param [in] string The string to be displayed.
     */
    void setString(const char* string)
    {
        debugString = string;
    }

    /**
     * Sets the position onscreen where the debug string will be displayed.
     *
     * @param [in] x The coordinate of the region where the debug string is displayed.
     * @param [in] y The coordinate of the region where the debug string is displayed.
     * @param [in] w The width of the region where the debug string is displayed.
     * @param [in] h The height of the region where the debug string is displayed.
     */
    void setPosition(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
        debugRegion = Rect(x, y, w, h);
    }

    /**
     * Sets the font scale of the debug string.
     *
     * @param [in] scale The font scale of the debug string.
     */
    void setScale(uint8_t scale)
    {
        if (!scale)
        {
            scale = 1;
        }

        debugScale = scale;
    }

    /**
     * Sets the foreground color of the debug string.
     *
     * @param [in] fg The foreground color of the debug string.
     */
    void setColor(colortype fg)
    {
        debugForegroundColor = fg;
    }

    /**
     * Draws the debug string on top of the framebuffer content.
     *
     * @param [in] rect The rect to draw inside.
     */
    virtual void draw(const Rect& rect) const = 0;

    /**
     * Returns the region where the debug string is displayed.
     *
     * @return Rect The debug string region.
     */
    const Rect& getRegion() const
    {
        return debugRegion;
    }

protected:
    /**
     * Gets a glyph (15 bits) arranged with 3 bits wide, 5 bits high in a single uint16_t value.
     *
     * @param  c The character to get a glyph for.
     *
     * @return The glyph.
     */
    uint16_t getGlyph(uint8_t c) const
    {
        static const uint16_t builtin_debug_font[] = {
            000000, 022202, 055000, 057575, 026532, 051245, 025253, 022000,
            012221, 042224, 005250, 002720, 000024, 000700, 000002, 011244,
            025752, 026222, 061247, 061216, 045571, 074616, 034652, 071222,
            025252, 025312, 002020, 002024, 012421, 007070, 042124, 061202,
            025543, 025755, 065656, 034443, 065556, 074647, 074644, 034553,
            055755, 072227, 011152, 055655, 044447, 057555, 015754, 025552,
            065644, 025573, 065655, 034216, 072222, 055557, 055522, 055575,
            055255, 055222, 071247, 032223, 044211, 062226, 025000, 000007,
            042000, 003553, 046556, 003443, 013553, 002743, 012722, 002716,
            046555, 020627, 010316, 045655, 062227, 006777, 006555, 002552,
            006564, 003531, 006544, 003636, 022721, 005553, 005522, 005575,
            005255, 005316, 007247, 032623, 022222, 062326, 063000, 077777
        };

        if (c < ' ' || c > '~')
        {
            c = 0x7F;
        }
        return builtin_debug_font[c - ' '];
    }

    const char* debugString;        ///< Debug string to be displayed on screen.
    Rect debugRegion;               ///< Region on screen where the debug message is displayed.
    colortype debugForegroundColor; ///< Font color to use when displaying the debug string.
    uint8_t debugScale;             ///< Font scaling factor to use when displaying the debug string.
};

} // namespace touchgfx

#endif // TOUCHGFX_DEBUGPRINTER_HPP
