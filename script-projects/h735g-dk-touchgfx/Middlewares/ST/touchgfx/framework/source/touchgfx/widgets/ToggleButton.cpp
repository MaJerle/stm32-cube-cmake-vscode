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

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>

namespace touchgfx
{
void ToggleButton::forceState(bool activeState)
{
    if (activeState)
    {
        // up should equal originalPressed
        if (up.getId() != originalPressed.getId())
        {
            down = up;
            up = originalPressed;
        }
    }
    else
    {
        // down should equal originalPressed
        if (down.getId() != originalPressed.getId())
        {
            up = down;
            down = originalPressed;
        }
    }
}

void ToggleButton::handleClickEvent(const ClickEvent& event)
{
    if (pressed && (event.getType() == ClickEvent::RELEASED))
    {
        Bitmap tmp = up;
        up = down;
        down = tmp;
    }
    Button::handleClickEvent(event);
}
} // namespace touchgfx
