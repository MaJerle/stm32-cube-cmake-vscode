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

/**
 * @file touchgfx/containers/SwipeContainer.hpp
 *
 * Declares the touchgfx::SwipeContainer class.
 */
#ifndef TOUCHGFX_SWIPECONTAINER_HPP
#define TOUCHGFX_SWIPECONTAINER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/GestureEvent.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TiledImage.hpp>

namespace touchgfx
{
/**
 * A SwipeContainer is a Container with a horizontally laid out list of identically sized Drawables. The bottom of
 * the SwipeContainer shows a page indicator to indicate the position in the horizontal
 * list of items in the SwipeContainer.
 *
 * @see ListLayout
 */
class SwipeContainer : public Container
{
public:
    SwipeContainer();
    virtual ~SwipeContainer();

    virtual void handleTickEvent();
    virtual void handleClickEvent(const ClickEvent& event);
    virtual void handleDragEvent(const DragEvent& event);
    virtual void handleGestureEvent(const GestureEvent& event);

    /**
     * Adds a page to the container.
     *
     * @param [in] page The page to add.
     *
     * @note All pages must have the same width and height.
     */
    virtual void add(Drawable& page);

    /**
     * Removes the page from the container.
     *
     * @param [in] page The page to remove.
     *
     * @note This is safe to call even if page is not a page (in which case nothing happens).
     */
    virtual void remove(Drawable& page);

    /**
     * Set the swipe cutoff which indicates how far you should drag a page before it results in
     * a page change.
     *
     * @param  cutoff The cutoff in pixels.
     */
    virtual void setSwipeCutoff(uint16_t cutoff);

    /**
     * Sets the x and y position of the page indicator.
     *
     * @param  x The x coordinate.
     * @param  y The y coordinate.
     *
     * @see setPageIndicatorXYWithCenteredX, setPageIndicatorCenteredX
     */
    void setPageIndicatorXY(int16_t x, int16_t y);

    /**
     * Sets the x and y position of the page indicator. The value specified as x will be the
     * center coordinate of the page indicators.
     *
     * @param  x The center x coordinate.
     * @param  y The y coordinate.
     *
     * @see setPageIndicatorCenteredX, setPageIndicatorXY
     *
     * @note This method should not be used until all pages have been added, the
     *       setPageIndicatorBitmaps() has been called and the page indicator therefore has the
     *       correct width.
     */
    void setPageIndicatorXYWithCenteredX(int16_t x, int16_t y);

    /**
     * Sets the page indicator centered inside the SwipeContainer without changing the y
     * position.
     *
     * @see setPageIndicatorXYWithCenteredX, setPageIndicatorXY
     *
     * @note This method should not be used until all pages have been added, the
     *       setPageIndicatorBitmaps() has been called and the page indicator therefore has the
     *       correct width.
     *
     * ### param  x The center x coordinate.
     */
    void setPageIndicatorCenteredX();

    /**
     * Sets the x position of the page indicator without changing the y position. The value
     * specified as x will be the center coordinate of the page indicators.
     *
     * @param  x The center x coordinate.
     *
     * @see setPageIndicatorXYWithCenteredX, setPageIndicatorXY
     *
     * @note This method should not be used until all pages have been added, the
     *       setPageIndicatorBitmaps() has been called and the page indicator therefore has the
     *       correct width.
     */
    void setPageIndicatorCenteredX(int16_t x);

    /**
     * Sets the bitmaps that are used by the page indicator. The bitmap for the normal page is
     * repeated side-by-side and the bitmap for a highlighted page is put in the proper position.
     *
     * @param  normalPage      The normal page.
     * @param  highlightedPage The highlighted page.
     */
    void setPageIndicatorBitmaps(const Bitmap& normalPage, const Bitmap& highlightedPage);

    /**
     * When dragging either one of the end pages a part of the background will become visible
     * until the user stop dragging and the end page swipes back to its position. The width of
     * this area is set by this method.
     *
     * @param  width The width in pixels.
     */
    void setEndSwipeElasticWidth(uint16_t width);

    /**
     * Gets number of pages.
     *
     * @return The number of pages.
     */
    uint8_t getNumberOfPages()
    {
        return numberOfPages;
    }

    /**
     * Sets the selected page.
     *
     * @param  pageIndex Zero-based index of the page. Range from 0 to numberOfPages-1.
     *
     * @see getSelectedPage
     */
    void setSelectedPage(uint8_t pageIndex);

    /**
     * Gets the currently selected page.
     *
     * @return Zero-based index of the current page. Rage from 0 to numberOfPages-1.
     *
     * @see setSelectedPage
     */
    uint8_t getSelectedPage() const;

private:
    static const int16_t DRAG_CANCEL_THRESHOLD = 3;

    enum States
    {
        ANIMATE_SWIPE_CANCELLED_LEFT,
        ANIMATE_SWIPE_CANCELLED_RIGHT,
        ANIMATE_LEFT,
        ANIMATE_RIGHT,
        NO_ANIMATION
    } currentState;

    uint8_t numberOfPages;
    uint8_t animationCounter;
    uint16_t swipeCutoff;
    int16_t dragX;
    int16_t animateDistance;
    int16_t startX;
    uint8_t currentPage;
    uint16_t endElasticWidth;

    ListLayout pages;

    void adjustPages();

    void animateSwipeCancelledLeft();
    void animateSwipeCancelledRight();
    void animateLeft();
    void animateRight();

    class PageIndicator : public Container
    {
    public:
        PageIndicator();
        void setNumberOfPages(uint8_t size);
        void setBitmaps(const Bitmap& normalPage, const Bitmap& highlightedPage);
        void goRight();
        void goLeft();
        void setHighlightPosition(uint8_t index);

    private:
        TiledImage unselectedPages;
        Image selectedPage;
        uint8_t numberOfPages;
        uint8_t currentPage;
    } pageIndicator;
};

} // namespace touchgfx

#endif // TOUCHGFX_SWIPECONTAINER_HPP
