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
 * @file platform/driver/lcd/LCD2bpp.hpp
 *
 * Declares the touchgfx::LCD2bpp class.
 */
#ifndef TOUCHGFX_LCD2BPP_HPP
#define TOUCHGFX_LCD2BPP_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/lcd/LCD2DebugPrinter.hpp>

namespace touchgfx
{
#define USE_LSB

/**
 * This class contains the various low-level drawing routines for drawing bitmaps, texts and
 * rectangles on 2 bits per pixel gray scale displays.
 *
 * @see LCD
 *
 * @note All coordinates are expected to be in absolute coordinates!
 */
class LCD2bpp : public LCD
{
public:
    LCD2bpp();

    virtual void drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true);

    virtual void blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    virtual void blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    virtual uint16_t* copyFrameBufferRegionToMemory(const Rect& visRegion, const Rect& absRegion, const BitmapId bitmapId);

    virtual void fillRect(const Rect& rect, colortype color, uint8_t alpha = 255);

    virtual void fillBuffer(uint8_t* const destination, uint16_t pixelStride, const Rect& rect, const colortype color, const uint8_t alpha);

    virtual uint8_t bitDepth() const
    {
        return 2;
    }

    virtual Bitmap::BitmapFormat framebufferFormat() const
    {
        return Bitmap::GRAY2;
    }

    virtual uint16_t framebufferStride() const
    {
        return getFramebufferStride();
    }

    /**
     * Framebuffer stride in bytes. The distance (in bytes) from the start of one
     * framebuffer row, to the next.
     *
     * @return The number of bytes in one framebuffer row.
     */
    FORCE_INLINE_FUNCTION static uint16_t getFramebufferStride()
    {
        assert(HAL::FRAME_BUFFER_WIDTH > 0 && "HAL has not been initialized yet");
        return (HAL::FRAME_BUFFER_WIDTH + 3) / 4;
    }

    /**
     * Generates a color representation to be used on the LCD, based on 24 bit RGB values.
     *
     * @param  red   Value of the red part (0-255).
     * @param  green Value of the green part (0-255).
     * @param  blue  Value of the blue part (0-255).
     *
     * @return The color representation depending on LCD color format.
     */
    FORCE_INLINE_FUNCTION static uint8_t getNativeColorFromRGB(uint8_t red, uint8_t green, uint8_t blue)
    {
        // Find the GRAY value (http://en.wikipedia.org/wiki/Luma_%28video%29) rounded to nearest integer
        return (red * 54 + green * 183 + blue * 19) >> 14;
    }

    /**
     * Generates a color representation to be used on the LCD, based on 24 bit RGB values.
     *
     * @param  color The color.
     *
     * @return The color representation depending on LCD color format.
     */
    FORCE_INLINE_FUNCTION static uint8_t getNativeColor(colortype color)
    {
        return getNativeColorFromRGB(Color::getRed(color), Color::getGreen(color), Color::getBlue(color));
    }

    /**
     * Enables the texture mappers for all image formats. This allows drawing any image
     * using Bilinear Interpolation and Nearest Neighbor algorithms, but might use a lot of
     * memory for the drawing algorithms.
     */
    void enableTextureMapperAll();

    /**
     * Enables the texture mappers for GRAY2 image format. This allows drawing GRAY2 images
     * using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperGRAY2_BilinearInterpolation,
     *      enableTextureMapperGRAY2_NearestNeighbor
     */
    void enableTextureMapperGRAY2();

    /**
     * Enables the texture mappers for GRAY2 image format. This allows drawing GRAY2 images
     * using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperGRAY2, enableTextureMapperGRAY2_NearestNeighbor
     */
    void enableTextureMapperGRAY2_BilinearInterpolation();

    /**
     * Enables the texture mappers for GRAY2 image format. This allows drawing GRAY2 images
     * using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperGRAY2, enableTextureMapperGRAY2_BilinearInterpolation
     */
    void enableTextureMapperGRAY2_NearestNeighbor();

    /**
     * Shift value to get the right pixel in a byte.
     *
     * @param  offset The offset.
     *
     * @return The shift value.
     */
    FORCE_INLINE_FUNCTION static int shiftVal(int offset)
    {
#ifdef USE_LSB
        return (offset & 3) << 1;
#else
        return (3 - (offset & 3)) << 1;
#endif
    }

