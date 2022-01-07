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
 * @file touchgfx/containers/buttons/RepeatButtonTrigger.hpp
 *
 * Declares the touchgfx::RepeatButtonTrigger class.
 */
#ifndef TOUCHGFX_REPEATBUTTONTRIGGER_HPP
#define TOUCHGFX_REPEATBUTTONTRIGGER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Application.hpp>
#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>
#include <touchgfx/events/ClickEvent.hpp>

namespace touchgfx
{
/**
 * A repeat button trigger. This trigger will create a button that reacts to a consistent touch.
 * This means it will call the set action repeatedly as long as it is touched. The
 * RepeatButtonTrigger can be combined with one or more of the ButtonStyle classes to
 * create a fully functional button.
 */
class RepeatButtonTrigger : public AbstractButtonContainer
{
public:
    RepeatButtonTrigger()
        : AbstractButtonContainer(), ticksDelay(30), ticksInterval(15), ticks(0), ticksBeforeContinuous(0)
    {
    }

    /**
     * Sets the delay (in number of ticks) from the first button activation until the next
     * time it will be automatically activated.
     *
     * @param  delay The delay, measured in ticks, between first activation and second activation.
     *
     * @see setInterval, getDelay
     */
    void setDelay(int delay)
    {
        ticksDelay = delay;
    }

    /**
     * Gets the delay in ticks from first button activation until next activation.
     *
     * @return The delay, measured in ticks, between first activation and second activation.
     *
     * @see setDelay
     */
    int getDelay()
    {
        return ticksDelay;
    }

    /**
     * Sets the interval in number of ticks between each each activation of the pressed
     * button after the second activation.
     *
     * @param  interval The interval between repeated activations, measured in ticks.
     *
     * @see setDelay, getInterval
     */
    void setInterval(int interval)
    {
        ticksInterval = interval;
    }

    /**
     * The interval between repeated activations, measured in ticks. This is the number of
     * ticks between the an activation beyond the first and the following activation.
     *
     * @return The interval between repeated activations, measured in ticks.
     *
     * @see setInterval
     */
    int getInterval()
    {
        return ticksInterval;
    }

    void handleClickEvent(const ClickEvent& event)
    {
        bool wasPressed = getPressed();
        bool newPressedValue = (event.getType() == ClickEvent::PRESSED);
        if ((newPressedValue && !wasPressed) || (!newPressedValue && wasPressed))
        {
            setPressed(newPressedValue);
            invalidate();
        }

        if (event.getType() == ClickEvent::PRESSED)
        {
            executeAction();

            ticks = 0;
            ticksBeforeContinuous = ticksDelay;
            Application::getInstance()->registerTimerWidget(this);
        }
        else
        {
            Application::getInstance()->unregisterTimerWidget(this);
        }
    }

    void handleTickEvent()
    {
        AbstractButtonContainer::handleTickEvent();

        if (!pressed)
        {
            return;
        }
        if (ticks == ticksBeforeContinuous)
        {
            executeAction();

            ticks = 0;
            ticksBeforeContinuous = ticksInterval;
        }
        else
        {
            ticks++;
        }
    }

private:
    int16_t ticksDelay;
    int16_t ticksInterval;

    int16_t ticks;
    int16_t ticksBeforeContinuous;
};

} // namespace touchgfx

#endif // TOUCHGFX_REPEATBUTTONTRIGGER_HPP
