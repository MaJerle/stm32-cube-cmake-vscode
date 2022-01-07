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
#include <touchgfx/Utils.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565Bitmap.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>

namespace touchgfx
{
void PainterRGB565Bitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    assert((bitmap.getId() == BITMAP_INVALID || bitmap.getFormat() == Bitmap::RGB565 || bitmap.getFormat() == Bitmap::ARGB8888) && "The chosen painter only works with RGB565 and ARGB8888 bitmaps");
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterRGB565Bitmap::setOffset(int16_t x, int16_t y)
{
    xOffset = x;
    yOffset = y;
}

void PainterRGB565Bitmap::setTiled(bool tiled)
{
    isTiled = tiled;
}

void PainterRGB565Bitmap::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    uint16_t* p = reinterpret_cast<uint16_t*>(ptr) + (x + xAdjust);

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
        p -= currentX;
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

    const uint16_t* const p_lineend = p + count;
    // Max number of pixels before we reach end of bitmap row
    unsigned int available = bitmapRectToFrameBuffer.width - currentX;
    if (bitmapRGB565Pointer)
    {
        const uint16_t* const rgb565_linestart = ((const uint16_t*)bitmap.getData()) + (currentY * bitmapRectToFrameBuffer.width);
        if (bitmapAlphaPointer)
        {
            const uint8_t* const alpha_linestart = bitmap.getExtraData() + currentY * bitmapRectToFrameBuffer.width;
            if (widgetAlpha == 0xFF)
            {
                do
                {
                    const unsigned length = MIN(available, count);
                    const uint16_t* const p_chunkend = p + length;
                    count -= length;
                    do
                    {
                        const uint8_t alpha = LCD::div255((*covers++) * (*bitmapAlphaPointer++));
                        if (alpha == 0xFF)
                        {
                            // Solid pixel
                            *p = *bitmapRGB565Pointer;
                        }
                        else if (alpha)
                        {
                            // Non-Transparent pixel
                            *p = mixColors(*bitmapRGB565Pointer, *p, alpha);
                        }
                        p++;
                        bitmapRGB565Pointer++;
                    } while (p < p_chunkend);
                    bitmapRGB565Pointer = rgb565_linestart;
                    bitmapAlphaPointer = alpha_linestart;
                    available = bitmapRectToFrameBuffer.width;
                } while (p < p_lineend);
            }
            else
            {
                do
                {
                    const unsigned length = MIN(available, count);
                    const uint16_t* const p_chunkend = p + length;
                    count -= length;
                    do
                    {
                        const uint8_t alpha = LCD::div255((*covers++) * LCD::div255((*bitmapAlphaPointer++) * widgetAlpha));
                        if (alpha) // This can never get to max=0XFF as totalAlpha<0xFF
                        {
                            // Non-Transparent pixel
                            *p = mixColors(*bitmapRGB565Pointer, *p, alpha);
                        }
                        p++;
                        bitmapRGB565Pointer++;
                    } while (p < p_chunkend);
                    bitmapRGB565Pointer = rgb565_linestart;
                    bitmapAlphaPointer = alpha_linestart;
                    available = bitmapRectToFrameBuffer.width;
                } while (p < p_lineend);
            }
        }
        else
        {
            if (widgetAlpha == 0xFF)
            {
                do
                {
                    const unsigned length = MIN(available, count);
                    const uint16_t* const p_chunkend = p + length;
                    count -= length;
                    do
                    {
                        // Use alpha from covers directly
                        const uint8_t alpha = *covers++;
                        if (alpha == 0xFF)
                        {
                            // Solid pixel
                            *p = *bitmapRGB565Pointer;
                        }
                        else
                        {
                            // Non-Transparent pixel
                            *p = mixColors(*bitmapRGB565Pointer, *p, alpha);
                        }
                        p++;
                        bitmapRGB565Pointer++;
                    } while (p < p_chunkend);
                    bitmapRGB565Pointer = rgb565_linestart;
                    available = bitmapRectToFrameBuffer.width;
                } while (p < p_lineend);
            }
            else
            {
                do
                {
                    const unsigned length = MIN(available, count);
                    const uint16_t* const p_chunkend = p + length;
                    count -= length;
                    do
                    {
                        const uint8_t alpha = LCD::div255((*covers++) * widgetAlpha);

                        *p = mixColors(*bitmapRGB565Pointer, *p, alpha);

                        p++;
                        bitmapRGB565Pointer++;
                    } while (p < p_chunkend);
                    bitmapRGB565Pointer = rgb565_linestart;
                    available = bitmapRectToFrameBuffer.width;
                } while (p < p_lineend);
            }
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
                const uint16_t* const p_chunkend = p + length;
                count -= length;
                do
                {
                    const uint8_t srcAlpha = (*bitmapARGB8888Pointer) >> 24;
                    const uint8_t alpha = LCD::div255((*covers++) * srcAlpha);
                    if (alpha == 0xFF)
                    {
                        *p = LCD16bpp::getNativeColor(*bitmapARGB8888Pointer);
                    }
                    else if (alpha)
                    {
                        const uint32_t newpix = *bitmapARGB8888Pointer;
                        *p = mixColors((newpix >> 8) & RMASK, (newpix >> 5) & GMASK, (newpix >> 3) & BMASK, *p, alpha);
                    }
                    p++;
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
                const uint16_t* const p_chunkend = p + length;
                count -= length;
                do
                {
                    const uint8_t srcAlpha = (*bitmapARGB8888Pointer) >> 24;
                    const uint8_t alpha = LCD::div255((*covers++) * LCD::div255(srcAlpha * widgetAlpha));
                    if (alpha)
                    {
                        const uint32_t newpix = *bitmapARGB8888Pointer;
                        *p = mixColors((newpix >> 8) & RMASK, (newpix >> 5) & GMASK, (newpix >> 3) & BMASK, *p, alpha);
                    }
                    p++;
                    bitmapARGB8888Pointer++;
                } while (p < p_chunkend);
                bitmapARGB8888Pointer = argb8888_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
    }
}

bool PainterRGB565Bitmap::renderInit()
{
    bitmapARGB8888Pointer = 0;
    bitmapRGB565Pointer = 0;
    bitmapAlphaPointer = 0;

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

    if (bitmap.getFormat() == Bitmap::RGB565)
    {
        bitmapRGB565Pointer = (const uint16_t*)bitmap.getData();
        if (!bitmapRGB565Pointer)
        {
            return false;
        }
        bitmapRGB565Pointer += currentX + currentY * bitmapRectToFrameBuffer.width;
        // Get alpha data (RGB565 format)
        bitmapAlphaPointer = (const uint8_t*)bitmap.getExtraData();
        if (bitmapAlphaPointer)
        {
            bitmapAlphaPointer += currentX + currentY * bitmapRectToFrameBuffer.width;
        }
        return true;
    }

    return false;
}
} // namespace touchgfx