    /**
     * Get pixel from buffer/image.
     *
     * @param  addr   The address.
     * @param  offset The offset.
     *
     * @return The pixel value.
     */
    FORCE_INLINE_FUNCTION static uint8_t getPixel(const uint8_t* addr, int offset)
    {
        return (addr[offset >> 2] >> shiftVal(offset)) & 3;
    }

    /**
     * Get pixel from buffer/image.
     *
     * @param  addr   The address.
     * @param  offset The offset.
     *
     * @return The pixel value.
     */
    FORCE_INLINE_FUNCTION static uint8_t getPixel(const uint16_t* addr, int offset)
    {
        return getPixel(reinterpret_cast<const uint8_t*>(addr), offset);
    }

    /**
     * Set pixel in buffer.
     *
     * @param [in] addr   The address.
     * @param      offset The offset.
     * @param      value  The value.
     */
    FORCE_INLINE_FUNCTION static void setPixel(uint8_t* addr, int offset, uint8_t value)
    {
        int shift = shiftVal(offset);
        addr[offset >> 2] = (addr[offset >> 2] & ~(3 << shift)) | ((value & 3) << shift);
    }

    /**
     * Set pixel in buffer.
     *
     * @param [in] addr   The address.
     * @param      offset The offset.
     * @param      value  The value.
     */
    FORCE_INLINE_FUNCTION static void setPixel(uint16_t* addr, int offset, uint8_t value)
    {
        setPixel(reinterpret_cast<uint8_t*>(addr), offset, value);
    }

protected:
    static const uint8_t alphaBlend[16][4][4]; ///< The alpha lookup table to avoid arithmetics when alpha blending

    virtual DrawTextureMapScanLineBase* getTextureMapperDrawScanLine(const TextureSurface& texture, RenderingVariant renderVariant, uint8_t alpha);

    /**
     * Find out how much to advance in the display buffer to get to the next pixel.
     *
     * @param  rotatedDisplay Is the display running in portrait mode?
     * @param  textRotation   Rotation to perform.
     *
     * @return How much to advance to get to the next pixel.
     */
    static int nextPixel(bool rotatedDisplay, TextRotation textRotation);

    /**
     * Find out how much to advance in the display buffer to get to the next line.
     *
     * @param  rotatedDisplay Is the display running in portrait mode?
     * @param  textRotation   Rotation to perform.
     *
     * @return How much to advance to get to the next line.
     */
    static int nextLine(bool rotatedDisplay, TextRotation textRotation);

    virtual void drawGlyph(uint16_t* wbuf16, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, uint8_t byteAlignRow, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation);

