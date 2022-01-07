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
 * @file touchgfx/widgets/AnimatedImage.hpp
 *
 * Declares the touchgfx::AnimatedImage class.
 */
#ifndef TOUCHGFX_ANIMATEDIMAGE_HPP
#define TOUCHGFX_ANIMATEDIMAGE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/widgets/Image.hpp>

namespace touchgfx
{
/**
 * A widget capable of basic animation using a range of bitmaps. The AnimatedImage is capable of
 * running the animation from start to end or, in reverse order, end to start. It is
 * capable of doing a single animation or looping the animation until stopped or paused.
 */
class AnimatedImage : public Image
{
public:
    /**
     * Constructs an AnimatedImage. The start and the end specifies the range of bitmaps to
     * be used for animation. The update interval defines how often the animation should be
     * updated. The animation will iterate over the bitmaps that lies between the IDs of
     * start and end, both included.
     *
     * @param  start          Defines the start of the range of images in the animation.
     * @param  end            Defines the end of the range of images in the animation.
     * @param  updateInterval (Optional) Defines the number of ticks between each animation step.
     *                        Higher value results in a slower animation. Default is to
     *                        update the image on every tick.
     */
    AnimatedImage(const BitmapId& start, const BitmapId& end, const uint8_t& updateInterval = 1)
        : Image(Bitmap(start)),
          animationDoneAction(0),
          startId(start),
          endId(end),
          updateTicksInterval(updateInterval),
          ticksSinceUpdate(0),
          reverse(false),
          loopAnimation(false),
          running(false)
    {
    }

    /**
     * Constructs an AnimatedImage without initializing bitmaps.
     *
     * @param  updateInterval (Optional) Defines the number of ticks between each animation step.
     *                        Higher value results in a slower animation.
     *
     * @note The bitmaps to display must be configured through set setBitmaps function before this
     *       widget displays anything.
     */
    AnimatedImage(const uint8_t& updateInterval = 1)
        : animationDoneAction(0),
          startId(BITMAP_INVALID),
          endId(BITMAP_INVALID),
          updateTicksInterval(updateInterval),
          ticksSinceUpdate(0),
          reverse(false),
          loopAnimation(false),
          running(false)
    {
    }

    /**
     * Starts the animation with the given parameters for animation direction, normal or
     * reverse, whether to restart the animation and finally if the animation should loop
     * automatically upon completion.
     *
     * @param  rev   Defines if the animation should be performed in reverse order.
     * @param  reset (Optional) Defines if the animation should reset and start from the first (or
     *               last if reverse order) bitmap.
     * @param  loop  (Optional) Defines if the animation should loop or do a single animation.
     */
    virtual void startAnimation(const bool rev, const bool reset = false, const bool loop = false);

    /**
     * Stops and resets the animation. If the animation should not reset to the first image
     * in the animation sequence, use pauseAnimation().
     *
     * @see startAnimation, pauseAnimation
     */
    virtual void stopAnimation();

    /**
     * Toggles the running state of an animation. Pauses the animation if the animation is
     * running. Continues the animation if previously paused.
     *
     * @see stopAnimation
     */
    virtual void pauseAnimation();

    virtual void handleTickEvent();

    /**
     * Associates an action to be performed when the animation of the AnimatedImage is done.
     * If the animation is set to loop at the end, the action is also triggered when the
     * animation starts over.
     *
     * @param  callback The callback is executed when done. The callback is given the
     *                  animated image.
     */
    void setDoneAction(GenericCallback<const AnimatedImage&>& callback)
    {
        animationDoneAction = &callback;
    }

    /**
     * Gets the running state of the AnimatedImage.
     *
     * @return true if the animation is currently running, false otherwise.
     */
    bool isAnimatedImageRunning() const
    {
        return running;
    }

    /**
     * Query if this object is running in reverse.
     *
     * @return true if the animation is performed in reverse order.
     */
    bool isReverse()
    {
        return reverse;
    }

    /**
     * @copydoc Image::setBitmap(const Bitmap&)
     *
     * @see setBitmaps, setBitmapEnd
     *
     * @note This only sets the start image.
     */
    virtual void setBitmap(const Bitmap& bitmap);

    /**
     * Sets the end bitmap for this AnimatedImage sequence.
     *
     * @param  bitmap The bitmap.
     *
     * @see setBitmaps, setBitmap
     */
    virtual void setBitmapEnd(const Bitmap& bitmap);

    /**
     * Sets the bitmaps that are used by the animation.
     *
     * The animation will iterate over the bitmaps that lies between the IDs of start and
     * end, both inclusive.
     *
     * @param  start Defines the start of the range of images in the animation.
     * @param  end   Defines the end of the range of images in the animation.
     *
     * @see setBitmap, setBitmapEnd
     */
    void setBitmaps(BitmapId start, BitmapId end);

    /**
     * Sets the update interval. The value specifies the number of ticks between each step
     * of the animation. The default update interval for animated images is 1, which means
     * results in the fastest possible animation.
     *
     * @param  updateInterval Defines the number of ticks between each animation step. Higher value
     *                        results in a slower animation.
     */
    void setUpdateTicksInterval(uint8_t updateInterval);

protected:
    GenericCallback<const AnimatedImage&>* animationDoneAction; ///< Pointer to the callback to be executed when animation is done.

    BitmapId startId;            ///< Id of first bitmap in animation.
    BitmapId endId;              ///< Id of last bitmap in animation.
    uint8_t updateTicksInterval; ///< Number of ticks between each animation update (image change).
    uint8_t ticksSinceUpdate;    ///< Number of ticks since last animation update.
    bool reverse;                ///< If true, run in reverse direction (last to first).
    bool loopAnimation;          ///< If true, continuously loop animation.
    bool running;                ///< If true, animation is running.
};

} // namespace touchgfx

#endif // TOUCHGFX_ANIMATEDIMAGE_HPP
