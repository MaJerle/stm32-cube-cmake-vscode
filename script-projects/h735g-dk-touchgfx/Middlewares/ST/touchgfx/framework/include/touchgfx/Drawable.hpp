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
 * @file touchgfx/Drawable.hpp
 *
 * Declares the touchgfx::Drawable class.
 */
#ifndef TOUCHGFX_DRAWABLE_HPP
#define TOUCHGFX_DRAWABLE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/GestureEvent.hpp>

namespace touchgfx
{
/**
 * The Drawable class is an abstract definition of something that can be drawn. In the composite
 * design pattern, the Drawable is the component interface. Drawables can be added to a
 * screen as a tree structure through the leaf node class Widget and the Container
 * class. A Drawable contains a pointer to its next sibling and a pointer to its parent
 * node. These are maintained by the Container to which the Drawable is added.
 *
 * The Drawable interface contains two pure virtual functions which must be implemented
 * by widgets, namely draw() and getSolidRect(). In addition it contains general
 * functionality for receiving events and navigating the tree structure.
 *
 * The coordinates of a Drawable are always relative to its parent node.
 *
 * @see Widget, Container
 */
class Drawable
{
public:
    /** Initializes a new instance of the Drawable class. */
    Drawable()
        : rect(),
          cachedVisibleRect(),
          parent(0),
          nextSibling(0),
          nextDrawChainElement(0),
          cachedAbsX(0),
          cachedAbsY(0),
          touchable(false),
          visible(true)
    {
    }

    /** Finalizes an instance of the Drawable class. */
    virtual ~Drawable()
    {
    }

    /**
     * Draw this drawable. It is a requirement that the draw implementation does not draw
     * outside the region specified by invalidatedArea.
     *
     * @param  invalidatedArea The sub-region of this drawable that needs to be redrawn,
     *                         expressed in coordinates relative to its parent (e.g. for a
     *                         complete redraw, invalidatedArea will be (0, 0, width,
     *                         height).
     */
    virtual void draw(const Rect& invalidatedArea) const = 0;

    /**
     * Get (the largest possible) rectangle that is guaranteed to be solid (opaque). This
     * information is important, as any Drawable underneath the solid area does not need to
     * be drawn.
     *
     * @return The solid rectangle part of the Drawable.
     *
     * @note The rectangle returned must be relative to upper left corner of the Drawable, meaning
     *       that a completely solid widget should return the full size Rect(0, 0,
     *       getWidth(), getHeight()). If no area can be guaranteed to be solid, an empty
     *       Rect(0, 0, 0, 0) must be returned. Failing to return the correct rectangle
     *       may result in errors on the display.
     */
    virtual Rect getSolidRect() const = 0;

    /**
     * Request that a region of this drawable is redrawn. Will recursively traverse the tree
     * towards the root, and once reached, issue a draw operation. When this function
     * returns, the specified invalidated area has been redrawn for all appropriate
     * Drawables covering the region.
     *
     * To invalidate the entire Drawable, use invalidate()
     *
     * @param [in] invalidatedArea The area of this drawable to redraw expressed in relative
     *                             coordinates.
     *
     * @see invalidate
     */
    virtual void invalidateRect(Rect& invalidatedArea) const;

    /**
     * Tell the framework that this entire Drawable needs to be redrawn. It is the same as
     * calling invalidateRect() with Rect(0, 0, getWidth(), getHeight()).
     *
     * @see invalidateRect
     */
    virtual void invalidate() const;

    /**
     * Gets the next sibling node. This will be the next Drawable that has been added to the
     * same Container as this Drawable.
     *
     * @return The next sibling. If there are no more siblings, the return value is 0.
     */
    Drawable* getNextSibling()
    {
        return nextSibling;
    }

    /**
     * Function for obtaining the first child of this drawable if any.
     *
     * @return A pointer on the first child drawable if any.
     * @see Container::getFirstChild
     */
    virtual Drawable* getFirstChild()
    {
        return 0;
    }

