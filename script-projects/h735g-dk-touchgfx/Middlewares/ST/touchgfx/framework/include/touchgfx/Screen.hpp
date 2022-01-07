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
 * @file touchgfx/Screen.hpp
 *
 * Declares the touchgfx::Screen class.
 */
#ifndef TOUCHGFX_SCREEN_HPP
#define TOUCHGFX_SCREEN_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/GestureEvent.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
class Drawable;
class Transition;

/**
 * A Screen represents a full-screen drawable area. Applications create specific screens by
 * subclassing this class. Each Screen has a root container to which drawables are
 * added. The Screen makes sure to delegate draw requests and various events to the
 * appropriate drawables in correct order.
 */
class Screen
{
public:
    /** Initializes a new instance of the Screen class. */
    Screen();

    /** Finalizes an instance of the Screen class. */
    virtual ~Screen()
    {
    }

    /**
     * Tells the screen to draw its entire area.
     *
     * @note The more specific draw(Rect&amp;) version is preferred when possible.
     */
    void draw();

    /**
     * Starts a JSMOC run, analyzing what parts of what widgets should be redrawn.
     *
     * @param [in] invalidatedArea The area to redraw, expressed in absolute coordinates.
     *
     * @note SMOC is an abbreviation of <em>S&oslash;ren &amp; Martin's Occlusion Culling</em>.
     */
    void startSMOC(const Rect& invalidatedArea);

    /**
     * Recursive JSMOC function. This is the actual occlusion culling implementation.
     *
     * @param [in] invalidatedArea The area to redraw, expressed in absolute coordinates.
     * @param [in] widgetToDraw    Widget currently being drawn.
     *
     * @note JSMOC is an abbreviation of <em>Jesper, S&oslash;ren &amp; Martin's Occlusion Culling</em>.
     */
    void JSMOC(const Rect& invalidatedArea, Drawable* widgetToDraw);

    /**
     * Tell the screen to draw the specified area. Will traverse the drawables tree from in
     * z-order and delegate draw to them.
     *
     * @param [in] rect The area in absolute coordinates.
     *
     * @note The given rect must be in absolute coordinates.
     */
    virtual void draw(Rect& rect);

    /**
     * Called by Application::switchScreen() when this screen is going to be displayed. Base
     * version does nothing, but place any screen specific initialization code in an
     * overridden version.
     *
     * @see Application::switchScreen
     */
    virtual void setupScreen()
    {
    }

    /**
     * Called by Application::handleTickEvent() when the transition to the screen is done. Base
     * version does nothing, but override to do screen specific initialization code that has
     * to be done after the transition to the screen.
     *
     * @see Application::handleTickEvent
     */
    virtual void afterTransition()
    {
    }

    /**
     * Called by Application::switchScreen() when this screen will no longer be displayed.
     * Base version does nothing, but place any screen specific cleanup code in an
     * overridden version.
     *
     * @see Application::switchScreen
     */
    virtual void tearDownScreen()
    {
    }

    /**
     * Traverse the drawables in reverse z-order and notify them of a click event.
     *
     * @param  event The event to handle.
     */
    virtual void handleClickEvent(const ClickEvent& event);

    /**
     * Traverse the drawables in reverse z-order and notify them of a drag event.
     *
     * @param  event The event to handle.
     */
    virtual void handleDragEvent(const DragEvent& event);

    /**
     * Handle gestures. Traverses drawables in reverse-z and notifies them of the gesture.
     *
     * @param  event The event to handle.
     */
    virtual void handleGestureEvent(const GestureEvent& event);

    /**
     * Called by the Application on the current screen with a frequency of
     * Application::TICK_INTERVAL_MS.
     */
    virtual void handleTickEvent()
    {
    }

    /**
     * Called by the Application on the reception of a "key", the meaning of which is
     * platform/application specific. Default implementation does nothing.
     *
     * @param  key The key to handle.
     */
    virtual void handleKeyEvent(uint8_t key)
    {
    }

    /**
     * Determines if using JSMOC.
     *
     * @return true if this screen uses the JSMOC drawing algorithm.
     */
    bool usingSMOC() const
    {
        return useSMOC;
    }

    /**
     * Enables the transition to access the containers.
     *
     * @param [in] trans The transition to bind.
     */
    void bindTransition(Transition& trans);

    /**
     * Obtain a reference to the root container of this screen.
     *
     * @return The root container.
     */
    Container& getRootContainer()
    {
        return container;
    }

    /**
     * Gets width of the current screen. In most cases, this is the same as HAL::DISPLAY_WIDTH.
     *
     * @return  The screen width.
     */
    int16_t getScreenWidth() const
    {
        return HAL::DISPLAY_WIDTH;
    }

    /**
     * Gets height of the current screen. In most cases, this is the same as HAL::DISPLAY_HEIGHT.
     *
     * @return  The screen height.
     */
    int16_t getScreenHeight() const
    {
        return HAL::DISPLAY_HEIGHT;
    }

protected:
    /**
     * Determines whether to use JSMOC or painter's algorithm for drawing.
     *
     * @param  enabled true if JSMOC should be enabled, false if disabled (meaning painter's
     *                 algorithm is employed instead).
     */
    void useSMOCDrawing(bool enabled);

    /**
     * Add a drawable to the content container.
     *
     * @param [in] d The Drawable to add.
     *
     * @note Must not be called with a Drawable that was already added to the screen. If in doubt,
     *       call remove() first.
     */
    void add(Drawable& d)
    {
        container.add(d);
    }

    /**
     * Inserts a Drawable after a specific child node. See Container::insert.
     *
     * @param [in] previous The Drawable to insert after. If null, insert as header.
     * @param [in] d        The Drawable to insert.
     *
     * @see Container::insert
     *
     * @note As with add, do not add the same drawable twice.
     */
    void insert(Drawable* previous, Drawable& d)
    {
        container.insert(previous, d);
    }

    /**
     * Request that a region of this screen is redrawn. See Container::invalidateRect.
     *
     * To invalidate the entire Screen, use invalidate()
     *
     * @param [in] invalidatedArea The area of this drawable to redraw expressed in relative
     *                             coordinates.
     *
     * @see Container::invalidateRect
     */
    void invalidateRect(Rect& invalidatedArea) const
    {
        container.invalidateRect(invalidatedArea);
    }

    /**
     * Tell the framework that this entire Screen needs to be redrawn.
     *
     * @see Container::invalidate
     */
    void invalidate() const
    {
        container.invalidate();
    }

    /**
     * Removes a drawable from the content container. Safe to call even if the drawable was
     * never added (in which case nothing happens).
     *
     * @param [in] d The Drawable to remove.
     */
    void remove(Drawable& d)
    {
        container.remove(d);
    }

    Container container; ///< The container contains the contents of the screen.

    Drawable* focus; ///< The drawable currently in focus (set when DOWN_PRESSED is received).

private:
    int16_t fingerAdjustmentX;
    int16_t fingerAdjustmentY;
    bool useSMOC;
};

} // namespace touchgfx

#endif // TOUCHGFX_SCREEN_HPP
