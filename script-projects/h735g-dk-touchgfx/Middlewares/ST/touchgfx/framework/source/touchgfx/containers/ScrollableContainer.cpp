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
#include <touchgfx/Application.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/Utils.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/GestureEvent.hpp>

namespace touchgfx
{
ScrollableContainer::ScrollableContainer()
    : Container(),
      scrollbarPadding(0),
      scrollbarWidth(2),
      scrollbarAlpha(120),
      scrollbarColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF)),
      maxVelocity(SCROLLBAR_MAX_VELOCITY),
      accelDirection(GestureEvent::SWIPE_HORIZONTAL),
      xSlider(0, 0, scrollbarColor, scrollbarAlpha),
      ySlider(0, 0, scrollbarColor, scrollbarAlpha),
      pressedDrawable(0),
      lastDraggableChild(0),
      scrolledXDistance(0),
      scrolledYDistance(0),
      scrollThreshold(5),
      pressedX(0),
      pressedY(0),
      isPressed(false),
      isScrolling(false),
      scrollableX(true),
      scrollableY(true),
      scrollbarsVisible(true),
      scrollbarsPermanentlyVisible(false),
      scrollDuration(0),
      beginningValue(0),
      targetValue(0),
      animationCounter(0),
      animate(false),
      fingerAdjustmentX(0),
      fingerAdjustmentY(0),
      hasIssuedCancelEvent(false),
      scrollDurationSpeedup(7),
      scrollDurationSlowdown(1)
{
    xSlider.setVisible(false);
    ySlider.setVisible(false);
    setTouchable(true);
}

void ScrollableContainer::handleClickEvent(const ClickEvent& event)
{
    if (event.getType() == ClickEvent::PRESSED)
    {
        isPressed = true;

        if (animate)
        {
            // Stop scroll animation
            animate = false;
            animationCounter = 0;
            Application::getInstance()->unregisterTimerWidget(this);
        }

        getLastChildNear(event.getX(), event.getY(), &pressedDrawable, &fingerAdjustmentX, &fingerAdjustmentY);

        if (pressedDrawable == this)
        {
            pressedDrawable = 0;
        }

        if (pressedDrawable)
        {
            hasIssuedCancelEvent = false;
            pressedX = event.getX();
            pressedY = event.getY();
            Rect r = pressedDrawable->getAbsoluteRect();
            Rect me = getAbsoluteRect();
            ClickEvent relative(event.getType(), event.getX() + fingerAdjustmentX - (r.x - me.x), event.getY() + fingerAdjustmentY - (r.y - me.y));
            pressedDrawable->handleClickEvent(relative);
            lastDraggableChild = pressedDrawable;
        }
    }
    else if (event.getType() == ClickEvent::CANCEL)
    {
        return;
    }
    else // if (event.getType() == ClickEvent::RELEASED)
    {
        if (pressedDrawable)
        {
            Rect r = pressedDrawable->getAbsoluteRect();
            Rect me = getAbsoluteRect();
            ClickEvent relative(event.getType(), event.getX() + fingerAdjustmentX - (r.x - me.x), event.getY() + fingerAdjustmentY - (r.y - me.y));
            pressedDrawable->handleClickEvent(relative);
        }

        pressedDrawable = 0;
        lastDraggableChild = 0;
        isPressed = false;
    }

    isScrolling = false;

    // Redraw scrollbars.
    xSlider.setVisible((isPressed && scrollbarsVisible) || scrollbarsPermanentlyVisible);
    ySlider.setVisible((isPressed && scrollbarsVisible) || scrollbarsPermanentlyVisible);
    invalidateScrollbars();
}

