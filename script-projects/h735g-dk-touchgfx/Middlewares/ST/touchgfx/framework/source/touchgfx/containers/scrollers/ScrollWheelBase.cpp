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
#include <touchgfx/Callback.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/Utils.hpp>
#include <touchgfx/containers/scrollers/ScrollBase.hpp>
#include <touchgfx/containers/scrollers/ScrollWheelBase.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/GestureEvent.hpp>

namespace touchgfx
{
ScrollWheelBase::ScrollWheelBase()
    : ScrollBase(),
      animateToCallback(0)
{
    ScrollWheelBase::setHorizontal(false);
    setTouchable(true);
}

void ScrollWheelBase::setSelectedItemOffset(int16_t offset)
{
    int32_t currentOffset = getOffset();
    distanceBeforeAlignedItem = offset;
    setOffset(currentOffset);
}

int16_t ScrollWheelBase::getSelectedItemOffset() const
{
    return distanceBeforeAlignedItem;
}

int32_t ScrollWheelBase::getPositionForItem(int16_t itemIndex)
{
    int32_t newOffset = -itemIndex * itemSize;
    if (getCircular())
    {
        // Check if it is closer to scroll backwards
        int32_t otherOffset = newOffset + getNumberOfItems() * itemSize;
        int32_t offset = getOffset();
        if (abs(otherOffset - offset) < abs(newOffset - offset))
        {
            newOffset = otherOffset;
        }
    }
    return newOffset;
}

void ScrollWheelBase::animateToPosition(int32_t position, int16_t steps)
{
    if (itemSize == 0)
    {
        return;
    }
    if (animateToCallback && animateToCallback->isValid() && itemSize > 0)
    {
        position = getNearestAlignedOffset(position);
        int16_t itemIndex = (-position) / itemSize;
        animateToCallback->execute(itemIndex);
    }
    ScrollBase::animateToPosition(position, steps);
}

int ScrollWheelBase::getSelectedItem() const
{
    if (itemSize == 0)
    {
        return 0;
    }
    if (currentAnimationState == ANIMATING_GESTURE)
    {
        // Scroll in progress, get the destination value
        return (-getNormalizedOffset(gestureEnd)) / itemSize;
    }
    return (-getNormalizedOffset(getOffset())) / itemSize;
}

int32_t ScrollWheelBase::keepOffsetInsideLimits(int32_t newOffset, int16_t overShoot) const
{
    if (!getCircular())
    {
        newOffset = MIN(newOffset, overShoot);
        int16_t numberOfItems = getNumberOfItems();
        newOffset = MAX(newOffset, -(itemSize * (numberOfItems - 1)) - overShoot);
    }
    return newOffset;
}

void ScrollWheelBase::handleClickEvent(const ClickEvent& event)
{
    if (itemSize == 0)
    {
        return;
    }
    int32_t offset = getOffset();
    if (event.getType() == ClickEvent::PRESSED)
    {
        xClick = event.getX();
        yClick = event.getY();
        initialSwipeOffset = offset;

        if (itemPressedCallback && itemPressedCallback->isValid())
        {
            itemPressedCallback->execute(getSelectedItem());
        }
    }
    else if (event.getType() == ClickEvent::RELEASED)
    {
        if (currentAnimationState == NO_ANIMATION)
        {
            int16_t click = getHorizontal() ? xClick : yClick;
            // Click => move to clicked position
            if (click < distanceBeforeAlignedItem)
            {
                animateToPosition(offset + ((distanceBeforeAlignedItem - click) / itemSize + 1) * itemSize);
            }
            else if (click > distanceBeforeAlignedItem + itemSize)
            {
                animateToPosition(offset - ((click - distanceBeforeAlignedItem) / itemSize) * itemSize);
            }
            else
            {
                animateToPosition(offset);
            }
        }
        else if (currentAnimationState == ANIMATING_DRAG)
        {
            // click + drag + release. Find best Y to scroll to
            animateToPosition(offset);
        }

        if (itemSelectedCallback && itemSelectedCallback->isValid())
        {
            itemSelectedCallback->execute(getSelectedItem());
        }
    }
}

void ScrollWheelBase::handleDragEvent(const DragEvent& event)
{
    currentAnimationState = ANIMATING_DRAG;
    int newOffset = getOffset() + (getHorizontal() ? event.getDeltaX() : event.getDeltaY()) * dragAcceleration / 10;
    if (!getCircular())
    {
        newOffset = MIN(newOffset, itemSize * 3 / 4);
        int16_t numberOfItems = getNumberOfItems();
        newOffset = MAX(newOffset, -(itemSize * (numberOfItems - 1)) - itemSize * 3 / 4);
    }
    setOffset(newOffset);
}

void ScrollWheelBase::handleGestureEvent(const GestureEvent& event)
{
    if (event.getType() == (getHorizontal() ? GestureEvent::SWIPE_HORIZONTAL : GestureEvent::SWIPE_VERTICAL))
    {
        int32_t newOffset = getOffset() + event.getVelocity() * swipeAcceleration / 10;
        if (maxSwipeItems > 0)
        {
            int32_t maxDistance = maxSwipeItems * itemSize;
            newOffset = MIN(newOffset, initialSwipeOffset + maxDistance);
            newOffset = MAX(newOffset, initialSwipeOffset - maxDistance);
        }
        animateToPosition(newOffset);
    }
}

void ScrollWheelBase::setAnimateToCallback(GenericCallback<int16_t>& callback)
{
    animateToCallback = &callback;
}
} // namespace touchgfx
