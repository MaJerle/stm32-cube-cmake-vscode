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

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>
#include <touchgfx/widgets/canvas/PainterXRGB8888Bitmap.hpp>
#include <platform/driver/lcd/LCD32bpp_XRGB8888.hpp>

namespace touchgfx
{
void PainterXRGB8888Bitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    assert((bitmap.getId() == BITMAP_INVALID || bitmap.getFormat() == Bitmap::RGB565 || bitmap.getFormat() == Bitmap::RGB888 || bitmap.getFormat() == Bitmap::ARGB8888) && "The chosen painter only works with RGB565, RGB888 and ARGB8888 bitmaps");
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterXRGB8888Bitmap::setOffset(int16_t x, int16_t y)
{
    xOffset = x;
    yOffset = y;
}

void PainterXRGB8888Bitmap::setTiled(bool tiled)
{
    isTiled = tiled;
}

void PainterXRGB8888Bitmap::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    uint32_t* RESTRICT p32 = reinterpret_cast<uint32_t*>(ptr) + (x + xAdjust);

    currentX = x + areaOffsetX + xOffset;
    currentY = y + areaOffsetY + yOffset;

    if (!isTiled && currentX < 0)
    {
        if (count < (unsigned int)-currentX)
        {
            return;
        }
        count += currentX;
        covers -= currentX;
        p32 -= currentX;
        currentX = 0;
    }

    if (!renderInit())
    {
        return;
    }

    if (!isTiled && currentX + (int)count > bitmapRectToFrameBuffer.width)
    {
        count = bitmapRectToFrameBuffer.width - currentX;
    }

    const uint32_t* const p32_lineend = p32 + count;
    // Max number of pixels before we reach end of bitmap row
    unsigned int available = bitmapRectToFrameBuffer.width - currentX;
    if (bitmapARGB8888Pointer)
    {
        const uint32_t* const argb8888_linestart = ((const uint32_t*)bitmap.getData()) + (currentY * bitmapRectToFrameBuffer.width);
        if (widgetAlpha == 0xFF)
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint32_t* const p32_chunkend = p32 + length;
                count -= length;
                do
                {
                    const uint8_t srcAlpha = (*bitmapARGB8888Pointer) >> 24;
                    const uint8_t alpha = LCD::div255((*covers++) * srcAlpha);
                    if (alpha == 0xFF)
                    {
                        *p32 = *bitmapARGB8888Pointer;
                    }
                    else
                    {
                        *p32 = LCD32bpp_XRGB8888::blendRgb888withXrgb8888(reinterpret_cast<const uint8_t*>(bitmapARGB8888Pointer), *p32, alpha, 0xFF - alpha);
                    }
                    bitmapARGB8888Pointer++;
                    p32++;
                } while (p32 < p32_chunkend);
                bitmapARGB8888Pointer = argb8888_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p32 < p32_lineend);
        }
        else
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint32_t* const p32_chunkend = p32 + length;
                count -= length;
                do
                {
                    const uint8_t srcAlpha = (*bitmapARGB8888Pointer) >> 24;
                    const uint8_t alpha = LCD::div255((*covers++) * LCD::div255(srcAlpha * widgetAlpha));
                    if (alpha)
                    {
                        *p32 = LCD32bpp_XRGB8888::blendRgb888withXrgb8888(reinterpret_cast<const uint8_t*>(bitmapARGB8888Pointer), *p32, alpha, 0xFF - alpha);
                    }
                    bitmapARGB8888Pointer++;
                    p32++;
                } while (p32 < p32_chunkend);
                bitmapARGB8888Pointer = argb8888_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p32 < p32_lineend);
        }
    }
    else if (bitmapRGB888Pointer)
    {
        const uint8_t* const rgb888_linestart = ((const uint8_t*)bitmap.getData()) + (currentY * bitmapRectToFrameBuffer.width) * 3;
        if (widgetAlpha == 0xFF)
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint32_t* const p32_chunkend = p32 + length;
                count -= length;
                do
                {
                    const uint8_t alpha = *covers++;
                    if (alpha == 0xFF)
                    {
                        // Opaque pixel
                        *p32 = LCD32bpp_XRGB8888::rgb888toXrgb8888(bitmapRGB888Pointer);
                    }
                    else
                    {
                        // Non-Opaque pixel
                        *p32 = LCD32bpp_XRGB8888::blendRgb888withXrgb8888(bitmapRGB888Pointer, *p32, alpha, 0xFF - alpha);
                    }
                    bitmapRGB888Pointer += 3;
                    p32++;
                } while (p32 < p32_chunkend);
                bitmapRGB888Pointer = rgb888_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p32 < p32_lineend);
        }
        else
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint32_t* const p32_chunkend = p32 + length;
                count -= length;
                do
                {
                    const uint8_t alpha = LCD::div255((*covers++) * widgetAlpha);
                    if (alpha)
                    {
                        *p32 = LCD32bpp_XRGB8888::blendRgb888withXrgb8888(bitmapRGB888Pointer, *p32, alpha, 0xFF - alpha);
                    }
                    bitmapRGB888Pointer += 3;
                    p32++;
                } while (p32 < p32_chunkend);
                bitmapRGB888Pointer = rgb888_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p32 < p32_lineend);
        }
    }
    else if (bitmapRGB565Pointer)
    {
        const uint16_t* const rgb565_linestart = ((const uint16_t*)bitmap.getData()) + (currentY * bitmapRectToFrameBuffer.width);
        if (widgetAlpha == 0xFF)
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint32_t* const p32_chunkend = p32 + length;
                count -= length;
                do
                {
                    const uint8_t alpha = *covers++;
                    const uint16_t srcpix = *bitmapRGB565Pointer++;
                    if (alpha == 0xFF)
                    {
                        *p32 = Color::rgb565toXrgb8888(srcpix);
                    }
                    else
                    {
                        *p32 = LCD32bpp_XRGB8888::blendRgb565withXrgb8888(srcpix, *p32, alpha, 0xFF - alpha);
                    }
                    p32++;
                } while (p32 < p32_chunkend);
                bitmapRGB565Pointer = rgb565_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p32 < p32_lineend);
        }
        else
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint32_t* const p32_chunkend = p32 + length;
                count -= length;
                do
                {
                    const uint8_t alpha = LCD::div255((*covers++) * widgetAlpha);
                    uint16_t srcpix = *bitmapRGB565Pointer++;
                    if (alpha)
                    {
                        *p32 = LCD32bpp_XRGB8888::blendRgb565withXrgb8888(srcpix, *p32, alpha, 0xFF - alpha);
                    }
                    p32++;
                } while (p32 < p32_chunkend);
                bitmapRGB565Pointer = rgb565_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p32 < p32_lineend);
        }
    }
}