void ScrollableContainer::handleDragEvent(const DragEvent& event)
{
    DragEvent actualDrag = event;
    bool acceptInitialScroll = false;

    bool canScrollX = false;
    bool canScrollY = false;
    isScrollableXY(canScrollX, canScrollY);

    if ((pressedDrawable != 0) && (pressedDrawable != this))
    {
        // Also send this drag event to the appropriate child widget
        Rect r = pressedDrawable->getAbsoluteRect();
        Rect me = getAbsoluteRect();
        int16_t oldX = event.getOldX() + fingerAdjustmentX - (r.x - me.x);
        int16_t oldY = event.getOldY() + fingerAdjustmentY - (r.y - me.y);
        int16_t newX = canScrollX ? oldX : event.getNewX() + fingerAdjustmentX - (r.x - me.x);
        int16_t newY = canScrollY ? oldY : event.getNewY() + fingerAdjustmentY - (r.y - me.y);

        // but only in the direction(s) where the scrollable container itself
        // cannot scroll.
        if ((!canScrollX && newX != oldX) || (!canScrollY && newY != oldY))
        {
            DragEvent relative(DragEvent::DRAGGED, oldX, oldY, newX, newY);
            pressedDrawable->handleDragEvent(relative);
        }
    }

    // If we are not currently scrolling, the drag event delta must
    // be larger than the threshold value, otherwise the event is ignored.
    if (!isScrolling)
    {
        // Only consider the delta in directions that are actually scrollable.
        // Note: Do not use the delta from received event since that only reflects
        // change since last drag. What we want to check here is if the total
        // delta from the point of click has now exceeded the threshold.
        actualDrag = DragEvent(DragEvent::DRAGGED, pressedX + fingerAdjustmentX, pressedY + fingerAdjustmentY, event.getNewX() + fingerAdjustmentX, event.getNewY() + fingerAdjustmentY);
        if (canScrollX)
        {
            // Can scroll in X.
            if (abs(actualDrag.getDeltaX()) > scrollThreshold)
            {
                acceptInitialScroll = true;
            }
        }

        if (canScrollY)
        {
            // Can scroll in Y.
            if (abs(actualDrag.getDeltaY()) > scrollThreshold)
            {
                acceptInitialScroll = true;
            }
        }

        if (acceptInitialScroll)
        {
            isScrolling = true;
        }
        else
        {
            // Discard this drag event. However, if the new coordinates no longer matches the drawable which received the PRESSED click event
            // issue a CANCEL event to that drawable.
            if (pressedDrawable && !hasIssuedCancelEvent)
            {
                Drawable* child = 0;
                Container::getLastChild(event.getNewX() + fingerAdjustmentX, event.getNewY() + fingerAdjustmentY, &child);
                if (pressedDrawable != child)
                {
                    ClickEvent ce(ClickEvent::CANCEL, 0, 0);
                    pressedDrawable->handleClickEvent(ce);
                    hasIssuedCancelEvent = true;
                }
            }
            return;
        }
    }

    // Send cancel events to child in focus
    if (pressedDrawable && !hasIssuedCancelEvent)
    {
        ClickEvent ce(ClickEvent::CANCEL, 0, 0);
        pressedDrawable->handleClickEvent(ce);
        hasIssuedCancelEvent = true;
    }

    int16_t deltaX = 0;
    int16_t deltaY = 0;

    if (scrollableX)
    {
        if (acceptInitialScroll)
        {
            // Initial drag which is past threshold, only scroll one pixel in initial event.
            if (actualDrag.getDeltaX() > 0)
            {
                deltaX = 1;
            }
            else if (actualDrag.getDeltaX() < 0)
            {
                deltaX = -1;
            }
        }
        else
        {
            // Scroll entire delta
            deltaX = actualDrag.getDeltaX();
        }
    }
    else
    {
        // Not scrollable
        deltaX = 0;
    }

    if (scrollableY)
    {
        if (acceptInitialScroll)
        {
            // Initial drag which is past threshold, only scroll one pixel in initial event.
            if (actualDrag.getDeltaY() > 0)
            {
                deltaY = 1;
            }
            else if (actualDrag.getDeltaY() < 0)
            {
                deltaY = -1;
            }
        }
        else
        {
            // Scroll entire delta
            deltaY = actualDrag.getDeltaY();
        }
    }
    else
    {
        // Not scrollable
        deltaY = 0;
    }

    doScroll(deltaX, deltaY);
}

