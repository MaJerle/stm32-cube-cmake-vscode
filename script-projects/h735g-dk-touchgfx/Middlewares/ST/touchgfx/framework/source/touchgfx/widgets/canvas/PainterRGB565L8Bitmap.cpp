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
#include <touchgfx/widgets/canvas/AbstractPainterRGB565.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565L8Bitmap.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>

namespace touchgfx
{
void PainterRGB565L8Bitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    assert((bitmap.getId() == BITMAP_INVALID || bitmap.getFormat() == Bitmap::L8) && "The chosen painter only works with appropriate L8 bitmaps");
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterRGB565L8Bitmap::setOffset(int16_t x, int16_t y)
{
    xOffset = x;
    yOffset = y;
}

void PainterRGB565L8Bitmap::setTiled(bool tiled)
{
    isTiled = tiled;
}

void PainterRGB565L8Bitmap::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
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
    const uint8_t* const l8_linestart = bitmap.getData() + (currentY * bitmapRectToFrameBuffer.width);
    // Max number of pixels before we reach end of bitmap row
    unsigned int available = bitmapRectToFrameBuffer.width - currentX;
    if (l8format == Bitmap::CLUT_FORMAT_L8_RGB565)
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
                        *p = ((const uint16_t*)bitmapExtraPointer)[*bitmapPointer];
                    }
                    else
                    {
                        // Non-Transparent pixel
                        *p = mixColors(((const uint16_t*)bitmapExtraPointer)[*bitmapPointer], *p, alpha);
                    }
                    p++;
                    bitmapPointer++;
                } while (p < p_chunkend);
                bitmapPointer = l8_linestart;
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
                    *p = mixColors(((const uint16_t*)bitmapExtraPointer)[*bitmapPointer], *p, alpha);
                    p++;
                    bitmapPointer++;
                } while (p < p_chunkend);
                bitmapPointer = l8_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
    }
    else if (l8format == Bitmap::CLUT_FORMAT_L8_RGB888)
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
                    const uint8_t* src = &bitmapExtraPointer[*bitmapPointer++ * 3];
                    // Use alpha from covers directly
                    const uint8_t alpha = *covers++;
                    const uint8_t blue = *src++;
                    const uint8_t green = *src++;
                    const uint8_t red = *src;
                    if (alpha == 0xFF)
                    {
                        // Solid pixel
                        *p++ = ((red << 8) & RMASK) | ((green << 3) & GMASK) | ((blue >> 3) & BMASK);
                    }
                    else
                    {
                        const uint8_t ialpha = 0xFF - alpha;
                        const uint16_t bufpix = *p;
                        uint8_t fbr = Color::getRedFromRGB565(bufpix);
                        uint8_t fbg = Color::getGreenFromRGB565(bufpix);
                        uint8_t fbb = Color::getBlueFromRGB565(bufpix);
                        *p++ = ((LCD::div255(red * alpha + fbr * ialpha) << 8) & RMASK) | ((LCD::div255(green * alpha + fbg * ialpha) << 3) & GMASK) | ((LCD::div255(blue * alpha + fbb * ialpha) >> 3) & BMASK);
                    }
                } while (p < p_chunkend);
                bitmapPointer = l8_linestart;
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
                    const uint8_t* src = &bitmapExtraPointer[*bitmapPointer++ * 3];
                    const uint8_t blue = *src++;
                    const uint8_t green = *src++;
                    const uint8_t red = *src;
                    const uint8_t alpha = LCD::div255((*covers++) * widgetAlpha);
                    const uint8_t ialpha = 0xFF - alpha;
                    const uint16_t bufpix = *p;
                    const uint8_t fbr = Color::getRedFromRGB565(bufpix);
                    const uint8_t fbg = Color::getGreenFromRGB565(bufpix);
                    const uint8_t fbb = Color::getBlueFromRGB565(bufpix);
                    *p++ = ((LCD::div255(red * alpha + fbr * ialpha) << 8) & RMASK) | ((LCD::div255(green * alpha + fbg * ialpha) << 3) & GMASK) | ((LCD::div255(blue * alpha + fbb * ialpha) >> 3) & BMASK);
                } while (p < p_chunkend);
                bitmapPointer = l8_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
    }
    else // Bitmap::CLUT_FORMAT_L8_ARGB8888
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
                    const uint32_t newpix = ((const uint32_t*)bitmapExtraPointer)[*bitmapPointer];
                    const uint8_t srcAlpha = newpix >> 24;
                    const uint8_t alpha = LCD::div255((*covers++) * srcAlpha);
                    if (alpha == 0xFF)
                    {
                        *p = LCD16bpp::getNativeColor(newpix);
                    }
                    else if (alpha)
                    {
                        *p = mixColors((newpix >> 8) & RMASK, (newpix >> 5) & GMASK, (newpix >> 3) & BMASK, *p, alpha);
                    }
                    p++;
                    bitmapPointer++;
                } while (p < p_chunkend);
                bitmapPointer = l8_linestart;
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
                    const uint32_t newpix = ((const uint32_t*)bitmapExtraPointer)[*bitmapPointer];
                    const uint8_t srcAlpha = newpix >> 24;
                    const uint8_t alpha = LCD::div255((*covers++) * LCD::div255(srcAlpha * widgetAlpha));
                    if (alpha)
                    {
                        *p = mixColors((newpix >> 8) & RMASK, (newpix >> 5) & GMASK, (newpix >> 3) & BMASK, *p, alpha);
                    }
                    p++;
                    bitmapPointer++;
                } while (p < p_chunkend);
                bitmapPointer = l8_linestart;
                available = bitmapRectToFrameBuffer.width;
            } while (p < p_lineend);
        }
    }
}

bool PainterRGB565L8Bitmap::renderInit()
{
    bitmapPointer = 0;
    bitmapExtraPointer = 0;

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

    if (bitmap.getFormat() == Bitmap::L8)
    {
        bitmapPointer = bitmap.getData();
        if (!bitmapPointer)
        {
            return false;
        }
        bitmapPointer += currentX + currentY * bitmapRectToFrameBuffer.width;
        bitmapExtraPointer = bitmap.getExtraData();
        assert(bitmapExtraPointer);
        l8format = (Bitmap::ClutFormat)(*(const uint16_t*)bitmapExtraPointer);
        assert(l8format == Bitmap::CLUT_FORMAT_L8_RGB565 || l8format == Bitmap::CLUT_FORMAT_L8_ARGB8888 || l8format == Bitmap::CLUT_FORMAT_L8_RGB888);
        bitmapExtraPointer += 4; // Skip header
        return true;
    }

    return false;
}
} // namespace touchgfx
