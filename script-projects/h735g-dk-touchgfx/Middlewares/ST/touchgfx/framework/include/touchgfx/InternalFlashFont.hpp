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
 * @file touchgfx/InternalFlashFont.hpp
 *
 * Declares the touchgfx::InternalFlashFont class.
 */
#ifndef TOUCHGFX_INTERNALFLASHFONT_HPP
#define TOUCHGFX_INTERNALFLASHFONT_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/ConstFont.hpp>
#include <touchgfx/Font.hpp>

namespace touchgfx
{
/**
 * An InternalFlashFont has both glyph table and glyph data placed in a flash which supports
 * random access read (i.e. not a NAND flash)
 *
 * @see Font, ConstFont
 */
class InternalFlashFont : public ConstFont
{
public:
    /**
     * Initializes a new instance of the InternalFlashFont class.
     *
     * @param  list                   The array of glyphs known to this font.
     * @param  size                   The number of glyphs in list.
     * @param  height                 The height in pixels of the highest character in this font.
     * @param  pixBelowBase           The maximum number of pixels that can be drawn below the
     *                                baseline in this font.
     * @param  bitsPerPixel           The number of bits per pixel in this font.
     * @param  byteAlignRow           The glyphs are saved with each row byte aligned.
     * @param  maxLeft                The maximum a character extends to the left.
     * @param  maxRight               The maximum a character extends to the right.
     * @param  glyphDataInternalFlash Pointer to the glyph data for the font, placed in internal
     *                                flash.
     * @param  kerningList            pointer to the kerning data for the font, placed in internal
     *                                flash.
     * @param  fallbackChar           The fallback character for the typography in case no glyph is
     *                                available.
     * @param  ellipsisChar           The ellipsis character used for truncating long texts.
     */
    InternalFlashFont(const GlyphNode* list, uint16_t size, uint16_t height, uint8_t pixBelowBase, uint8_t bitsPerPixel, uint8_t byteAlignRow, uint8_t maxLeft, uint8_t maxRight, const uint8_t* glyphDataInternalFlash, const KerningNode* kerningList, const Unicode::UnicodeChar fallbackChar, const Unicode::UnicodeChar ellipsisChar);

    virtual const uint8_t* getPixelData(const GlyphNode* glyph) const;

    virtual int8_t getKerning(Unicode::UnicodeChar prevChar, const GlyphNode* glyph) const;

private:
    InternalFlashFont()
        : ConstFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0), glyphData(0), kerningData(0)
    {
    }
    const uint8_t* glyphData;       ///< Information describing the glyph
    const KerningNode* kerningData; ///< Information describing the kerning
};

} // namespace touchgfx

#endif // TOUCHGFX_INTERNALFLASHFONT_HPP