void ScrollableContainer::handleGestureEvent(const GestureEvent& event)
{
    bool canScrollX = false;
    bool canScrollY = false;
    isScrollableXY(canScrollX, canScrollY);

    if ((canScrollX && (event.getType() == GestureEvent::SWIPE_HORIZONTAL)) ||
        (canScrollY && (event.getType() == GestureEvent::SWIPE_VERTICAL)))
    {
        int16_t velocityAbsolute = abs(event.getVelocity());

        // Ignore gestures with velocity lower than threshold
        if (velocityAbsolute < scrollThreshold)
        {
            return;
        }

        // Force velocity within limits
        velocityAbsolute = MIN(velocityAbsolute, maxVelocity);
        velocityAbsolute = MAX(velocityAbsolute, SCROLLBAR_MIN_VELOCITY);

        // Try to set some reasonable values for how long the resulting scroll should be, and how many ticks is should take
        scrollDuration = velocityAbsolute * scrollDurationSpeedup / scrollDurationSlowdown;
        targetValue = ((event.getVelocity() > 0) ? 1 : -1) * (velocityAbsolute - 4) * 72;
        scrollDuration = MIN(scrollDuration, abs(targetValue));

        // Get ready to animate scroll: Initialize values
        beginningValue = (event.getType() == GestureEvent::SWIPE_VERTICAL) ? getContainedArea().y : getContainedArea().x;
        animate = true;
        Application::getInstance()->registerTimerWidget(this);
        accelDirection = event.getType();

        if (pressedDrawable && !hasIssuedCancelEvent)
        {
            ClickEvent ce(ClickEvent::CANCEL, 0, 0);
            pressedDrawable->handleClickEvent(ce);
            hasIssuedCancelEvent = true;
        }
    }
}

Rect ScrollableContainer::getXScrollbar() const
{
    Rect res(0, 0, 0, 0);
    if (scrollableX)
    {
        Rect contained = getContainedArea();
        const int scrollSpace = (scrollableY && (contained.height > rect.height)) ? (2 * scrollbarPadding + scrollbarWidth + SCROLLBAR_LINE) : 0;

        if (contained.width > rect.width)
        {
            int leftPadding = (-1 * contained.x * rect.width) / contained.width;
            int rightPadding = ((contained.right() - rect.width) * rect.width) / contained.width;
            const int startWidth = rect.width - 2 * scrollbarPadding - 2 * SCROLLBAR_LINE - scrollSpace;
            int width = startWidth;
            width -= (leftPadding + rightPadding);
            if (width < scrollbarWidth * 2)
            {
                // If the contained area is very large, the scrollbar width may become zero or even negative.
                int diff = scrollbarWidth * 2 - width;
                width = scrollbarWidth * 2; // Force scrollbar width to a minimum
                // Distribute the deviation error based on current scrollbar X position (the amount subtracted from scrollbar xpos increases gradually).
                leftPadding -= (diff * leftPadding) / startWidth;
            }
            res = Rect(leftPadding + scrollbarPadding + SCROLLBAR_LINE, rect.height - scrollbarWidth - scrollbarPadding - SCROLLBAR_LINE, width, scrollbarWidth);
        }
    }
    return res;
}

Rect ScrollableContainer::getYScrollbar() const
{
    Rect res(0, 0, 0, 0);
    if (scrollableY)
    {
        Rect contained = getContainedArea();
        const int scrollSpace = (scrollableX && (contained.width > rect.width)) ? (2 * scrollbarPadding + scrollbarWidth + SCROLLBAR_LINE) : 0;

        if (contained.height > rect.height)
        {
            int topPadding = (-1 * contained.y * rect.height) / contained.height;
            int bottomPadding = ((contained.bottom() - rect.height) * rect.height) / contained.height;
            const int startHeight = rect.height - 2 * scrollbarPadding - 2 * SCROLLBAR_LINE - scrollSpace;
            int height = startHeight;
            height -= (topPadding + bottomPadding);
            if (height < scrollbarWidth * 2)
            {
                // If the contained area is very large, the scrollbar height may become zero or even negative.
                int diff = scrollbarWidth * 2 - height;
                height = scrollbarWidth * 2; // Force scrollbar height to a minimum
                // Distribute the deviation error based on current scrollbar Y position (the amount subtracted from scrollbar ypos increases gradually).
                topPadding -= (diff * topPadding) / startHeight;
            }
            res = Rect(rect.width - scrollbarWidth - scrollbarPadding - 2 * SCROLLBAR_LINE, topPadding + scrollbarPadding + SCROLLBAR_LINE, scrollbarWidth, height);
        }
    }
    return res;
}

Rect ScrollableContainer::getXBorder(const Rect& xBar, const Rect& yBar) const
{
    Rect border(0, 0, 0, 0);
    if (!xBar.isEmpty())
    {
        const int scrollSpace = (!yBar.isEmpty()) ? (2 * scrollbarPadding + scrollbarWidth + SCROLLBAR_LINE) : 0;
        border = Rect(scrollbarPadding, xBar.y - SCROLLBAR_LINE, rect.width - 2 * scrollbarPadding - scrollSpace, scrollbarWidth + 2 * SCROLLBAR_LINE);
    }
    return border;
}