    /**
     * Helper function for obtaining the largest solid rect (as implemented by
     * getSolidRect()) expressed in absolute coordinates. Will recursively traverse to the
     * root of the tree to find the proper location of the rectangle on the display.
     *
     * @return The (largest) solid rect (as implemented by getSolidRect()) expressed in absolute
     *         coordinates.
     */
    virtual Rect getSolidRectAbsolute();

    /**
     * Function for obtaining the the last child of this drawable that intersects with the
     * specified point. The last child is the Drawable that is drawn last and therefore the
     * topmost child. Used in input event handling for obtaining the appropriate drawable
     * that should receive the event.
     *
     * @param       x    The point of intersection expressed in coordinates relative to the
     *                   parent.
     * @param       y    The point of intersection expressed in coordinates relative to the
     *                   parent.
     * @param [out] last Last (topmost) Drawable on the given coordinate.
     *
     * @note Input events must be delegated to the last drawable of the tree (meaning highest z-
     *       order / front-most drawable).
     */
    virtual void getLastChild(int16_t x, int16_t y, Drawable** last) = 0;

    /**
     * Function for finding the visible part of this drawable. If the parent node has a
     * smaller area than this Drawable, or if the Drawable is placed "over the edge" of the
     * parent, the parent will act as a view port, cutting off the parts of this Drawable
     * that are outside the region. Traverses the tree and yields a result expressed in
     * absolute coordinates.
     *
     * @param [out] rect The region of the Drawable that is visible.
     */
    virtual void getVisibleRect(Rect& rect) const;

    /**
     * Gets the rectangle this Drawable covers, in coordinates relative to its parent.
     *
     * @return The rectangle this Drawable covers expressed in coordinates relative to its
     *         parent.
     *
     * @see getAbsoluteRect
     */
    const Rect& getRect() const
    {
        return rect;
    }

    /**
     * Helper function for obtaining the rectangle this Drawable covers, expressed in
     * absolute coordinates.
     *
     * @return The rectangle this Drawable covers expressed in absolute coordinates.
     *
     * @see getRect, translateRectToAbsolute
     */
    Rect getAbsoluteRect() const;

    /**
     * Helper function for converting a region of this Drawable to absolute coordinates.
     *
     * @param [in,out] r The Rect to translate.
     */
    virtual void translateRectToAbsolute(Rect& r) const;

    /**
     * Sets the size and position of this Drawable, relative to its parent. The same as
     * calling setXY(), setWidth() and setHeight() in that order.
     *
     * @param  x      The x coordinate of this Drawable relative to its parent.
     * @param  y      The y coordinate of this Drawable relative to its parent.
     * @param  width  The width of this Drawable.
     * @param  height The height of this Drawable.
     *
     * @note For most Drawable widgets, changing this does normally not automatically yield a redraw.
     */
    void setPosition(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        setXY(x, y);
        setWidthHeight(width, height);
    }

    /**
     * Expands the Drawable to have the same size as its parent with a given margin around
     * the edge. If there is no parent, the position is set to the size of the entire display.
     *
     * @param  margin (Optional) The margin.
     */
    void expand(int margin = 0);

    /** Centers the Drawable inside its parent. */
    void center()
    {
        centerX();
        centerY();
    }

    /** Center the Drawable horizontally inside its parent. */
    void centerX()
    {
        assert(parent && "Cannot center a Drawable with no parent");
        setX((parent->getWidth() - getWidth()) / 2);
    }

    /** Center the Drawable vertically inside its parent. */
    void centerY()
    {
        assert(parent && "Cannot center a Drawable with no parent");
        setY((parent->getHeight() - getHeight()) / 2);
    }

    /**
     * Gets the x coordinate of this Drawable, relative to its parent.
     *
     * @return The x value, relative to the parent.
     */
    int16_t getX() const
    {
        return rect.x;
    }

