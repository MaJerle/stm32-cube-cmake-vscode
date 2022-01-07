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
 * @file touchgfx/containers/scrollers/ScrollWheelBase.hpp
 *
 * Declares the touchgfx::ScrollWheelBase class.
 */
#ifndef TOUCHGFX_SCROLLWHEELBASE_HPP
#define TOUCHGFX_SCROLLWHEELBASE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/containers/scrollers/ScrollBase.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/events/DragEvent.hpp>
#include <touchgfx/events/GestureEvent.hpp>

namespace touchgfx
{
/**
 * A base class for a scroll wheel. A scroll wheel is very much like the digit selector on a
 * padlock with numbers. The digits always snap into place and exactly one number is
 * always the "selected" number. Using ScrollWheel, all elements look the same, but an
 * underlying bitmap can help emphasize the "selected" element. The
 * ScrollWheelWithSelectionStyle can have a completely different style on the "selected"
 * item - the font can be larger or bold and images can change color - this can help to
 * give a kind of 3D effect using very few resources.
 *
 * @see ScrollWheel, ScrollWheelWithSelectionStyle
 */
class ScrollWheelBase : public ScrollBase
{
public:
    ScrollWheelBase();

    /**
     * Sets selected item offset, measured in pixels, from the edge of the widget. The
     * offset is the relative x coordinate if the ScrollWheel is horizontal, otherwise it is
     * the relative y coordinate. If this value is zero, the selected item is placed at the
     * very start of the widget.
     *
     * @param  offset The offset.
     *
     * @see getSelectedItemOffset
     */
    virtual void setSelectedItemOffset(int16_t offset);

    /**
     * Gets offset of selected item measured in pixels relative to the start of the widget.
     *
     * @return The selected item offset.
     *
     * @see setSelectedItemOffset
     */
    virtual int16_t getSelectedItemOffset() const;

    /**
     * Gets selected item. If an animation is in progress, the item that is being scrolled
     * to is returned, not the item that happens to be flying by at the time.
     *
     * @return The selected item.
     */
    int getSelectedItem() const;

    virtual int32_t keepOffsetInsideLimits(int32_t newOffset, int16_t overShoot) const;

    virtual void handleClickEvent(const ClickEvent& event);

    virtual void handleDragEvent(const DragEvent& event);

    virtual void handleGestureEvent(const GestureEvent& event);

    /**
     * Sets Callback which will be called when the ScrollWheel animates to a new item.
     *
     * @param [in] callback The callback.
     */
    void setAnimateToCallback(GenericCallback<int16_t>& callback);

protected:
    virtual int32_t getPositionForItem(int16_t itemIndex);

    virtual void animateToPosition(int32_t position, int16_t steps = -1);

    GenericCallback<int16_t>* animateToCallback; ///< The animate to callback
};

} // namespace touchgfx

#endif // TOUCHGFX_SCROLLWHEELBASE_HPP