Rect ScrollableContainer::getYBorder(const Rect& xBar, const Rect& yBar) const
{
    Rect border(0, 0, 0, 0);
    if (!yBar.isEmpty())
    {
        const int scrollSpace = (!xBar.isEmpty()) ? (2 * scrollbarPadding + scrollbarWidth + SCROLLBAR_LINE) : 0;
        border = Rect(yBar.x - SCROLLBAR_LINE, scrollbarPadding, scrollbarWidth + 2 * SCROLLBAR_LINE, rect.height - 2 * scrollbarPadding - scrollSpace);
    }
    return border;
}

void ScrollableContainer::invalidateScrollbars()
{
    Rect xBar = getXScrollbar();
    Rect yBar = getYScrollbar();

    Rect xBorder = getXBorder(xBar, yBar);
    Rect yBorder = getYBorder(xBar, yBar);

    // The two if statements ensure that the two sliders is invalidates thereby hides them, before they are set to size zero.
    if (xSlider.getY() > xBorder.y)
    {
        xSlider.invalidate();
    }
    if (ySlider.getX() > yBorder.x)
    {
        ySlider.invalidate();
    }

    xSlider.setPosition(xBar.x, xBar.y, xBar.width, xBar.height);
    ySlider.setPosition(yBar.x, yBar.y, yBar.width, yBar.height);

    // x-/yBorder is given the coordinates zero and the witdh of the visiable area for the scrollable container,
    // to ensure that the entire area where for the scrollable bars is and have been is invalidated correct.
    xBorder.x = 0;
    xBorder.width = rect.width;
    yBorder.height = rect.height;
    yBorder.y = 0;

    if (!xBorder.isEmpty())
    {
        invalidateRect(xBorder);
    }

    if (!yBorder.isEmpty())
    {
        invalidateRect(yBorder);
    }
}

bool ScrollableContainer::doScroll(int16_t deltaX, int16_t deltaY)
{
    if (!deltaX && !deltaY)
    {
        return false;
    }
    bool couldScroll = false;
    Rect contained = getContainedArea();
    if (contained.width > rect.width)
    {
        if (deltaX > 0)
        {
            if (contained.x + deltaX > 0)
            {
                deltaX = -contained.x;
            }
        }
        else if (deltaX < 0)
        {
            if (contained.right() + deltaX < rect.width)
            {
                deltaX = rect.width - contained.right();
            }
        }
    }
    else
    {
        deltaX = 0;
    }

    if (contained.height > rect.height)
    {
        if (deltaY > 0)
        {
            if (contained.y + deltaY > 0)
            {
                deltaY = -contained.y;
            }
        }
        else if (deltaY < 0)
        {
            if (contained.bottom() + deltaY < rect.height)
            {
                deltaY = rect.height - contained.bottom();
            }
        }
    }
    else
    {
        deltaY = 0;
    }

    if (deltaX || deltaY)
    {
        moveChildrenRelative(deltaX, deltaY);

        invalidateScrollbars();
        couldScroll = true;
    }
    return couldScroll;
}

void ScrollableContainer::childGeometryChanged()
{
    int deltaX = 0;
    int deltaY = 0;
    Rect contained = getChildrenContainedArea();
    if (contained.y > 0)
    {
        // Make sure we haven't scrolled above the top
        deltaY = contained.y;
    }
    else if (contained.bottom() < rect.height)
    {
        // Make sure we haven't scrolled below the bottom
        deltaY = contained.bottom() - rect.height;
        if (contained.y > deltaY)
        {
            deltaY = contained.y;
        }
    }

    if (contained.x > 0)
    {
        // Make sure we haven't scrolled too far to the left
        deltaX = contained.x;
    }
    else if (contained.right() < rect.width)
    {
        // Make sure we haven't scrolled too far to the right
        deltaX = contained.right() - rect.width;
        if (contained.x > deltaX)
        {
            deltaX = contained.x;
        }
    }

    if (deltaX != 0 || deltaY != 0)
    {
        moveChildrenRelative(-deltaX, -deltaY);
        invalidateScrollbars();
    }
}

void ScrollableContainer::add(Drawable& d)
{
    remove(xSlider);
    remove(ySlider);

    Container::add(d);
    Container::add(xSlider);
    Container::add(ySlider);
}

