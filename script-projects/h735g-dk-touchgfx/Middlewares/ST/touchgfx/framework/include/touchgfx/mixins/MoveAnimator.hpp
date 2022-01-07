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
 * @file touchgfx/mixins/MoveAnimator.hpp
 *
 * Declares the touchgfx::MoveAnimator class.
 */
#ifndef TOUCHGFX_MOVEANIMATOR_HPP
#define TOUCHGFX_MOVEANIMATOR_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Application.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{
/**
 * A MoveAnimator makes the template class T able to animate a movement from its current
 * position to a specified end position. The speed of the movement in both the X and Y
 * direction can be controlled by supplying EasingEquations. The MoveAnimator performs a
 * callback when the animation has finished.
 *
 * This mixin can be used on any Drawable.
 */
template <class T>
class MoveAnimator : public T
{
public:
    MoveAnimator()
        : T(),
          moveAnimationRunning(false),
          moveAnimationCounter(0),
          moveAnimationDelay(0),
          moveAnimationDuration(0),
          moveAnimationStartX(0),
          moveAnimationStartY(0),
          moveAnimationEndX(0),
          moveAnimationEndY(0),
          moveAnimationXEquation(),
          moveAnimationYEquation(),
          moveAnimationEndedCallback(0)
    {
    }

    /**
     * Associates an action to be performed when the animation ends.
     *
     * @param  callback The callback to be executed. The callback will be given a reference
     *                  to the MoveAnimator.
     *
     * @see clearMoveAnimationEndedAction
     */
    void setMoveAnimationEndedAction(GenericCallback<const MoveAnimator<T>&>& callback)
    {
        moveAnimationEndedCallback = &callback;
    }

    /**
     * Clears the move animation ended action previously set by setMoveAnimationEndedAction.
     * The effect is that any action set using setMoveAnimationEndedAction() will not be
     * executed.
     *
     * @see setMoveAnimationEndedAction
     */
    void clearMoveAnimationEndedAction()
    {
        moveAnimationEndedCallback = 0;
    }

    /**
     * Sets a delay on animations done by the MoveAnimator.
     *
     * @param  delay The delay in ticks.
     *
     * @see getMoveAnimationDelay
     */
    virtual void setMoveAnimationDelay(uint16_t delay)
    {
        moveAnimationDelay = delay;
    }

    /**
     * Gets the current animation delay.
     *
     * @return The current animation delay.
     *
     * @see setMoveAnimationDelay
     */
    virtual uint16_t getMoveAnimationDelay() const
    {
        return moveAnimationDelay;
    }

    /**
     * Gets whether or not the move animation is running.
     *
     * @return true if the move animation is running.
     */
    bool isMoveAnimationRunning() const
    {
        return moveAnimationRunning;
    }

    /**
     * Starts the move animation from the current position to the specified end position.
     * The development of the position (X, Y) during the animation is described by the
     * supplied EasingEquations. If no easing equation is given, the movement is performed
     * linear.
     *
     * @param  endX                 The X position at animation end.
     * @param  endY                 The Y position at animation end.
     * @param  duration             The duration of the animation measured in ticks.
     * @param  xProgressionEquation (Optional) The equation that describes the development of the
     *                              X position during the animation. Default is
     *                              EasingEquations::linearEaseNone.
     * @param  yProgressionEquation (Optional) The equation that describes the development of the
     *                              Y position during the animation. Default is
     *                              EasingEquations::linearEaseNone.
     */
    void startMoveAnimation(int16_t endX, int16_t endY, uint16_t duration, EasingEquation xProgressionEquation = &EasingEquations::linearEaseNone, EasingEquation yProgressionEquation = &EasingEquations::linearEaseNone)
    {
        if (!moveAnimationRunning)
        {
            Application::getInstance()->registerTimerWidget(this);
        }

        moveAnimationCounter = 0;
        moveAnimationStartX = T::getX();
        moveAnimationStartY = T::getY();
        moveAnimationEndX = endX;
        moveAnimationEndY = endY;
        moveAnimationDuration = duration;
        moveAnimationXEquation = xProgressionEquation;
        moveAnimationYEquation = yProgressionEquation;

        moveAnimationRunning = true;

        if (moveAnimationDelay == 0 && moveAnimationDuration == 0)
        {
            nextMoveAnimationStep(); // Set end position and shut down
        }
    }

    /**
     * Cancel move animation and leave the Drawable in its current position. If the
     * animation is not running, nothing is done.
     */
    void cancelMoveAnimation()
    {
        if (moveAnimationRunning)
        {
            Application::getInstance()->unregisterTimerWidget(this);
            moveAnimationRunning = false;
        }
    }

    /** The tick handler that handles the actual animation steps. */
    virtual void handleTickEvent()
    {
        T::handleTickEvent();
        nextMoveAnimationStep();
    }

protected:
    /** Execute next step in move animation and stop the timer if the animation has finished. */
    void nextMoveAnimationStep()
    {
        if (moveAnimationRunning)
        {
            moveAnimationCounter++;
            if (moveAnimationCounter >= moveAnimationDelay)
            {
                // Adjust the used animationCounter for the startup delay
                uint32_t actualAnimationCounter = moveAnimationCounter - moveAnimationDelay;

                int16_t deltaX = moveAnimationXEquation(actualAnimationCounter, 0, moveAnimationEndX - moveAnimationStartX, moveAnimationDuration);
                int16_t deltaY = moveAnimationYEquation(actualAnimationCounter, 0, moveAnimationEndY - moveAnimationStartY, moveAnimationDuration);

                T::moveTo(moveAnimationStartX + deltaX, moveAnimationStartY + deltaY);

                if (moveAnimationCounter >= (uint32_t)(moveAnimationDelay + moveAnimationDuration))
                {
                    // End of animation
                    moveAnimationRunning = false;
                    moveAnimationCounter = 0;
                    Application::getInstance()->unregisterTimerWidget(this);

                    if (moveAnimationEndedCallback && moveAnimationEndedCallback->isValid())
                    {
                        moveAnimationEndedCallback->execute(*this);
                    }
                }
            }
        }
    }

    bool moveAnimationRunning;             ///< True if the animation is running
    uint16_t moveAnimationCounter;         ///< Counter that is equal to the current step in the animation
    uint16_t moveAnimationDelay;           ///< The delay applied before animation start. Expressed in ticks.
    uint16_t moveAnimationDuration;        ///< The complete duration of the actual animation. Expressed in ticks.
    int16_t moveAnimationStartX;           ///< The X value at the beginning of the animation.
    int16_t moveAnimationStartY;           ///< The Y value at the beginning of the animation.
    int16_t moveAnimationEndX;             ///< The X value at the end of the animation.
    int16_t moveAnimationEndY;             ///< The Y value at the end of the animation.
    EasingEquation moveAnimationXEquation; ///< EasingEquation expressing the development of the X value during the animation.
    EasingEquation moveAnimationYEquation; ///< EasingEquation expressing the development of the Y value during the animation.

    GenericCallback<const MoveAnimator<T>&>* moveAnimationEndedCallback; ///< Animation ended Callback.
};

} // namespace touchgfx

#endif // TOUCHGFX_MOVEANIMATOR_HPP
