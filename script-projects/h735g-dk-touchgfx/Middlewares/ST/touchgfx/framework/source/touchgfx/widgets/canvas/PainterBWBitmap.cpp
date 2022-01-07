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
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>
#include <touchgfx/widgets/canvas/PainterBWBitmap.hpp>

namespace touchgfx
{
void PainterBWBitmap::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    assert((bitmap.getId() == BITMAP_INVALID || bitmap.getFormat() == Bitmap::BW || bitmap.getFormat() == Bitmap::BW_RLE) && "The chosen painter only works with BW and BW_RLE bitmaps");
    bitmapRectToFrameBuffer = bitmap.getRect();
    DisplayTransformation::transformDisplayToFrameBuffer(bitmapRectToFrameBuffer);
}

void PainterBWBitmap::setOffset(int16_t x, int16_t y)
{
    xOffset = x;
    yOffset = y;
}

void PainterBWBitmap::setTiled(bool tiled)
{
    isTiled = tiled;
}

// Found in LCD1bpp
void fillBits(uint8_t* fb, uint16_t startX, uint16_t startY, uint16_t stride, uint32_t count, uint8_t color);

void PainterBWBitmap::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* /*covers*/)
{
    currentX = x + areaOffsetX + xOffset;
    currentY = y + areaOffsetY + yOffset;

    x += xAdjust;

    if (!isTiled && currentX < 0)
    {
        if (count < (unsigned int)-currentX)
        {
            return;
        }
        count += currentX;
        //covers -= currentX;
        x -= currentX;
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

    // Max number of pixels before we reach end of bitmap row
    unsigned int available = bitmapRectToFrameBuffer.width - currentX;
    if (bitmap.getFormat() == Bitmap::BW_RLE)
    {
        const uint32_t skip = currentY * bitmapRectToFrameBuffer.width;
        do
        {
            unsigned length = MIN(available, count);
            count -= length;
            do
            {
                uint32_t bw_length = bw_rle.getLength();
                uint32_t bitsToDraw = MIN(bw_length, length);

                fillBits(ptr, x, 0, 0 /* not used */, bitsToDraw, bw_rle.getColor());
                x += bitsToDraw;
                length -= bitsToDraw;
                bw_rle.skipNext(bitsToDraw);
            } while (length);
            bw_rle.init(bitmapBWPointer);
            bw_rle.skipNext(skip);
            available = bitmapRectToFrameBuffer.width;
        } while (count);
    }
    else
    {
        do
        {
            const unsigned length = MIN(available, count);

            unsigned char* p = ptr + (x / 8);
            const uint8_t* src = bitmapBWPointer + currentX / 8;
            uint8_t* RESTRICT dst = p;
            uint16_t srcBitX = currentX % 8; // & 7
            uint16_t dstBitX = x % 8;        // & 7

            uint16_t remainingBits = length;

            if (dstBitX > 0)
            {
                // Start by getting (dst-)aligned for faster transfer
                uint16_t neededBits = 8 - dstBitX;
                if (neededBits > remainingBits)
                {
                    neededBits = remainingBits; // Very narrow src inside same word
                }
                const uint16_t availableBits = 8 - srcBitX;
                uint8_t mask = (1u << neededBits) - 1u;
                const uint8_t dstShift = static_cast<uint8_t>(8u - (dstBitX + neededBits));
                mask <<= dstShift;

                uint8_t word = *src;

                if (availableBits > neededBits)
                {
                    word >>= availableBits - neededBits;
                }
                else if (availableBits < neededBits)
                {
                    // Get the last required bits from src[1]
                    word <<= neededBits - availableBits;
                    word |= src[1] >> (8u - (neededBits - availableBits));
                }

                word <<= dstShift;
                *dst = (*dst & ~mask) | (word & mask);

                srcBitX = (srcBitX + neededBits) % 8; // & 7

                if (availableBits <= neededBits)
                {
                    src++;
                }
                dst++;
                remainingBits -= neededBits;
            }

            // dstX is now word aligned (or we have transferred everything of a narrow image and remainingBits==0)
            if (remainingBits >= 8)
            {
                uint16_t bytesPerLine = remainingBits / 8;
                if (srcBitX == 0)
                {
                    HAL::getInstance()->blockCopy(dst, src, bytesPerLine);
                    src += bytesPerLine;
                    dst += bytesPerLine;
                }
                else
                {
                    uint16_t _remainingBits = remainingBits;

                    remainingBits = _remainingBits;
                    while (remainingBits >= 8)
                    {
                        uint8_t word = *src++;
                        word <<= srcBitX;
                        word |= (*src) >> (8 - srcBitX);
                        *dst++ = word;
                        remainingBits -= 8;
                    }
                }
                remainingBits %= 8; // &= 7
            }

            // Take the last bits, again we need to mask dst
            if (remainingBits > 0)
            {
                uint8_t word = *src;
                if (srcBitX != 0)
                {
                    word <<= srcBitX;
                    word |= src[1] >> (8u - srcBitX);
                }
                const uint8_t mask = ((1u << remainingBits) - 1u) << (8u - remainingBits);
                *dst = (*dst & ~mask) | (word & mask);
            }

            x += length;
            count -= length;
            currentX = 0;
            available = bitmapRectToFrameBuffer.width;
        } while (count);
    }
}

bool PainterBWBitmap::renderInit()
{
    bitmapBWPointer = 0;

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

    // Common for BW and BW_RLE
    bitmapBWPointer = (const uint8_t*)bitmap.getData();
    if (!bitmapBWPointer)
    {
        return false;
    }

    if (bitmap.getFormat() == Bitmap::BW_RLE)
    {
        bw_rle.init(bitmapBWPointer);
        uint32_t skip = (int32_t)currentY * (int32_t)bitmapRectToFrameBuffer.width + (int32_t)currentX;
        bw_rle.skipNext(skip);
        return true;
    }

    if (bitmap.getFormat() == Bitmap::BW)
    {
        bitmapBWPointer += currentY * ((bitmapRectToFrameBuffer.width + 7) / 8);
        return true;
    }

    return false;
}

} // namespace touchgfx
