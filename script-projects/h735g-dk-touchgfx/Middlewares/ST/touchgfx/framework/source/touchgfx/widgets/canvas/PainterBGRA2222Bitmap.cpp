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
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>
#include <touchgfx/widgets/canvas/PainterBGRA2222Bitmap.hpp>
#include <platform/driver/lcd/LCD8bpp_BGRA2222.hpp>

namespace touchgfx
{
void PainterBGRA2222Bitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    assert((bitmap.getId() == BITMAP_INVALID || bitmap.getFormat() == Bitmap::BGRA2222) && "The chosen painter only works with BGRA2222 bitmaps");
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterBGRA2222Bitmap::setOffset(int16_t x, int16_t y)
{
    xOffset = x;
    yOffset = y;
}

void PainterBGRA2222Bitmap::setTiled(bool tiled)
{
    isTiled = tiled;
}

void PainterBGRA2222Bitmap::render(uint8_t* ptr,
                                   int x,
                                   int xAdjust,
                                   int y,
                                   unsigned count,
                                   const uint8_t* covers)
{
    uint8_t* p = ptr + (x + xAdjust);

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

    const uint8_t* const p_lineend = p + count;
    // Max number of pixels before we reach end of bitmap row
    unsigned int available = bitmapRectToFrameBuffer.width - currentX;
    const uint8_t* const bgra2222_linestart = bitmap.getData() + (currentY * bitmapRectToFrameBuffer.width);
    if (widgetAlpha == 0xFF)
    {
        do
        {
            const unsigned length = MIN(available, count);
            const uint8_t* const p_chunkend = p + length;
            count -= length;
            do
            {
                const uint8_t srcAlpha = ((*bitmapBGRA2222Pointer) & 0x03) * 0x55;
                const uint8_t alpha = LCD::div255((*covers++) * srcAlpha);
                if (alpha == 0xFF)
                {
                    // Solid pixel
                    *p = *bitmapBGRA2222Pointer;
                }
                else if (alpha)
                {
                    // Non-Transparent pixel
                    *p = mixColors(*bitmapBGRA2222Pointer, *p, alpha);
                }
                p++;
                bitmapBGRA2222Pointer++;
            } while (p < p_chunkend);
            bitmapBGRA2222Pointer = bgra2222_linestart;
            available = bitmapRectToFrameBuffer.width;
        } while (p < p_lineend);
    }
    else
    {
        do
        {
            const unsigned length = MIN(available, count);
            const uint8_t* const p_chunkend = p + length;
            count -= length;
            do
            {
                const uint8_t srcAlpha = ((*bitmapBGRA2222Pointer) & 0x03) * 0x55;
                const uint8_t alpha = LCD::div255((*covers++) * LCD::div255(srcAlpha * widgetAlpha));
                if (alpha) // This can never get to max=0xFF*0xFF as widgetAlpha<255
                {
                    // Non-Transparent pixel
                    *p = mixColors(*bitmapBGRA2222Pointer, *p, alpha);
                }
                p++;
                bitmapBGRA2222Pointer++;
            } while (p < p_chunkend);
            bitmapBGRA2222Pointer = bgra2222_linestart;
            available = bitmapRectToFrameBuffer.width;
        } while (p < p_lineend);
    }
}

bool PainterBGRA2222Bitmap::renderInit()
{
    bitmapBGRA2222Pointer = 0;

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

    if (bitmap.getFormat() == Bitmap::BGRA2222)
    {
        bitmapBGRA2222Pointer = bitmap.getData();
        if (!bitmapBGRA2222Pointer)
        {
            return false;
        }
        bitmapBGRA2222Pointer += currentX + currentY * bitmapRectToFrameBuffer.width;
        return true;
    }

    return false;
}

} // namespace touchgfx
