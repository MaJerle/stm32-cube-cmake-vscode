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
 * @file common/TouchGFXInit.hpp
 *
 * Declares the touch graphics generic initialization function.
 */
#ifndef TOUCHGFX_TOUCHGFXINIT_HPP
#define TOUCHGFX_TOUCHGFXINIT_HPP

#include <BitmapDatabase.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Application.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/hal/DMA.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <platform/driver/touch/TouchController.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include <texts/TypedTextDatabase.hpp>
#include <gui/common/FrontendHeap.hpp>

static ApplicationFontProvider fontProvider; ///< The font provider

/**
 * The global touchgfx namespace. All TouchGFX framework classes and global functions are placed in this namespace.
 */
namespace touchgfx
{
/// @cond

static Texts texts; ///< The texts

template <class T>
HAL& getHAL(DMA_Interface& dma, LCD& display, TouchController& tc, int16_t width, int16_t height)
{
    static T hal(dma, display, tc, width, height);
    return hal;
}
/// @endcond

/**
 * @globalfn
 */

/**
 * TouchGFX generic initialize.
 *
 * @tparam HALType The class type of the HAL subclass used for this port.
 * @param [in] dma                    Reference to the DMA implementation object to use. Can be of
 *                                    type NoDMA to disable the use of DMA for rendering.
 * @param [in] display                Reference to the LCD renderer implementation (subclass of
 *                                    LCD). Could be either LCD16bpp for RGB565 UIs, or
 *                                    LCD1bpp for monochrome UIs or LCD24bpp for 24bit
 *                                    displays using RGB888 UIs.
 * @param [in] tc                     Reference to the touch controller driver (or
 *                                    NoTouchController to disable touch input).
 * @param      width                  The \a native display width of the actual display, in pixels.
 *                                    This value is irrespective of whether the concrete UI
 *                                    should be portrait or landscape mode. It must match
 *                                    what the display itself is configured as.
 * @param      height                 The \a native display height of the actual display, in
 *                                    pixels. This value is irrespective of whether the
 *                                    concrete UI should be portrait or landscape mode. It
 *                                    must match what the display itself is configured as.
 * @param [in] bitmapCache            Optional pointer to starting address of a memory region in
 *                                    which to place the bitmap cache. Usually in external
 *                                    RAM. Pass 0 if bitmap caching is not used.
 * @param      bitmapCacheSize        Size of bitmap cache in bytes. Pass 0 if bitmap cache is not
 *                                    used.
 * @param      numberOfDynamicBitmaps (Optional) Number of dynamic bitmaps.
 *
 * @return A reference to the allocated (and initialized) HAL object.
 */
template <class HALType>
HAL& touchgfx_generic_init(DMA_Interface& dma, LCD& display, TouchController& tc, int16_t width, int16_t height,
                           uint16_t* bitmapCache, uint32_t bitmapCacheSize, uint32_t numberOfDynamicBitmaps = 0)
{
    HAL& hal = getHAL<HALType>(dma, display, tc, width, height);
    hal.initialize();

    Bitmap::registerBitmapDatabase(BitmapDatabase::getInstance(),
                                   BitmapDatabase::getInstanceSize(),
                                   bitmapCache,
                                   bitmapCacheSize,
                                   numberOfDynamicBitmaps);

    TypedText::registerTexts(&texts);
    Texts::setLanguage(0);

    FontManager::setFontProvider(&fontProvider);

    FrontendHeap::getInstance(); // We need to initialize the frontend heap.

    hal.registerEventListener(*(Application::getInstance()));

    return hal;
}

} // namespace touchgfx

#endif // TOUCHGFX_TOUCHGFXINIT_HPP