    /**
     * Gets the y coordinate of this Drawable, relative to its parent.
     *
     * @return The y value, relative to the parent.
     */
    int16_t getY() const
    {
        return rect.y;
    }

    /**
     * Gets the width of this Drawable.
     *
     * @return The width of this Drawable.
     */
    int16_t getWidth() const
    {
        return rect.width;
    }

    /**
     * Gets the height of this Drawable.
     *
     * @return The height of this Drawable.
     */
    int16_t getHeight() const
    {
        return rect.height;
    }

    /**
     * Sets the x coordinate of this Drawable, relative to its parent.
     *
     * @param  x The new x value, relative to the parent. A negative value is allowed.
     *
     * @note For most Drawable widgets, changing this does normally not automatically yield a redraw.
     */
    virtual void setX(int16_t x)
    {
        rect.x = x;
    }

    /**
     * Sets the y coordinate of this Drawable, relative to its parent.
     *
     * @param  y The new y value, relative to the parent. A negative value is allowed.
     *
     * @note For most Drawable widgets, changing this does normally not automatically yield a redraw.
     */
    virtual void setY(int16_t y)
    {
        rect.y = y;
    }

    /**
     * Sets the x and y coordinates of this Drawable, relative to its parent. The same as
     * calling setX() followed by calling setY().
     *
     * @param  x The new x value, relative to the parent. A negative value is allowed.
     * @param  y The new y value, relative to the parent. A negative value is allowed.
     *
     * @see moveTo
     *
     * @note For most Drawable widgets, changing this does normally not automatically yield a redraw.
     */
    void setXY(int16_t x, int16_t y)
    {
        setX(x);
        setY(y);
    }

    /**
     * Sets the width of this drawable.
     *
     * @param  width The new width.
     *
     * @note For most Drawable widgets, changing this does normally not automatically yield a redraw.
     */
    virtual void setWidth(int16_t width)
    {
        rect.width = width;
    }

    /**
     * Sets the height of this drawable.
     *
     * @param  height The new height.
     *
     * @note For most Drawable widgets, changing this does normally not automatically yield a redraw.
     */
    virtual void setHeight(int16_t height)
    {
        rect.height = height;
    }

    /**
     * This function can be called on parent nodes to signal that the size or position of
     * one or more of its children has changed. Currently only used in ScrollableContainer
     * to redraw scrollbars when the size of the scrolling contents changes.
     */
    virtual void childGeometryChanged()
    {
    }

    /**
     * Defines the event handler interface for ClickEvents. The default implementation
     * ignores the event. The event is only received if the Drawable is touchable and
     * visible.
     *
     * @param  event The ClickEvent received from the HAL.
     */
    virtual void handleClickEvent(const ClickEvent& event)
    {
    }

    /**
     * Defines the event handler interface for GestureEvents. The default implementation
     * ignores the event. The event is only received if the Drawable is touchable and
     * visible.
     *
     * @param  event The GestureEvent received from the HAL.
     */
    virtual void handleGestureEvent(const GestureEvent& event)
    {
    }

    /**
     * Sets the dimensions (width and height) of the Drawable without changing the x and y
     * coordinates).
     *
     * @param  width  The width.
     * @param  height The height.
     */
    void setWidthHeight(int16_t width, int16_t height)
    {
        setWidth(width);
        setHeight(height);
    }

    /**
     * Sets the position of the Drawable to the same as the given Drawable. This will copy
     * the x, y, width and height.
     *
     * @param  drawable The drawable.
     *
     * @see setPosition(int16_t,int16_t,int16_t,int16_t)
     */
    void setPosition(const Drawable& drawable)
    {
        setPosition(drawable.getX(), drawable.getY(), drawable.getWidth(), drawable.getHeight());
    }

