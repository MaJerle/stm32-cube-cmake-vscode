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

#ifndef TOUCHGFX_JSMOCHELPER_HPP
#define TOUCHGFX_JSMOCHELPER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Drawable.hpp>

/// @cond
namespace touchgfx
{
/**
 * Helper class providing caching of certain information while the JSMOC algorithm runs during
 * draw operations.
 *
 * @note Not intended for application-level use.
 * @note JSMOC is an abbreviation of <em>Jesper, S&oslash;ren &amp; Martin's Occlusion Culling</em>.
 */
class JSMOCHelper
{
public:
    /** Initializes a new instance of the JSMOCHelper class. */
    JSMOCHelper()
    {
    }

    /**
     * Sets a widget.
     *
     * @param [in] newWidget The widget to operate on.
     */
    void setWidget(Drawable* newWidget)
    {
        widget = newWidget;

        // Resetting the cached indicators
        cachedVisibleRect.x = CACHED_INDICATOR;
        cachedAbsX = CACHED_INDICATOR;
        cachedAbsY = CACHED_INDICATOR;
    }

    /**
     * Gets the widget.
     *
     * @return The widget this helper operates on.
     */
    Drawable* getWidget()
    {
        return widget;
    }

    /**
     * Gets the visible rect for the widget of this helper.
     *
     * @return The visible rect for the widget of this helper. Only calculated once.
     */
    Rect& getCachedVisibleRect()
    {
        assert(widget != 0);

        if (!hasCachedVisibleRect())
        {
            Rect visibleRect(0, 0, widget->getWidth(), widget->getHeight());
            widget->getVisibleRect(visibleRect);
            cachedVisibleRect = visibleRect;
        }
        return cachedVisibleRect;
    }

    /**
     * Gets the absolute x coordinate for the widget of this helper.
     *
     * @return The absolute x coordinate for the widget of this helper. Only calculated once.
     */
    int16_t getCachedAbsX()
    {
        if (!hasCachedAbsX())
        {
            Rect absRect = widget->getAbsoluteRect();
            cachedAbsX = absRect.x;
            cachedAbsY = absRect.y;
        }
        return cachedAbsX;
    }

    /**
     * Gets the absolute y coordinate for the widget of this helper.
     *
     * @return The absolute y coordinate for the widget of this helper. Only calculated once.
     */
    int16_t getCachedAbsY()
    {
        if (!hasCachedAbsY())
        {
            Rect absRect = widget->getAbsoluteRect();
            cachedAbsX = absRect.x;
            cachedAbsY = absRect.y;
        }
        return cachedAbsY;
    }

    /**
     * Gets the width of the widget of this helper.
     *
     * @return The width of the widget of this helper.
     */
    int16_t getWidth()
    {
        return widget->getWidth();
    }

    /**
     * Gets the height of the widget of this helper.
     *
     * @return The height of the widget of this helper.
     */
    int16_t getHeight()
    {
        return widget->getHeight();
    }

    /**
     * Draws the widget of this helper.
     *
     * @param  invalidatedArea The area of the widget to draw.
     */
    void draw(const Rect& invalidatedArea)
    {
        widget->draw(invalidatedArea);
    }

private:
    static const int16_t CACHED_INDICATOR = -1;

    Drawable* widget;

    Rect cachedVisibleRect;

    /**
     * Query if this object has cached visible rectangle.
     *
     * @return True if cached visible rectangle, false if not.
     */
    bool hasCachedVisibleRect()
    {
        return cachedVisibleRect.x != CACHED_INDICATOR;
    }

    int16_t cachedAbsX;
    int16_t cachedAbsY;

    /**
     * Query if this object has cached abs x coordinate.
     *
     * @return True if cached abs x coordinate, false if not.
     */
    bool hasCachedAbsX()
    {
        return cachedAbsX != CACHED_INDICATOR;
    }

    /**
     * Query if this object has cached abs y coordinate.
     *
     * @return True if cached abs y coordinate, false if not.
     */
    bool hasCachedAbsY()
    {
        return cachedAbsY != CACHED_INDICATOR;
    }
};

} // namespace touchgfx
/// @endcond

#endif // TOUCHGFX_JSMOCHELPER_HPP
