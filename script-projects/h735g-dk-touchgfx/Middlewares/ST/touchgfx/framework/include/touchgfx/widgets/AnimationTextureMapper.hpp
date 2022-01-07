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
 * @file touchgfx/widgets/AnimationTextureMapper.hpp
 *
 * Declares the touchgfx::AnimationTextureMapper class.
 */
#ifndef TOUCHGFX_ANIMATIONTEXTUREMAPPER_HPP
#define TOUCHGFX_ANIMATIONTEXTUREMAPPER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>

namespace touchgfx
{
/**
 * A TextureMapper with animation capabilities. Note that the angles of the TextureMapper is
 * normalized to lie in the range [0; 2PI[ at the beginning at the animation. The end
 * angles should be relative to this and are limited to values in the range [-32.7;
 * 32.7].
 */
class AnimationTextureMapper : public TextureMapper
{
public:
    /** Values that represent different animation parameter. */
    enum AnimationParameter
    {
        X_ROTATION, ///< Rotation around the X axis
        Y_ROTATION, ///< Rotation around the Y axis
        Z_ROTATION, ///< Rotation around the Z axis
        SCALE       ///< Scaling of the image
    };

    static const int NUMBER_OF_ANIMATION_PARAMETERS = SCALE + 1; ///< Number of animation parameters

    AnimationTextureMapper();

    /**
     * Associates an action to be performed for every step in the animation. Will not be called
     * during the delay period.
     *
     * @param  callback The callback to be executed. The callback will be given a reference
     *                  to the AnimationTextureMapper.
     *
     * @see GenericCallback
     */
    void setTextureMapperAnimationStepAction(GenericCallback<const AnimationTextureMapper&>& callback);

    /**
     * Associates an action to be performed when the animation ends.
     *
     * @param  callback The callback to be executed. The callback will be given a reference
     *                  to the AnimationTextureMapper.
     *
     * @see GenericCallback
     */
    void setTextureMapperAnimationEndedAction(GenericCallback<const AnimationTextureMapper&>& callback);

    /**
     * Gets whether or not the animation is running.
     *
     * @return true if the animation is running.
     */
    virtual bool isTextureMapperAnimationRunning() const;

    /**
     * Sets up the animation for a specific parameter (angle/scale) for the next animation.
     * The specific parameter is chosen using the AnimationType enum. AnimationTypes that
     * are not setup using this method will keep their value during the animation.
     *
     * @param  parameter           The parameter of the TextureMapper that should be animated.
     * @param  endValue            The end value for the parameter.
     * @param  duration            The duration for the animation of the parameter. Specified in
     *                             ticks.
     * @param  delay               The delay before the animation of the parameter starts.
     *                             Specified in ticks.
     * @param  progressionEquation (Optional) the progression equation for the animation of this
     *                             parameter. Default is EasingEquations::linearEaseNone.
     */
    virtual void setupAnimation(AnimationParameter parameter, float endValue, uint16_t duration, uint16_t delay, EasingEquation progressionEquation = &EasingEquations::linearEaseNone);

    /**
     * Starts the animation from the current position to the specified end angles/scale, as
     * specified by one or more calls to setupAnimation().
     */
    virtual void startAnimation();

    /**
     * Cancel move animation. Stops any running animation at the current position regardless
     * of the progress made so far. Disables all animation parameters set using
     * setupAnimation and mark the animation as stopped.
     */
    virtual void cancelAnimationTextureMapperAnimation();

    /**
     * Gets the current animation step measured in ticks since the call to startAnimation().
     * The steps during the initial delay are also counted.
     *
     * @return The current animation step.
     */
    virtual uint16_t getAnimationStep();

    virtual void handleTickEvent();

protected:
    /** Information about how a specific animation parameter should be animated. */
    struct AnimationSetting
    {
        bool animationActive;                        ///< Should this animation be performed?
        float animationStart;                        ///< The animation start value
        float animationEnd;                          ///< The animation end value
        uint16_t animationDelay;                     ///< A delay before the actual animation start. Expressed in ticks.
        uint16_t animationDuration;                  ///< The complete duration of the animation. Expressed in ticks.
        EasingEquation animationProgressionEquation; ///< EasingEquation expressing the development of the value during the animation.
    };

    AnimationSetting animations[NUMBER_OF_ANIMATION_PARAMETERS]; ///< Descriptions of the animation of specific animation parameters

    GenericCallback<const AnimationTextureMapper&>* textureMapperAnimationStepCallback;  ///< Callback that is executed after every step of the animation.
    GenericCallback<const AnimationTextureMapper&>* textureMapperAnimationEndedCallback; ///< Callback that is executed after the animation ends.

    uint16_t animationCounter; ///< Counter that is equal to the current step in the animation
    bool animationRunning;     ///< Boolean that is true if the animation is running
};

} // namespace touchgfx

#endif // TOUCHGFX_ANIMATIONTEXTUREMAPPER_HPP
