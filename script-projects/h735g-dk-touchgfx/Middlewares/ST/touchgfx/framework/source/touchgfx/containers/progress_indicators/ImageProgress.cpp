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
#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>
#include <touchgfx/containers/progress_indicators/ImageProgress.hpp>

namespace touchgfx
{
ImageProgress::ImageProgress()
    : AbstractDirectionProgress(), container(), image(), fixedPosition(true)
{
    container.add(image);
    progressIndicatorContainer.add(container);
}

void ImageProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    container.setPosition(0, 0, width, height);

    AbstractProgressIndicator::setProgressIndicatorPosition(x, y, width, height);
}

void ImageProgress::setAnchorAtZero(bool anchorAtZero)
{
    fixedPosition = anchorAtZero;
    setValue(getValue());
}

bool ImageProgress::getAnchorAtZero() const
{
    return fixedPosition;
}

void ImageProgress::setBitmap(BitmapId bitmapId)
{
    image.setBitmap(Bitmap(bitmapId));
}

BitmapId ImageProgress::getBitmap() const
{
    return image.getBitmapId();
}

void ImageProgress::setAlpha(uint8_t newAlpha)
{
    AbstractDirectionProgress::setAlpha(newAlpha);
    image.setAlpha(newAlpha);
}

void ImageProgress::setValue(int value)
{
    AbstractDirectionProgress::setValue(value);
    const uint16_t maxProgress = (progressDirection == RIGHT || progressDirection == LEFT) ? progressIndicatorContainer.getWidth() : progressIndicatorContainer.getHeight();
    int16_t progress = AbstractDirectionProgress::getProgress(maxProgress);
    if (fixedPosition)
    {
        switch (progressDirection)
        {
        case RIGHT:
            {
                int16_t oldWidth = container.getWidth();
                container.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
                image.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
                int16_t newWidth = container.getWidth();
                Rect rect(MIN(oldWidth, newWidth), 0, abs(oldWidth - newWidth), container.getHeight());
                progressIndicatorContainer.invalidateRect(rect);
                break;
            }
        case LEFT:
            {
                int16_t oldX = container.getX();
                container.setPosition(getWidth() - progress, 0, progress, progressIndicatorContainer.getHeight());
                image.setPosition(-container.getX(), 0, progressIndicatorContainer.getWidth(), progressIndicatorContainer.getHeight());
                int16_t newX = container.getX();
                Rect rect(MIN(oldX, newX), 0, abs(oldX - newX), container.getHeight());
                progressIndicatorContainer.invalidateRect(rect);
                break;
            }
        case DOWN:
            {
                int16_t oldHeight = container.getHeight();
                container.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
                image.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
                int16_t newHeight = container.getHeight();
                Rect rect(0, MIN(oldHeight, newHeight), container.getWidth(), abs(oldHeight - newHeight));
                progressIndicatorContainer.invalidateRect(rect);
                break;
            }
        case UP:
            {
                int16_t oldY = container.getY();
                container.setPosition(0, progressIndicatorContainer.getHeight() - progress, progressIndicatorContainer.getWidth(), progress);
                image.setPosition(0, -container.getY(), progressIndicatorContainer.getWidth(), progressIndicatorContainer.getHeight());
                int16_t newY = container.getY();
                Rect rect(0, MIN(oldY, newY), container.getWidth(), abs(oldY - newY));
                progressIndicatorContainer.invalidateRect(rect);
                break;
            }
        }
    }
    else
    {
        container.invalidate();
        switch (progressDirection)
        {
        case RIGHT:
            container.setPosition(0, 0, progress, getHeight());
            image.setPosition(progress - progressIndicatorContainer.getWidth(), 0, progressIndicatorContainer.getWidth(), getHeight());
            break;
        case LEFT:
            container.setPosition(progressIndicatorContainer.getWidth() - progress, 0, progress, progressIndicatorContainer.getHeight());
            image.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
            break;
        case DOWN:
            container.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, progress - progressIndicatorContainer.getHeight(), progressIndicatorContainer.getWidth(), progressIndicatorContainer.getHeight());
            break;
        case UP:
            container.setPosition(0, progressIndicatorContainer.getHeight() - progress, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            break;
        }
        container.invalidate();
    }
}
} // namespace touchgfx