    /**
     * Blit a 2D source-array to the framebuffer performing alpha-blending per pixel as
     * specified Performs always a software blend.
     *
     * @param  sourceData16    The source-array pointer (points to the beginning of the
     *                         data). The sourceData must be stored as 2bpp GRAY2 values.
     * @param  sourceAlphaData The alpha channel array pointer (points to the beginning of
     *                         the data)
     * @param  source          The location and dimensions of the source.
     * @param  blitRect        A rectangle describing what region is to be drawn.
     * @param  alpha           The alpha value to use for blending applied to the whole
     *                         image (255 = solid, no blending)
     */
    static void blitCopyAlphaPerPixel(const uint16_t* sourceData16, const uint8_t* sourceAlphaData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Copies a rectangular area.
     *
     * @param      srcAddress     Source address (byte address).
     * @param      srcStride      Source stride (number of bytes to advance to next line).
     * @param      srcPixelOffset Source pixel offset (first pixel in first source byte).
     * @param [in] dstAddress     If destination address (byte address).
     * @param      dstStride      Destination stride (number of bytes to advance to next line).
     * @param      dstPixelOffset Destination pixel offset (first pixel in destination byte).
     * @param      width          The width of area (in pixels).
     * @param      height         The height of area (in pixels).
     */
    void copyRect(const uint8_t* srcAddress, uint16_t srcStride, uint8_t srcPixelOffset, uint8_t* RESTRICT dstAddress, uint16_t dstStride, uint8_t dstPixelOffset, uint16_t width, uint16_t height) const;

private:
    DrawTextureMapScanLineBase* textureMapper_GRAY2_NonOpaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_GRAY2_Opaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_GRAY2_NonOpaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_GRAY2_Opaque_BilinearInterpolation_GA;

    FORCE_INLINE_FUNCTION static uint8_t bilinearInterpolate8(uint8_t c00, uint8_t c10, uint8_t x)
    {
        assert(x < 16);
        uint16_t xy10 = 16 * x;
        uint16_t xy00 = 256 - xy10;

        return (c00 * xy00 + c10 * xy10) >> 8;
    }

    FORCE_INLINE_FUNCTION static uint8_t bilinearInterpolate8(uint8_t c00, uint8_t c10, uint8_t c01, uint8_t c11, uint8_t x, uint8_t y)
    {
        assert(x < 16 && y < 16);
        uint16_t xy11 = x * y;
        uint16_t xy10 = 16 * x - xy11;
        uint16_t xy01 = 16 * y - xy11;
        uint16_t xy00 = 256 - (xy11 + xy10 + xy01);

        return (c00 * xy00 + c10 * xy10 + c01 * xy01 + c11 * xy11) >> 8;
    }

    FORCE_INLINE_FUNCTION static uint8_t div255_2(uint16_t value)
    {
        return div255(value * 0x55) >> 6;
    }

    class DrawTextureMapScanLineBase2 : public DrawTextureMapScanLineBase
    {
    protected:
        FORCE_INLINE_FUNCTION bool overrunCheckBilinearInterpolation(uint32_t& destOffset, int& pixelsToDraw, fixed16_16& U, fixed16_16& V, fixed16_16 deltaU, fixed16_16 deltaV, const int16_t maxWidth, const int16_t maxHeight);
        FORCE_INLINE_FUNCTION bool overrunCheckNearestNeighbor(uint32_t& destOffset, int& pixelsToDraw, fixed16_16& U, fixed16_16& V, fixed16_16 deltaU, fixed16_16 deltaV, const int16_t maxWidth, const int16_t maxHeight);
    };

    class TextureMapper_GRAY2_NonOpaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase2
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* destAddress, uint32_t const destOffset, const uint16_t* const textureBits, const uint8_t* const alphaBits, const int16_t bitmapStride, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha);
        void writePixelOnEdge(uint16_t* destAddress, uint32_t const destOffset, const uint16_t* const textureBits, const uint8_t* const alphaBits, const int16_t bitmapStride, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha);
    };

    class TextureMapper_GRAY2_Opaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase2
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* destAddress, uint32_t const destOffset, const uint16_t* const textureBits, const int16_t bitmapStride, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha);
        void writePixelOnEdge(uint16_t* destAddress, uint32_t const destOffset, const uint16_t* const textureBits, const int16_t bitmapStride, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha);
    };

    class TextureMapper_GRAY2_NonOpaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase2
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* destAddress, uint32_t const destOffset, const uint16_t* const textureBits, const uint8_t* const alphaBits, const int16_t bitmapStride, const int UInt, const int VInt, const uint8_t alpha);
    };

    class TextureMapper_GRAY2_Opaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase2
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* destAddress, uint32_t const destOffset, const uint16_t* const textureBits, const int16_t bitmapStride, const int UInt, const int VInt, const uint8_t alpha);
    };

    class TextureMapper_A4_NearestNeighbor_GA : public DrawTextureMapScanLineBase2
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destAddress, const uint32_t destOffset, const uint8_t a4, const uint8_t alpha);
    };

    class TextureMapper_A4_BilinearInterpolation_GA : public DrawTextureMapScanLineBase2
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destAddress, const uint32_t destOffset, const uint16_t* const textureBits, const uint32_t offset, const int16_t bitmapStride, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha);
        void writePixelOnEdge(uint16_t* const destAddress, const uint32_t destOffset, const uint16_t* const textureBits, const uint16_t bitmapStride, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha);
    };
};

} // namespace touchgfx

#endif // TOUCHGFX_LCD2BPP_HPP
