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
#include <touchgfx/Utils.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>

namespace touchgfx
{
void PainterRGB888Bitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    assert((bitmap.getId() == BITMAP_INVALID || bitmap.getFormat() == Bitmap::RGB888 || bitmap.getFormat() == Bitmap::ARGB8888) && "The chosen painter only works with RGB888 and ARGB8888 bitmaps");
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterRGB888Bitmap::setOffset(int16_t x, int16_t y)
{
    xOffset = x;
    yOffset = y;
}

void PainterRGB888Bitmap::setTiled(bool tiled)
{
    isTiled = tiled;
}

void PainterRGB888Bitmap::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    uint8_t* p = ptr + (x + xAdjust) * 3;

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
        p -= currentX * 3;
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

    const uint8_t* const p_lineend = p + 3 * count;
    // Max number of pixels before we reach end of bitmap row
    unsigned int available = bitmapRectToFrameBuffer.width - currentX;
    if (bitmapRGB888Pointer)
    {
        const uint8_t* const rgb888_linestart = ((const uint8_t*)bitmap.getData()) + (currentY * bitmapRectToFrameBuffer.width) * 3;
        if (widgetAlpha == 0xFF)
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint8_t* const p_chunkend = p + length * 3;
                count -= length;
                do
                {
                    // Use alpha from covers directly
                    const uint8_t alpha = *covers++;
                    if (alpha == 0xFF)
                    {
                        // Solid pixel
                        *p++ = *bitmapRGB888Pointer++;
                        *p++ = *bitmapRGB888Pointer++;
                        *p++ = *bitmapRGB888Pointer++;
                    }
                    else
                    {
                        const uint8_t ialpha = 0xFF - alpha;
                        *p = LCD::div255(*bitmapRGB888Pointer++ * alpha + *p * ialpha);
                        p++;
                        *p = LCD::div255(*bitmapRGB888Pointer++ * alpha + *p * ialpha);
                        p++;
                        *p = LCD::div255(*bitmapRGB888Pointer++ * alpha + *p * ialpha);
                        p++;
                    }
                } while (p < p_chunkend);
                bitmapRGB888Pointer = rgb888_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
        else
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint8_t* const p_chunkend = p + length * 3;
                count -= length;
                do
                {
                    const uint8_t alpha = LCD::div255((*covers++) * widgetAlpha);
                    const uint8_t ialpha = 0xFF - alpha;
                    *p = LCD::div255(*bitmapRGB888Pointer++ * alpha + *p * ialpha);
                    p++;
                    *p = LCD::div255(*bitmapRGB888Pointer++ * alpha + *p * ialpha);
                    p++;
                    *p = LCD::div255(*bitmapRGB888Pointer++ * alpha + *p * ialpha);
                    p++;
                } while (p < p_chunkend);
                bitmapRGB888Pointer = rgb888_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
    }
    else if (bitmapARGB8888Pointer)
    {
        const uint32_t* const argb8888_linestart = ((const uint32_t*)bitmap.getData()) + (currentY * bitmapRectToFrameBuffer.width);
        if (widgetAlpha == 0xFF)
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint8_t* const p_chunkend = p + length * 3;
                count -= length;
                do
                {
                    const uint8_t srcAlpha = (*bitmapARGB8888Pointer) >> 24;
                    const uint8_t alpha = LCD::div255((*covers++) * srcAlpha);
                    if (alpha == 0xFF)
                    {
                        // Solid pixel
                        *p++ = (*bitmapARGB8888Pointer);       // Blue
                        *p++ = (*bitmapARGB8888Pointer) >> 8;  // Green
                        *p++ = (*bitmapARGB8888Pointer) >> 16; // Red
                    }
                    else
                    {
                        // Non-Transparent pixel
                        const uint8_t ialpha = 0xFF - alpha;
                        uint8_t cByte = (*bitmapARGB8888Pointer);
                        *p = LCD::div255(cByte * alpha + *p * ialpha);
                        p++;
                        cByte = (*bitmapARGB8888Pointer) >> 8;
                        *p = LCD::div255(cByte * alpha + *p * ialpha);
                        p++;
                        cByte = (*bitmapARGB8888Pointer) >> 16;
                        *p = LCD::div255(cByte * alpha + *p * ialpha);
                        p++;
                    }
                    bitmapARGB8888Pointer++;
                } while (p < p_chunkend);
                bitmapARGB8888Pointer = argb8888_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
        else
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint8_t* const p_chunkend = p + length * 3;
                count -= length;
                do
                {
                    const uint8_t srcAlpha = (*bitmapARGB8888Pointer) >> 24;
                    const uint8_t alpha = LCD::div255((*covers++) * LCD::div255(srcAlpha * widgetAlpha));
                    if (alpha)
                    {
                        const uint8_t ialpha = 0xFF - alpha;
                        uint8_t cByte = (*bitmapARGB8888Pointer);
                        *p = LCD::div255(cByte * alpha + *p * ialpha);
                        p++;
                        cByte = (*bitmapARGB8888Pointer) >> 8;
                        *p = LCD::div255(cByte * alpha + *p * ialpha);
                        p++;
                        cByte = (*bitmapARGB8888Pointer) >> 16;
                        *p = LCD::div255(cByte * alpha + *p * ialpha);
                        p++;
                    }
                    else
                    {
                        p += 3;
                    }
                    bitmapARGB8888Pointer++;
                } while (p < p_chunkend);
                bitmapARGB8888Pointer = argb8888_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
    }
}

bool PainterRGB888Bitmap::renderInit()
{
    bitmapARGB8888Pointer = 0;
    bitmapRGB888Pointer = 0;

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
        bitmapRGB888Pointer = bitmap.getData();
        if (!bitmapRGB888Pointer)
        {
            return false;
        }
        bitmapRGB888Pointer += (currentX + currentY * bitmapRectToFrameBuffer.width) * 3;
        return true;
    }

    return false;
}
} // namespace touchgfx
