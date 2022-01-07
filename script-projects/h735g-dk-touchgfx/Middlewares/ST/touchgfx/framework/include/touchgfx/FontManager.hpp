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
 * @file touchgfx/FontManager.hpp
 *
 * Declares the touchgfx::FontProvider class.
 */
#ifndef TOUCHGFX_FONTMANAGER_HPP
#define TOUCHGFX_FONTMANAGER_HPP

#include <touchgfx/Font.hpp>

namespace touchgfx
{
/**
 * A generic pure virtual definition of a FontProvider, which is a class capable of returning a
 * Font based on a FontId. An application-specific derivation of this class must be
 * implemented.
 */
class FontProvider
{
public:
    /**
     * Gets a Font.
     *
     * @param  fontId The FontId of the font to get.
     *
     * @return The font with a font id of fontId.
     */
    virtual Font* getFont(FontId fontId) = 0;

    /** Finalizes an instance of the FontProvider class. */
    virtual ~FontProvider()
    {
    }

private:
};

/**
 * This class is the entry point for looking up a font based on a font id. Must be initialized
 * with the appropriate FontProvider by the application.
 */
class FontManager
{
public:
    /**
     * Sets the font provider. Must be initialized with the appropriate FontProvider by the
     * application.
     *
     * @param [in] fontProvider Sets the font provider. Must be initialized with the
     *                          appropriate FontProvider by the application.
     */
    static void setFontProvider(FontProvider* fontProvider);

    /**
     * Gets a font.
     *
     * @param  fontId The FontId of the font to get.
     *
     * @return The font with a FontId of fontId.
     */
    static Font* getFont(FontId fontId);

private:
    static FontProvider* provider;
};

} // namespace touchgfx

#endif // TOUCHGFX_FONTMANAGER_HPP