    /**
     * Sets the x and y coordinates of this Drawable.
     *
     * @param  drawable The Drawable to copy the x and y coordinates from.
     *
     * @see setXY(int16_t,int16_t)
     */
    void setXY(const Drawable& drawable)
    {
        setXY(drawable.getX(), drawable.getY());
    }

    /**
     * Sets the dimensions (width and height) of the Drawable without changing the x and y
     * coordinates).
     *
     * @param  drawable The Drawable to copy the width and height from.
     *
     * @see setWidthHeight(int16_t,int16_t)
     */
    void setWidthHeight(const Drawable& drawable)
    {
        setWidthHeight(drawable.getWidth(), drawable.getHeight());
    }

    /**
     * Sets the dimensions (width and height) of the Drawable without changing the x and y
     * coordinates).
     *
     * @param  bitmap The Bitmap to copy the width and height from.
     *
     * @see setWidthHeight(int16_t,int16_t)
     */
    void setWidthHeight(const Bitmap& bitmap)
    {
        setWidthHeight(bitmap.getWidth(), bitmap.getHeight());
    }

    /**
     * Sets the dimensions (width and height) of the Drawable without changing the x and y
     * coordinates).
     *
     * @param  rect The Rect to copy the width and height from.
     *
     * @see setWidthHeight(int16_t,int16_t)
     */
    void setWidthHeight(const Rect& rect)
    {
        setWidthHeight(rect.width, rect.height);
    }

    /**
     * Defines the event handler interface for DragEvents. The default implementation
     * ignores the event. The event is only received if the drawable is touchable and
     * visible.
     *
     * @param  event The DragEvent received from the HAL.
     */
    virtual void handleDragEvent(const DragEvent& event)
    {
    }

    /**
     * Called periodically by the framework if the Drawable instance has subscribed to timer
     * ticks.
     *
     * @see Application::registerTimerWidget
     */
    virtual void handleTickEvent()
    {
    }

    /**
     * Controls whether this Drawable should be visible. Only visible Drawables will have
     * their draw function called. Additionally, invisible drawables will not receive input
     * events.
     *
     * @param  vis true if this Drawable should be visible. By default, drawables are visible
     *             unless this function has been called with false as argument.
     *
     * @note For most Drawable widgets, changing this does normally not automatically yield a redraw.
     */
    void setVisible(bool vis)
    {
        visible = vis;
    }

    /**
     * Controls whether this Drawable receives touch events or not.
     *
     * @param  touch If true it will receive touch events, if false it will not.
     */
    void setTouchable(bool touch)
    {
        touchable = touch;
    }

    /**
     * Gets whether this Drawable is visible.
     *
     * @return true if the Drawable is visible.
     *
     * @see setVisible
     */
    bool isVisible() const
    {
        return visible;
    }

    /**
     * Gets whether this Drawable receives touch events or not.
     *
     * @return True if touch events are received.
     *
     * @see setTouchable
     */
    bool isTouchable() const
    {
        return touchable;
    }

    /**
     * Returns the parent node. For the root container, the return value is 0.
     *
     * @return The parent node. For the root container, the return value is 0.
     *
     * @note A disconnected Drawable also has parent 0 which may cause strange side effects.
     */
    Drawable* getParent() const
    {
        return parent;
    }

    /**
     * Moves the drawable.
     *
     * @param  x The relative position to move to.
     * @param  y The relative position to move to.
     *
     * @see moveTo, setXY
     *
     * @note Will redraw the appropriate areas of the screen.
     */
    virtual void moveRelative(int16_t x, int16_t y);

    /**
     * Moves the drawable.
     *
     * @param  x The absolute position to move to.
     * @param  y The absolute position to move to.
     *
     * @see moveRelative, setXY
     *
     * @note Will redraw the appropriate areas of the screen.
     */
    virtual void moveTo(int16_t x, int16_t y)
    {
        moveRelative(x - rect.x, y - rect.y);
    }