Rect ScrollableContainer::getContainedArea() const
{
    Rect contained(0, 0, rect.width, rect.height);
    contained.expandToFit(getChildrenContainedArea());
    return contained;
}

Rect ScrollableContainer::getChildrenContainedArea() const
{
    Rect contained(0, 0, 0, 0);
    for (Drawable* d = firstChild; d; d = d->getNextSibling())
    {
        if ((d != &xSlider) && (d != &ySlider) && (d->isVisible()))
        {
            contained.expandToFit(d->getRect());
        }
    }
    return contained;
}

void ScrollableContainer::reset()
{
    moveChildrenRelative(-scrolledXDistance, -scrolledYDistance);
    invalidateScrollbars();
}

void ScrollableContainer::moveChildrenRelative(int16_t deltaX, int16_t deltaY)
{
    for (Drawable* d = firstChild; d; d = d->getNextSibling())
    {
        if ((d != &xSlider) && (d != &ySlider))
        {
            d->moveRelative(deltaX, deltaY);
        }
    }
    scrolledXDistance += deltaX;
    scrolledYDistance += deltaY;
}

void ScrollableContainer::handleTickEvent()
{
    if (!animate)
    {
        return;
    }
    // Calculate new position or stop animation
    animationCounter++;
    if (animationCounter <= scrollDuration)
    {
        // Calculate value in [beginningValue ; (beginningValue+targetValue)]
        int16_t calculatedValue = EasingEquations::cubicEaseOut(animationCounter, beginningValue, targetValue, scrollDuration);

        // Note: Result of "calculatedValue & 1" is compiler dependent for negative values of calculatedValue
        if (calculatedValue % 2)
        {
            // Optimization: calculatedValue is odd, add 1/-1 to move drawables modulo 32 bits in framebuffer
            calculatedValue += (calculatedValue > 0) ? 1 : -1;
        }

        // Convert to delta value relative to current X or Y
        int16_t scrollX = (accelDirection == GestureEvent::SWIPE_VERTICAL) ? 0 : (calculatedValue - getContainedArea().x);
        int16_t scrollY = (accelDirection == GestureEvent::SWIPE_HORIZONTAL) ? 0 : (calculatedValue - getContainedArea().y);

        // Perform the actual animation step, stop animation if
        // scrolling was not possible (doScroll invalidates children)
        animate = doScroll(scrollX, scrollY);
    }
    else
    {
        animate = false;
    }

    if (!animate)
    {
        Application::getInstance()->unregisterTimerWidget(this);
        animationCounter = 0;
    }
}

void ScrollableContainer::setScrollbarsColor(colortype color)
{
    scrollbarColor = color;
    xSlider.setColor(scrollbarColor);
    ySlider.setColor(scrollbarColor);
}

void ScrollableContainer::setScrollbarsAlpha(uint8_t alpha)
{
    scrollbarAlpha = alpha;
    xSlider.setAlpha(scrollbarAlpha);
    ySlider.setAlpha(scrollbarAlpha);
}

void ScrollableContainer::setScrollbarPadding(uint8_t padding)
{
    scrollbarPadding = padding;
}

void ScrollableContainer::setScrollbarWidth(uint8_t width)
{
    scrollbarWidth = width;
}

void ScrollableContainer::setScrollbarsVisible(bool newVisible)
{
    scrollbarsVisible = newVisible;
}

void ScrollableContainer::setScrollbarsPermanentlyVisible(bool permanentlyVisible /*= true*/)
{
    scrollbarsPermanentlyVisible = permanentlyVisible;
    xSlider.setVisible(true);
    ySlider.setVisible(true);
    invalidateScrollbars();
}

int16_t ScrollableContainer::getScrolledX() const
{
    return scrolledXDistance;
}

int16_t ScrollableContainer::getScrolledY() const
{
    return scrolledYDistance;
}

void ScrollableContainer::setScrollDurationSpeedup(uint16_t speedup)
{
    scrollDurationSpeedup = MAX(1, speedup);
}

uint16_t ScrollableContainer::getScrollDurationSpeedup() const
{
    return scrollDurationSpeedup;
}

void ScrollableContainer::setScrollDurationSlowdown(uint16_t slowdown)
{
    scrollDurationSlowdown = MAX(1, slowdown);
}

uint16_t ScrollableContainer::getScrollDurationSlowdown() const
{
    return scrollDurationSlowdown;
}

} // namespace touchgfx
