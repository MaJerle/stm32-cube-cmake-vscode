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
 * @file touchgfx/mixins/FadeAnimator.hpp
 *
 * Declares the touchgfx::FadeAnimator class.
 */
#ifndef TOUCHGFX_FADEANIMATOR_HPP
#define TOUCHGFX_FADEANIMATOR_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Application.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{
/**
 * A FadeAnimator makes the template class T able to animate the alpha value from its current
 * value to a specified end value. It is possible to use a fade in effect as well as
 * fade out effect using FadeAnimator. The alpha progression can be described by
 * supplying an EasingEquation. The FadeAnimator performs a callback when the animation
 * has finished.
 *
 * This mixin can be used on any Drawable that has a 'void setAlpha(uint8_t)' and a
 * 'uint8_t getAlpha()' method.
 *
 * @tparam T specifies the type to extend with the FadeAnimator behavior.
 */
template <class T>
class FadeAnimator : public T
{
public:
    FadeAnimator()
        : T(),
          fadeAnimationRunning(false),
          fadeAnimationCounter(0),
          fadeAnimationDelay(0),
          fadeAnimationEndedCallback(0)
    {
    }

    /**
     * Associates an action to be performed when the animation ends.
     *
     * @param  callback The callback to be executed. The callback will be given a reference
     *                  to the FadeAnimator.
     */
    void setFadeAnimationEndedAction(GenericCallback<const FadeAnimator<T>&>& callback)
    {
        fadeAnimationEndedCallback = &callback;
    }

    /**
     * Clears the fade animation ended action previously set by setFadeAnimationEndedAction.
     *
     * Clears the fade animation ended action previously set by setFadeAnimationEndedAction.
     *
     * @see setFadeAnimationEndedAction
     */
    void clearFadeAnimationEndedAction()
    {
        fadeAnimationEndedCallback = 0;
    }

    /**
     * Sets a delay before the actual animation starts for the animation done by the
     * FadeAnimator.
     *
     * @param  delay The delay in ticks.
     *
     * @see getFadeAnimationDelay
     */
    virtual void setFadeAnimationDelay(uint16_t delay)
    {
        fadeAnimationDelay = delay;
    }

    /**
     * Gets the current animation delay.
     *
     * @return The current animation delay.
     *
     * @see setFadeAnimationDelay
     */
    virtual uint16_t getFadeAnimationDelay() const
    {
        return fadeAnimationDelay;
    }

    /**
     * Gets whether or not the fade animation is running.
     *
     * @return true if the fade animation is running.
     */
    bool isFadeAnimationRunning() const
    {
        return fadeAnimationRunning;
    }

    /**
     * Starts the fade animation from the current alpha value to the specified end alpha
     * value. The progression of the alpha value during the animation is described by the
     * supplied EasingEquation.
     *
     * @param  endAlpha                 The alpha value of T at animation end.
     * @param  duration                 The duration of the animation measured in ticks.
     * @param  alphaProgressionEquation (Optional) The equation that describes the
     *                                  development of the alpha value during the animation.
     *                                  Default is EasingEquations::linearEaseNone.
     */
    void startFadeAnimation(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone)
    {
        if (!fadeAnimationRunning)
        {
            Application::getInstance()->registerTimerWidget(this);
        }

        fadeAnimationCounter = 0;
        fadeAnimationStartAlpha = T::getAlpha();
        fadeAnimationEndAlpha = endAlpha;
        fadeAnimationDuration = duration;
        fadeAnimationAlphaEquation = alphaProgressionEquation;

        fadeAnimationRunning = true;

        if (fadeAnimationDelay == 0 && fadeAnimationDuration == 0)
        {
            nextFadeAnimationStep(); // Set end alpha and shut down
        }
    }

    /**
     * Cancel fade animation. The animation is stopped and the alpha value is left where it
     * currently is.
     */
    void cancelFadeAnimation()
    {
        if (fadeAnimationRunning)
        {
            Application::getInstance()->unregisterTimerWidget(this);
            fadeAnimationRunning = false;
        }
    }

    /** @copydoc Drawable::handleTickEvent */
    virtual void handleTickEvent()
    {
        T::handleTickEvent();
        nextFadeAnimationStep();
    }

protected:
    /** Execute next step in fade animation and stop the timer if necessary. */
    void nextFadeAnimationStep()
    {
        if (fadeAnimationRunning)
        {
            fadeAnimationCounter++;
            if (fadeAnimationCounter >= fadeAnimationDelay)
            {
                // Adjust the used animationCounter for the startup delay
                uint32_t actualAnimationCounter = fadeAnimationCounter - fadeAnimationDelay;

                int16_t deltaAlpha = (int16_t)fadeAnimationAlphaEquation(actualAnimationCounter, 0, fadeAnimationEndAlpha - fadeAnimationStartAlpha, fadeAnimationDuration);

                T::setAlpha(fadeAnimationStartAlpha + deltaAlpha);
                T::invalidate();

                if (fadeAnimationCounter >= (uint32_t)(fadeAnimationDelay + fadeAnimationDuration))
                {
                    // End of animation
                    fadeAnimationRunning = false;
                    fadeAnimationDuration = 0;
                    Application::getInstance()->unregisterTimerWidget(this);

                    if (fadeAnimationEndedCallback && fadeAnimationEndedCallback->isValid())
                    {
                        fadeAnimationEndedCallback->execute(*this);
                    }
                }
            }
        }
    }

    bool fadeAnimationRunning;                 ///< True if the animation is running.
    uint16_t fadeAnimationCounter;             ///< To the current step in the animation
    uint16_t fadeAnimationDelay;               ///< A delay that is applied before animation start. Expressed in ticks.
    uint16_t fadeAnimationDuration;            ///< The complete duration of the animation. Expressed in ticks.
    int16_t fadeAnimationStartAlpha;           ///< The alpha value at the beginning of the animation.
    int16_t fadeAnimationEndAlpha;             ///< The alpha value at the end of the animation.
    EasingEquation fadeAnimationAlphaEquation; ///< EasingEquation expressing the progression of the alpha value during the animation.

    GenericCallback<const FadeAnimator<T>&>* fadeAnimationEndedCallback; ///< Animation ended Callback.
};

} // namespace touchgfx

#endif // TOUCHGFX_FADEANIMATOR_HPP
