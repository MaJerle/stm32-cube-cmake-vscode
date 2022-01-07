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
#include <touchgfx/Utils.hpp>
#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>

namespace touchgfx
{
BoxProgress::BoxProgress()
    : AbstractDirectionProgress(), box()
{
    progressIndicatorContainer.add(box);
}

void BoxProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    box.setPosition(0, 0, width, height);

    AbstractDirectionProgress::setProgressIndicatorPosition(x, y, width, height);
}

void BoxProgress::setColor(colortype color)
{
    box.setColor(color);
}

colortype BoxProgress::getColor() const
{
    return box.getColor();
}

void BoxProgress::setAlpha(uint8_t newAlpha)
{
    AbstractDirectionProgress::setAlpha(newAlpha);
    box.setAlpha(newAlpha);
}

void BoxProgress::setValue(int value)
{
    AbstractDirectionProgress::setValue(value);
    int16_t progress = 0;
    switch (progressDirection)
    {
    case RIGHT:
    case LEFT:
        progress = AbstractDirectionProgress::getProgress(progressIndicatorContainer.getWidth());
        break;
    case DOWN:
    case UP:
        progress = AbstractDirectionProgress::getProgress(progressIndicatorContainer.getHeight());
        break;
    }
    switch (progressDirection)
    {
    case RIGHT:
        {
            int16_t oldWidth = box.getWidth();
            box.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
            int16_t newWidth = box.getWidth();
            Rect rect(MIN(oldWidth, newWidth), 0, abs(oldWidth - newWidth), box.getHeight());
            progressIndicatorContainer.invalidateRect(rect);
            break;
        }
    case LEFT:
        {
            int16_t oldX = box.getX();
            box.setPosition(getWidth() - progress, 0, progress, progressIndicatorContainer.getHeight());
            int16_t newX = box.getX();
            Rect rect(MIN(oldX, newX), 0, abs(oldX - newX), box.getHeight());
            progressIndicatorContainer.invalidateRect(rect);
            break;
        }
    case DOWN:
        {
            int16_t oldHeight = box.getHeight();
            box.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            int16_t newHeight = box.getHeight();
            Rect rect(0, MIN(oldHeight, newHeight), box.getWidth(), abs(oldHeight - newHeight));
            progressIndicatorContainer.invalidateRect(rect);
            break;
        }
    case UP:
        {
            int16_t oldY = box.getY();
            box.setPosition(0, progressIndicatorContainer.getHeight() - progress, progressIndicatorContainer.getWidth(), progress);
            int16_t newY = box.getY();
            Rect rect(0, MIN(oldY, newY), box.getWidth(), abs(oldY - newY));
            progressIndicatorContainer.invalidateRect(rect);
            break;
        }
    }
}
} // namespace touchgfx