    /**
     * Render the Drawable object into a dynamic bitmap.
     *
     * @param  id The target dynamic bitmap to use for rendering.
     */
    void drawToDynamicBitmap(BitmapId id);

protected:
    Rect rect; ///< The coordinates of this Drawable, relative to its parent.
    /// @cond
    Rect cachedVisibleRect; ///< Cached representation of currently visible area. For TouchGFX internal use.
    /// @endcond
    Drawable* parent;      ///< Pointer to this drawable's parent.
    Drawable* nextSibling; ///< Pointer to the next Drawable. Maintained by containers.
    /// @cond
    Drawable* nextDrawChainElement; ///< Next in draw chain. For TouchGFX internal use.
    int16_t cachedAbsX;             ///< Cached value of absolute x coordinate. For TouchGFX internal use.
    int16_t cachedAbsY;             ///< Cached value of absolute y coordinate. For TouchGFX internal use.
    /// @endcond
    bool touchable; ///< True if this drawable should receive touch events.
    bool visible;   ///< True if this drawable should be drawn.

    /// @cond
    static const int16_t UNCACHED_INDICATOR = -1; ///< Constant representing uncached value. For TouchGFX internal use.

    /**
     * For TouchGFX internal use only.
     *
     * Reset cached coordinate data.
     *
     * @note For TouchGFX internal use only.
     */
    void resetDrawChainCache()
    {
        // Resetting the cached indicators
        cachedVisibleRect.x = UNCACHED_INDICATOR;
        cachedAbsX = UNCACHED_INDICATOR;
        cachedAbsY = UNCACHED_INDICATOR;
    }

    /**
     * For TouchGFX internal use only.
     *
     * Obtain cached version of visible rect.
     *
     * @return The Visible rect for this drawable. Only calculated once.
     *
     * @note For TouchGFX internal use only.
     */
    Rect& getCachedVisibleRect()
    {
        if (cachedVisibleRect.x == UNCACHED_INDICATOR)
        {
            Rect visibleRect(0, 0, getWidth(), getHeight());
            getVisibleRect(visibleRect);
            cachedVisibleRect = visibleRect;
        }
        return cachedVisibleRect;
    }

    /**
     * For TouchGFX internal use only.
     *
     * Obtain cached version of absolute X-coord.
     *
     * @return The absolute x coordinate for this drawable. Only calculated once.
     *
     * @note For TouchGFX internal use only.
     */
    int16_t getCachedAbsX()
    {
        if (cachedAbsX == UNCACHED_INDICATOR)
        {
            Rect absRect = getAbsoluteRect();
            cachedAbsX = absRect.x;
            cachedAbsY = absRect.y;
        }
        return cachedAbsX;
    }

    /**
     * For TouchGFX internal use only.
     *
     * Obtain cached version of absolute Y-coord.
     *
     * @return The absolute y coordinate for this drawable. Only calculated once.
     *
     * @note For TouchGFX internal use only.
     */
    int16_t getCachedAbsY()
    {
        if (cachedAbsY == UNCACHED_INDICATOR)
        {
            Rect absRect = getAbsoluteRect();
            cachedAbsX = absRect.x;
            cachedAbsY = absRect.y;
        }
        return cachedAbsY;
    }

    /**
     * For TouchGFX internal use only.
     *
     * Configure linked list for draw chain.
     *
     * @param          invalidatedArea     Include drawables that intersect with this area only.
     * @param [in,out] nextPreviousElement Modifiable element in linked list.
     *
     * @note For TouchGFX internal use only.
     */
    virtual void setupDrawChain(const Rect& invalidatedArea, Drawable** nextPreviousElement)
    {
        resetDrawChainCache();
        nextDrawChainElement = *nextPreviousElement;
        *nextPreviousElement = this;
    }
    /// @endcond

    friend class Container;
    friend class Screen;
};

} // namespace touchgfx

#endif // TOUCHGFX_DRAWABLE_HPP
