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
#include <touchgfx/widgets/canvas/PainterARGB8888Bitmap.hpp>

namespace touchgfx
{
void PainterARGB8888Bitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    assert((bitmap.getId() == BITMAP_INVALID || bitmap.getFormat() == Bitmap::RGB565 || bitmap.getFormat() == Bitmap::RGB888 || bitmap.getFormat() == Bitmap::ARGB8888) && "The chosen painter only works with RGB565, RGB888 and ARGB8888 bitmaps");
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterARGB8888Bitmap::setOffset(int16_t x, int16_t y)
{
    xOffset = x;
    yOffset = y;
}

void PainterARGB8888Bitmap::setTiled(bool tiled)
{
    isTiled = tiled;
}

void PainterARGB8888Bitmap::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    uint8_t* RESTRICT p = ptr + (x + xAdjust) * 4;

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
        p -= currentX * 4;
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

    const uint8_t* const p_lineend = p + count * 4;
    // Max number of pixels before we reach end of bitmap row
    unsigned int available = bitmapRectToFrameBuffer.width - currentX;
    if (bitmapARGB8888Pointer)
    {
        const uint32_t* const argb8888_linestart = ((const uint32_t*)bitmap.getData()) + (currentY * bitmapRectToFrameBuffer.width);
        do
        {
            const unsigned length = MIN(available, count);
            const uint8_t* const p_chunkend = p + length * 4;
            count -= length;
            do
            {
                const uint8_t srcAlpha = (*bitmapARGB8888Pointer) >> 24;
                const uint8_t alphaFg = LCD::div255((*covers++) * LCD::div255(srcAlpha * widgetAlpha));
                const uint8_t alphaBg = p[3];
                if (alphaFg == 255 || alphaBg == 0)
                {
                    const uint8_t blueFg = *bitmapARGB8888Pointer;
                    *p++ = blueFg;
                    const uint8_t greenFg = (*bitmapARGB8888Pointer) >> 8;
                    *p++ = greenFg;
                    const uint8_t redFg = (*bitmapARGB8888Pointer) >> 16;
                    *p++ = redFg;
                    *p++ = alphaFg;
                }
                else if (alphaFg)
                {
                    const uint8_t alphaMult = LCD::div255(alphaFg * alphaBg);
                    const uint8_t alphaOut = alphaFg + alphaBg - alphaMult;

                    const uint8_t blueBg = *p;
                    const uint8_t blueFg = *bitmapARGB8888Pointer;
                    *p++ = (blueFg * alphaFg + blueBg * (alphaBg - alphaMult)) / alphaOut;
                    const uint8_t greenBg = *p;
                    const uint8_t greenFg = (*bitmapARGB8888Pointer) >> 8;
                    *p++ = (greenFg * alphaFg + greenBg * (alphaBg - alphaMult)) / alphaOut;
                    const uint8_t redBg = *p;
                    const uint8_t redFg = (*bitmapARGB8888Pointer) >> 16;
                    *p++ = (redFg * alphaFg + redBg * (alphaBg - alphaMult)) / alphaOut;
                    *p++ = alphaOut;
                }
                else
                {
                    p += 4;
                }
                bitmapARGB8888Pointer++;
            } while (p < p_chunkend);
            bitmapARGB8888Pointer = argb8888_linestart;
            available = bitmapRectToFrameBuffer.width;
        } while (p < p_lineend);
    }
    else if (bitmapRGB888Pointer)
    {
        const uint8_t* const rgb888_linestart = ((const uint8_t*)bitmap.getData()) + (currentY * bitmapRectToFrameBuffer.width) * 3;
        do
        {
            const unsigned length = MIN(available, count);
            const uint8_t* const p_chunkend = p + length * 4;
            count -= length;
            do
            {
                const uint8_t alphaFg = LCD::div255((*covers++) * widgetAlpha);
                if (alphaFg)
                {
                    const uint8_t alphaBg = p[3];
                    const uint8_t alphaMult = LCD::div255(alphaFg * alphaBg);
                    const uint8_t alphaOut = alphaFg + alphaBg - alphaMult;

                    const uint8_t blueBg = *p;
                    const uint8_t blueFg = *bitmapRGB888Pointer++;
                    *p++ = (blueFg * alphaFg + blueBg * alphaBg - blueBg * alphaMult) / alphaOut;
                    const uint8_t greenBg = *p;
                    const uint8_t greenFg = *bitmapRGB888Pointer++;
                    *p++ = (greenFg * alphaFg + greenBg * alphaBg - greenBg * alphaMult) / alphaOut;
                    const uint8_t redBg = *p;
                    const uint8_t redFg = *bitmapRGB888Pointer++;
                    *p++ = (redFg * alphaFg + redBg * alphaBg - redBg * alphaMult) / alphaOut;
                    *p++ = alphaOut;
                }
                else
                {
                    bitmapRGB888Pointer += 3;
                    p += 4;
                }
            } while (p < p_chunkend);
            bitmapRGB888Pointer = rgb888_linestart;
            available = bitmapRectToFrameBuffer.width;
        } while (p < p_lineend);
    }
    else if (bitmapRGB565Pointer)
    {
        const uint16_t* const rgb565_linestart = ((const uint16_t*)bitmap.getData()) + (currentY * bitmapRectToFrameBuffer.width);
        if (bitmapRGB565AlphaPointer == 0)
        {
            do
            {
                const unsigned length = MIN(available, count);
                const uint8_t* const p_chunkend = p + length * 4;
                count -= length;
                do
                {
                    const uint8_t alphaFg = LCD::div255((*covers++) * widgetAlpha);
                    if (alphaFg)
                    {
                        const uint8_t alphaBg = p[3];
                        const uint8_t alphaMult = LCD::div255(alphaFg * alphaBg);
                        const uint8_t alphaOut = alphaFg + alphaBg - alphaMult;

                        const uint8_t blueBg = *p;
                        const uint8_t blueFg = Color::getBlueFromRGB565(*bitmapRGB565Pointer);
                        *p++ = (blueFg * alphaFg + blueBg * alphaBg - blueBg * alphaMult) / alphaOut;
                        const uint8_t greenBg = *p;
                        const uint8_t greenFg = Color::getGreenFromRGB565(*bitmapRGB565Pointer);
                        *p++ = (greenFg * alphaFg + greenBg * alphaBg - greenBg * alphaMult) / alphaOut;
                        const uint8_t redBg = *p;
                        const uint8_t redFg = Color::getRedFromRGB565(*bitmapRGB565Pointer);
                        *p++ = (redFg * alphaFg + redBg * alphaBg - redBg * alphaMult) / alphaOut;
                        *p++ = alphaOut;
                    }
                    else
                    {
                        p += 4;
                    }
                    bitmapRGB565Pointer++;
                } while (p < p_chunkend);
                bitmapRGB565Pointer = rgb565_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
        else
        {
            const uint8_t* const alpha_linestart = bitmap.getExtraData() + currentY * bitmapRectToFrameBuffer.width;
            do
            {
                const unsigned length = MIN(available, count);
                const uint8_t* const p_chunkend = p + length * 4;
                count -= length;
                do
                {
                    const uint8_t srcAlpha = *bitmapRGB565AlphaPointer++;
                    const uint8_t alphaFg = LCD::div255((*covers++) * LCD::div255(srcAlpha * widgetAlpha));
                    if (alphaFg)
                    {
                        const uint8_t alphaBg = p[3];
                        const uint8_t alphaMult = LCD::div255(alphaFg * alphaBg);
                        const uint8_t alphaOut = alphaFg + alphaBg - alphaMult;

                        const uint8_t blueBg = *p;
                        const uint8_t blueFg = Color::getBlueFromRGB565(*bitmapRGB565Pointer);
                        *p++ = (blueFg * alphaFg + blueBg * alphaBg - blueBg * alphaMult) / alphaOut;
                        const uint8_t greenBg = *p;
                        const uint8_t greenFg = Color::getGreenFromRGB565(*bitmapRGB565Pointer);
                        *p++ = (greenFg * alphaFg + greenBg * alphaBg - greenBg * alphaMult) / alphaOut;
                        const uint8_t redBg = *p;
                        const uint8_t redFg = Color::getRedFromRGB565(*bitmapRGB565Pointer);
                        *p++ = (redFg * alphaFg + redBg * alphaBg - redBg * alphaMult) / alphaOut;
                        *p++ = alphaOut;
                    }
                    else
                    {
                        p += 4;
                    }
                    bitmapRGB565Pointer++;
                } while (p < p_chunkend);
                bitmapRGB565Pointer = rgb565_linestart;
                bitmapRGB565AlphaPointer = alpha_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
    }
}

bool PainterARGB8888Bitmap::renderInit()
{
    bitmapARGB8888Pointer = 0;
    bitmapRGB888Pointer = 0;
    bitmapRGB565Pointer = 0;
    bitmapRGB565AlphaPointer = 0;

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
        bitmapRGB565AlphaPointer = bitmap.getExtraData();
        if (bitmapRGB565AlphaPointer)
        {
            bitmapRGB565AlphaPointer += currentX + currentY * bitmapRectToFrameBuffer.width;
        }
        return true;
    }

    return false;
}

} // namespace touchgfx