bool PainterXRGB8888Bitmap::renderInit()
{
    bitmapARGB8888Pointer = 0;
    bitmapRGB888Pointer = 0;
    bitmapRGB565Pointer = 0;

    if (bitmap.getId() == BITMAP_INVALID)
    {
        return false;
    }

    if (isTiled)
    {
        // Modulus, also handling negative values
        currentX = ((currentX % bitmapRectToFrameBuffer.width) + bitmapRectToFrameBuffer.width) % bitmapRectToFrameBuffer.width;
        currentY = ((currentY % bitmapRectToFrameBuffer.height) + bitmapRectToFrameBuffer.height) % bitmapRectToFrameBuffer.height;
    }
    else if ((currentX >= bitmapRectToFrameBuffer.width) || (currentY < 0) || (currentY >= bitmapRectToFrameBuffer.height))
    {
        return false;
    }

    if (bitmap.getFormat() == Bitmap::ARGB8888)
    {
        bitmapARGB8888Pointer = (const uint32_t*)bitmap.getData();
        if (!bitmapARGB8888Pointer)
        {
            return false;
        }
        bitmapARGB8888Pointer += currentX + currentY * bitmapRectToFrameBuffer.width;
        return true;
    }

    if (bitmap.getFormat() == Bitmap::RGB888)
    {
        bitmapRGB888Pointer = (const uint8_t*)bitmap.getData();
        if (!bitmapRGB888Pointer)
        {
            return false;
        }
        bitmapRGB888Pointer += (currentX + currentY * bitmapRectToFrameBuffer.width) * 3;
        return true;
    }

    if (bitmap.getFormat() == Bitmap::RGB565)
    {
        bitmapRGB565Pointer = (const uint16_t*)bitmap.getData();
        if (!bitmapRGB565Pointer)
        {
            return false;
        }
        bitmapRGB565Pointer += currentX + currentY * bitmapRectToFrameBuffer.width;
        return true;
    }

    return false;
}
} // namespace touchgfx
