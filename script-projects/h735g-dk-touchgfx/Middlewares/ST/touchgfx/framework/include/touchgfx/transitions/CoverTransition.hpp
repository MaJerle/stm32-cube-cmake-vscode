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
 * @file touchgfx/transitions/CoverTransition.hpp
 *
 * Declares the touchgfx::CoverTransition class.
 */
#ifndef TOUCHGFX_COVERTRANSITION_HPP
#define TOUCHGFX_COVERTRANSITION_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/transitions/Transition.hpp>
#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
/**
 * A Transition that slides the new screen over the previous \e from the given direction.
 */
template <Direction templateDirection>
class CoverTransition : public Transition
{
public:
    /**
     * Initializes a new instance of the CoverTransition class.
     *
     * @param  transitionSteps (Optional) Number of steps in the transition animation.
     */
    CoverTransition(const uint8_t transitionSteps = 20)
        : Transition(),
          handleTickCallback(this, &CoverTransition::tickMoveDrawable),
          animationSteps(transitionSteps),
          animationCounter(0),
          calculatedValue(0),
          movedToPos(0),
          solid()
    {
        switch (templateDirection)
        {
        case EAST:
            targetValue = -HAL::DISPLAY_WIDTH;
            break;
        case WEST:
            targetValue = HAL::DISPLAY_WIDTH;
            break;
        case NORTH:
            targetValue = HAL::DISPLAY_HEIGHT;
            break;
        case SOUTH:
            targetValue = -HAL::DISPLAY_HEIGHT;
            break;
        default:
            done = true;
            // Nothing to do here
            break;
        }

        // Ensure that the solid area covers the entire screen
        solid.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
    }

    /**
     * Handles the tick event when transitioning. It moves the contents of the Screen's
     * container. The direction of the transition determines the direction the contents of
     * the container moves.
     */
    virtual void handleTickEvent()
    {
        Transition::handleTickEvent();

        // Calculate new position or stop animation
        animationCounter++;
        if (animationCounter <= animationSteps)
        {
            // Calculate value in [0;targetValue]
            calculatedValue = EasingEquations::cubicEaseOut(animationCounter, 0, targetValue, animationSteps);

            // Note: Result of "calculatedValue & 1" is compiler dependent for negative values of calculatedValue
            if (calculatedValue % 2)
            {
                // Optimization: calculatedValue is odd, add 1/-1 to move drawables modulo 32 bits in framebuffer
                calculatedValue += (calculatedValue > 0 ? 1 : -1);
            }
        }
        else
        {
            // Final step: stop the animation
            done = true;
            animationCounter = 0;
            return;
        }

        // Convert the calculated value to delta value relative to current moved-to position
        calculatedValue -= movedToPos;
        movedToPos += calculatedValue;

        // The Cover Transition only draws to parts of the non-TFT framebuffer. To avoid glitches
        // In Double buffering mode both framebuffers must be identical.
        //
        // The first tick of a Cover Transition will cover "calculatedValue" pixels vertically or horizontally
        // depending on the speed of the transition, so there's no need to transfer that.
        if (animationCounter == 1 && HAL::USE_DOUBLE_BUFFERING)
        {
            Rect rect;
            switch (templateDirection)
            {
            case EAST:
                rect.x = 0;
                rect.y = 0;
                rect.width = HAL::DISPLAY_WIDTH + calculatedValue;
                rect.height = HAL::DISPLAY_HEIGHT;
                break;
            case WEST:
                rect.x = calculatedValue;
                rect.y = 0;
                rect.width = HAL::DISPLAY_WIDTH - calculatedValue;
                rect.height = HAL::DISPLAY_HEIGHT;
                break;
            case NORTH:
                rect.x = 0;
                rect.y = calculatedValue;
                rect.width = HAL::DISPLAY_WIDTH;
                rect.height = HAL::DISPLAY_HEIGHT - calculatedValue;
                break;
            case SOUTH:
                rect.x = 0;
                rect.y = 0;
                rect.width = HAL::DISPLAY_WIDTH;
                rect.height = HAL::DISPLAY_HEIGHT + calculatedValue;
                break;
            default:
                // Nothing to do here
                break;
            }

            // Get the currently displayed framebuffer
            uint16_t* tftFb = HAL::getInstance()->getTFTFrameBuffer();

            Rect source;
            source.x = 0;
            source.y = 0;
            source.width = HAL::DISPLAY_WIDTH;
            source.height = HAL::DISPLAY_HEIGHT;

            // Copy rect from tft to client framebuffer
            HAL::getInstance()->lcd().blitCopy((const uint16_t*)tftFb, source, rect, 255, false);
        }

        // Move children with delta value for X or Y
        screenContainer->forEachChild(&handleTickCallback);
    }

    virtual void tearDown()
    {
        screenContainer->remove(solid);
    }

    virtual void init()
    {
        Transition::init();
        Callback<CoverTransition, Drawable&> initCallback(this, &CoverTransition::initMoveDrawable);
        screenContainer->forEachChild(&initCallback);
        screenContainer->add(solid);
    }

protected:
    /**
     * Moves the Drawable to its initial position just outside of the visible area of the
     * display.
     *
     * @param [in] d The Drawable to move.
     */
    virtual void initMoveDrawable(Drawable& d)
    {
        switch (templateDirection)
        {
        case EAST:
            d.moveRelative(HAL::DISPLAY_WIDTH, 0);
            break;
        case WEST:
            d.moveRelative(-HAL::DISPLAY_WIDTH, 0);
            break;
        case NORTH:
            d.moveRelative(0, -HAL::DISPLAY_HEIGHT);
            break;
        case SOUTH:
            d.moveRelative(0, HAL::DISPLAY_HEIGHT);
            break;
        default:
            // Nothing to do here
            break;
        }
    }

    /**
     * Moves the Drawable to the new position as calculated in handleTickEvent().
     *
     * @param [in] d The Drawable to move.
     */
    virtual void tickMoveDrawable(Drawable& d)
    {
        switch (templateDirection)
        {
        case EAST:
        case WEST:
            d.moveRelative(calculatedValue, 0);
            break;
        case NORTH:
        case SOUTH:
            d.moveRelative(0, calculatedValue);
            break;
        default:
            // Special case, do not move. Class NoTransition can be used instead.
            done = true;
            break;
        }
    }

private:
    class FullSolidRect : public Widget
    {
    public:
        virtual Rect getSolidRect() const
        {
            return Rect(0, 0, getWidth(), getHeight());
        }

        virtual void draw(const Rect& area) const
        {
        }
    };

    Callback<CoverTransition, Drawable&> handleTickCallback; ///< Callback used for tickMoveDrawable().

    const uint8_t animationSteps; ///< Number of steps the transition should move per complete animation.
    uint8_t animationCounter;     ///< Current step in the transition animation.
    int16_t targetValue;          ///< The target value for the transition animation.
    int16_t calculatedValue;      ///< The calculated X or Y value to move the snapshot and the children.
    int16_t movedToPos;
    FullSolidRect solid; ///< A solid rect that covers the entire screen to avoid copying elements outside
};

} // namespace touchgfx

#endif // TOUCHGFX_COVERTRANSITION_HPP
